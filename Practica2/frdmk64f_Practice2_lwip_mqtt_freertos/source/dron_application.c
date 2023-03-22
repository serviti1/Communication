/*
 * dron_application.c
 *
 *  Created on: 20 mar 2023
 *      Author: jesus
 */

#include "dron_application.h"
#include "config.h"
#include "board.h"

static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};

/*! @brief MQTT client data. */
static mqtt_client_t *mqtt_client;

/*! @brief MQTT client ID string. */
static char client_id[40];

static char client_user[] = "Jesus";

static char client_pass[] = "clase2023";

/*! @brief MQTT client information. */
static const struct mqtt_connect_client_info_t mqtt_client_info = {
    .client_id   = (const char *)&client_id[0],
    .client_user = (const char *)&client_user[0],
    .client_pass = (const char *)&client_pass[0],
    .keep_alive  = 100,
    .will_topic  = NULL,
    .will_msg    = NULL,
    .will_qos    = 0,
    .will_retain = 0,
#if LWIP_ALTCP && LWIP_ALTCP_TLS
    .tls_config = NULL,
#endif
};

/*! @brief MQTT broker IP address. */
static ip_addr_t mqtt_addr;

/*! @brief Indicates connection to MQTT broker. */
static volatile bool connected = false;

uint8_t Button0_Pressed;
uint8_t Button1_Pressed;

static const char* Filters[4] = {"Gaussian Filter", "Median Filter", "Box Filter", "Kalman Filter"};

uint8_t cameraStatus_Selector = 0;
uint8_t batteryStatus_Selector = 0;
static const char* CameraStatusMessages[3] = {"No issue", "Warning", "Error"};
static const char* BatteryStatusMessages[5] = {"100%", "80%", "60%", "40%", "20%"};
dronStatus DronApplication = {
	.batteryStatus = NULL,
	.cameraStatus = "Error",
	.position.x = "00.000",
	.position.y = "000.000",
	.position.altitude = "000",
	.temperatureThreshold = "200",
	.Alarm = 0,
};


/********************************************************
 *					Function Definition					*
 *******************************************************/
static void mqtt_topic_subscribed_cb(void *arg, err_t err);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
static void mqtt_subscribe_topics(mqtt_client_t *client);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void connect_to_mqtt(void *ctx);
static void mqtt_message_published_cb(void *arg, err_t err);
static void publish_message(void *ctx);
static void app_thread(void *arg);

void BOARD_SW3_IRQ_HANDLER (void){
	/* Clear external interrupt flag. */
	GPIO_PortClearInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN);
	Button0_Pressed = 1;
}

void BOARD_SW2_IRQ_HANDLER (void){
	/* Clear external interrupt flag. */
	GPIO_PortClearInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);
	Button1_Pressed = 1;
}

/*!
 * @brief Called when subscription request finishes.
 */
static void mqtt_topic_subscribed_cb(void *arg, err_t err)
{
    const char *topic = (const char *)arg;

    if (err == ERR_OK)
    {
        PRINTF("Subscribed to the topic \"%s\".\r\n", topic);
    }
    else
    {
        PRINTF("Failed to subscribe to the topic \"%s\": %d.\r\n", topic, err);
    }
}

/*!
 * @brief Called when there is a message on a subscribed topic.
 */
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    LWIP_UNUSED_ARG(arg);

    PRINTF("Received %u bytes from the topic \"%s\"", tot_len, topic);
}

/*!
 * @brief Called when recieved incoming published message fragment.
 */
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    int i;

    LWIP_UNUSED_ARG(arg);

    if('x' == data[0]){
		for(i = 0; i<6; i++){
			DronApplication.position.x[i] = data[i+2];
			DronApplication.position.y[i] = data[i+12];
		}
		DronApplication.position.y[6] = data[len-2];
		DronApplication.position.y[7] = data[len-1];
		PRINTF("\n\rGoing to coordinates ");
	} else if('C' == data[len-1]){
    	for(i = 0; i<3; i++){
    		DronApplication.temperatureThreshold[i] = data[i];
    	}
    	PRINTF("\n\rTemperature Threshold set to: ");
    } else if ('A' == data[0]) {
    	for(i = 0; i<3; i++){
    		DronApplication.position.altitude[i] = data[i+2];
    	}
    	PRINTF("\n\rGoing to altitude: ");
    }

    for (i = 0; i < len; i++)
    {
        if (isprint(data[i]))
        {
            PRINTF("%c", (char)data[i]);
        }
        else
        {
            PRINTF("\\x%02x", data[i]);
        }
    }

    if (flags & MQTT_DATA_FLAG_LAST)
    {
        PRINTF("\"\r\n");
    }
}

/*!
 * @brief Subscribe to MQTT topics.
 */
static void mqtt_subscribe_topics(mqtt_client_t *client)
{
    static const char *topics[] = {"jesus/dron/altitude", "jesus/dron/coordinates", "jesus/dron/TemperatureSelector"};
    int qos[]                   = {1, 1, 1};
    err_t err;
    int i;

    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb,
                            LWIP_CONST_CAST(void *, &mqtt_client_info));

    for (i = 0; i < ARRAY_SIZE(topics); i++)
    {
        err = mqtt_subscribe(client, topics[i], qos[i], mqtt_topic_subscribed_cb, LWIP_CONST_CAST(void *, topics[i]));

        if (err == ERR_OK)
        {
            PRINTF("Subscribing to the topic \"%s\" with QoS %d...\r\n", topics[i], qos[i]);
        }
        else
        {
            PRINTF("Failed to subscribe to the topic \"%s\" with QoS %d: %d.\r\n", topics[i], qos[i], err);
        }
    }
}

/*!
 * @brief Called when connection state changes.
 */
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    const struct mqtt_connect_client_info_t *client_info = (const struct mqtt_connect_client_info_t *)arg;

    connected = (status == MQTT_CONNECT_ACCEPTED);

    switch (status)
    {
        case MQTT_CONNECT_ACCEPTED:
            PRINTF("MQTT client \"%s\" connected.\r\n", client_info->client_id);
            mqtt_subscribe_topics(client);
            break;

        case MQTT_CONNECT_DISCONNECTED:
            PRINTF("MQTT client \"%s\" not connected.\r\n", client_info->client_id);
            /* Try to reconnect 1 second later */
            sys_timeout(1000, connect_to_mqtt, NULL);
            break;

        case MQTT_CONNECT_TIMEOUT:
            PRINTF("MQTT client \"%s\" connection timeout.\r\n", client_info->client_id);
            /* Try again 1 second later */
            sys_timeout(1000, connect_to_mqtt, NULL);
            break;

        case MQTT_CONNECT_REFUSED_PROTOCOL_VERSION:
        case MQTT_CONNECT_REFUSED_IDENTIFIER:
        case MQTT_CONNECT_REFUSED_SERVER:
        case MQTT_CONNECT_REFUSED_USERNAME_PASS:
        case MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_:
            PRINTF("MQTT client \"%s\" connection refused: %d.\r\n", client_info->client_id, (int)status);
            /* Try again 10 seconds later */
            sys_timeout(10000, connect_to_mqtt, NULL);
            break;

        default:
            PRINTF("MQTT client \"%s\" connection status: %d.\r\n", client_info->client_id, (int)status);
            /* Try again 10 seconds later */
            sys_timeout(10000, connect_to_mqtt, NULL);
            break;
    }
}

/*!
 * @brief Starts connecting to MQTT broker. To be called on tcpip_thread.
 */
static void connect_to_mqtt(void *ctx)
{
    LWIP_UNUSED_ARG(ctx);

    PRINTF("Connecting to MQTT broker at %s...\r\n", ipaddr_ntoa(&mqtt_addr));

    mqtt_client_connect(mqtt_client, &mqtt_addr, EXAMPLE_MQTT_SERVER_PORT, mqtt_connection_cb,
                        LWIP_CONST_CAST(void *, &mqtt_client_info), &mqtt_client_info);
}

/*!
 * @brief Called when publish request finishes.
 */
static void mqtt_message_published_cb(void *arg, err_t err)
{
    const char *topic = (const char *)arg;

    if (err == ERR_OK)
    {
        PRINTF("Published to the topic \"%s\".\r\n", topic);
    }
    else
    {
        PRINTF("Failed to publish to the topic \"%s\": %d.\r\n", topic, err);
    }
}

/*!
 * @brief Publishes a message. To be called on tcpip_thread.
 */
static void publish_message(void *ctx)
{
    static const char *topic   = "lwip_topic/100";
    static const char *message = "message from board";

    LWIP_UNUSED_ARG(ctx);

    PRINTF("Going to publish to the topic \"%s\"...\r\n", topic);

    mqtt_publish(mqtt_client, topic, message, strlen(message), 1, 0, mqtt_message_published_cb, (void *)topic);
}

/*!
 * @brief Publishes a message. To be called on tcpip_thread.
 */
static void publish_alert(void *ctx)
{
    static const char *topic   = "jesus/dron/alert";
    static const char *messageOn = "ON";
    static const char *messageOff = "OFF";

    LWIP_UNUSED_ARG(ctx);

    DronApplication.Alarm ^= 1;

    PRINTF("Going to publish to the topic \"%s\"...\r\n", topic);

    if(DronApplication.Alarm){
        mqtt_publish(mqtt_client, topic, messageOn, strlen(messageOn), 1, 0, mqtt_message_published_cb, (void *)topic);
    } else {
        mqtt_publish(mqtt_client, topic, messageOff, strlen(messageOff), 1, 0, mqtt_message_published_cb, (void *)topic);
    }
}

/*!
 * @brief Publishes a message. To be called on tcpip_thread.
 */
static void publish_status(void *ctx)
{
    static const char *BatteryTopic   = "jesus/dron/status/Battery";
    static const char *CameraTopic = "jesus/dron/status/Camera";

    LWIP_UNUSED_ARG(ctx);

    DronApplication.batteryStatus = (char *) &BatteryStatusMessages[batteryStatus_Selector][0];
    DronApplication.cameraStatus = (char *)&CameraStatusMessages[cameraStatus_Selector][0];

    PRINTF("Going to publish to the topic \"%s\"...\r\n", BatteryTopic);
    PRINTF("Going to publish to the topic \"%s\"...\r\n", CameraTopic);


    mqtt_publish(mqtt_client, BatteryTopic, DronApplication.batteryStatus, strlen(DronApplication.batteryStatus), 1, 0, mqtt_message_published_cb, (void *)BatteryTopic);
    mqtt_publish(mqtt_client, CameraTopic, DronApplication.cameraStatus, strlen(DronApplication.cameraStatus), 1, 0, mqtt_message_published_cb, (void *)CameraTopic);

    batteryStatus_Selector++;
    cameraStatus_Selector++;

    if(5 == batteryStatus_Selector){
    	batteryStatus_Selector = 0;
    }
    if(3 == cameraStatus_Selector){
		cameraStatus_Selector = 0;
	}
}

/*!
 * @brief Application thread.
 */
static void app_thread(void *arg)
{
    struct netif *netif = (struct netif *)arg;
    struct dhcp *dhcp;
    err_t err;
    int i;

    /* Wait for address from DHCP */

    PRINTF("Getting IP address from DHCP...\r\n");

    do
    {
        if (netif_is_up(netif))
        {
            dhcp = netif_dhcp_data(netif);
        }
        else
        {
            dhcp = NULL;
        }

        sys_msleep(20U);

    } while ((dhcp == NULL) || (dhcp->state != DHCP_STATE_BOUND));

    PRINTF("\r\nIPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
    PRINTF("IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
    PRINTF("IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));

    /*
     * Check if we have an IP address or host name string configured.
     * Could just call netconn_gethostbyname() on both IP address or host name,
     * but we want to print some info if goint to resolve it.
     */
    if (ipaddr_aton(EXAMPLE_MQTT_SERVER_HOST, &mqtt_addr) && IP_IS_V4(&mqtt_addr))
    {
        /* Already an IP address */
        err = ERR_OK;
    }
    else
    {
        /* Resolve MQTT broker's host name to an IP address */
        PRINTF("Resolving \"%s\"...\r\n", EXAMPLE_MQTT_SERVER_HOST);
        err = netconn_gethostbyname(EXAMPLE_MQTT_SERVER_HOST, &mqtt_addr);
    }

    if (err == ERR_OK)
    {
        /* Start connecting to MQTT broker from tcpip_thread */
        err = tcpip_callback(connect_to_mqtt, NULL);
        if (err != ERR_OK)
        {
            PRINTF("Failed to invoke broker connection on the tcpip_thread: %d.\r\n", err);
        }
    }
    else
    {
        PRINTF("Failed to obtain IP address: %d.\r\n", err);
    }

    /* Publish some messages */
    while(1)
    {
        if (connected)
        {
        	if(1 == Button0_Pressed){
        		err = tcpip_callback((publish_alert), NULL);
        		if (err != ERR_OK)
        		{
        			PRINTF("Failed to invoke publishing of a message on the tcpip_thread: %d.\r\n", err);
				}
        		Button0_Pressed = 0;
        	}
        	if(1 == Button1_Pressed){
        		err = tcpip_callback((publish_status), NULL);
				if (err != ERR_OK)
				{
					PRINTF("Failed to invoke publishing of a message on the tcpip_thread: %d.\r\n", err);
				}
				Button1_Pressed = 0;
        	}
        }
    }

    vTaskDelete(NULL);
}

static void generate_client_id(void)
{
    uint32_t mqtt_id[MQTT_ID_SIZE];
    int res;

    get_mqtt_id(&mqtt_id[0]);

    res = snprintf(client_id, sizeof(client_id), "nxp_%08lx%08lx%08lx%08lx", mqtt_id[3], mqtt_id[2], mqtt_id[1],
                   mqtt_id[0]);
    if ((res < 0) || (res >= sizeof(client_id)))
    {
        PRINTF("snprintf failed: %d\r\n", res);
        while (1)
        {
        }
    }
}

/*!
 * @brief Initializes lwIP stack.
 *
 * @param arg unused
 */
void stack_init(void *arg)
{
    static struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
    };

    LWIP_UNUSED_ARG(arg);
    generate_client_id();

    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    IP4_ADDR(&netif_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_gw, 0U, 0U, 0U, 0U);

    tcpip_init(NULL, NULL);

    LOCK_TCPIP_CORE();
    mqtt_client = mqtt_client_new();
    UNLOCK_TCPIP_CORE();
    if (mqtt_client == NULL)
    {
        PRINTF("mqtt_client_new() failed.\r\n");
        while (1)
        {
        }
    }

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    netifapi_dhcp_start(&netif);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" MQTT client example\r\n");
    PRINTF("************************************************\r\n");

    if (sys_thread_new("app_task", app_thread, &netif, APP_THREAD_STACKSIZE, APP_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("stack_init(): Task creation failed.", 0);
    }

    vTaskDelete(NULL);
}

