/*
 * SecureLayer.h
 *
 *  Created on: Feb 20, 2023
 *      Author: nxf63523
 */

#ifndef SECURELAYER_H_
#define SECURELAYER_H_

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

typedef struct netconn* SecureConnPtr;

enum{
	secureServer = 0,
	secureClient = 1,
};

SecureConnPtr initSecureConnection(uint8_t connectionType, const ip_addr_t *addr, uint16_t port);

uint8_t secureServerEcho(SecureConnPtr connection);

uint8_t secureTransmit(SecureConnPtr connection, uint8_t* sendBuffer, uint8_t length);

uint8_t secureReceive(SecureConnPtr connection, void* receiveBuffer, uint16_t* length);

uint8_t deinitSecureConnection(SecureConnPtr connection);

#endif

#endif /* SECURELAYER_H_ */
