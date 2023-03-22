/*
 * dron_application.h
 *
 *  Created on: 20 mar 2023
 *      Author: jesus
 */

#ifndef DRON_APPLICATION_H_
#define DRON_APPLICATION_H_

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "lwip/api.h"
#include "lwip/apps/mqtt.h"
#include "lwip/dhcp.h"
#include "lwip/netdb.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "lwip_mqtt_id.h"

#include "ctype.h"
#include "stdio.h"

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_device_registers.h"

void stack_init(void *arg);

typedef struct{
	char x[6];
	char y[8];
	char altitude[3];
}coordinates;

typedef struct {
	char* batteryStatus;
	char* cameraStatus;
	coordinates position;
	char temperatureThreshold[3];
	uint8_t Alarm;

}dronStatus;

#endif /* DRON_APPLICATION_H_ */
