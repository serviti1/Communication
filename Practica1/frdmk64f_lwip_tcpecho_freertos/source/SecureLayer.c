/*
 * SecureLayer.h
 *
 *  Created on: Feb 20, 2023
 *      Author: nxf63523
 */

#include "SecureLayer.h"
#include "aes.h"
#include "fsl_crc.h"

SecureConnPtr initSecureConnection(uint8_t connectionType, const ip_addr_t *addr, uint16_t port){
	SecureConnPtr conn, newconn;
	  err_t err;

	  if(secureServer == connectionType)
	  {
		  conn = netconn_new(NETCONN_TCP);

		  netconn_bind(conn, IP_ADDR_ANY, port);

		  LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

		  /* Tell connection to go into listening mode. */
		  err = netconn_listen(conn);

		  err = netconn_accept(conn, &newconn);

	  } else if (secureClient == connectionType) {
		  newconn = netconn_new(NETCONN_TCP);
		  err = netconn_connect(newconn, addr, port);
	  }

	  if(ERR_OK == err){
		  return newconn;
	  }

	  return conn;
}

SecureConnPtr secureServerConnect(SecureConnPtr conn){
	SecureConnPtr newconn;
	  err_t err;
	  err = netconn_listen(conn);
	  err = netconn_accept(conn, &newconn);

	  if(ERR_OK == err){
		  return newconn;
	  }
}

/*!
 * @brief Init for CRC-32.
 * @details Init CRC peripheral module for CRC-32 protocol.
 *          width=32 poly=0x04c11db7 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0xcbf43926
 *          name="CRC-32"
 *          http://reveng.sourceforge.net/crc-catalogue/
 */
static void InitCrc32(CRC_Type *base, uint32_t seed)
{
    crc_config_t config;

    config.polynomial         = 0x04C11DB7U;
    config.seed               = seed;
    config.reflectIn          = true;
    config.reflectOut         = true;
    config.complementChecksum = true;
    config.crcBits            = kCrcBits32;
    config.crcResult          = kCrcFinalChecksum;

    CRC_Init(base, &config);
}

uint8_t secureTransmit(SecureConnPtr connection, uint8_t* sendBuffer, uint8_t length){
	uint8_t key[16] = {0x00, 0x02, 0x09, 0x10, 0x1F, 0xF3, 0xFF, 0xBC,
					   0xCF, 0x25, 0x39, 0xAB, 0xB9, 0xC8, 0x79, 0xAC};
	uint8_t iv[16] =  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint32_t paddedMessageLength;
	uint32_t readyMessageLength;
	uint8_t paddedBuffer[512] = {0};
	CRC_Type *base = CRC0;
	uint32_t checksum;
	err_t err;

	struct AES_ctx AesCtx;

	PRINTF("Init Tranmission");

	AES_init_ctx_iv(&AesCtx, key, iv);

	PRINTF("\n\rVerify message padding...");
	paddedMessageLength = length + (16 - (length%16));
	memcpy(paddedBuffer, sendBuffer, length);
	PRINTF(" complete");

	PRINTF("\n\rStarting Encryption...");
	AES_CBC_encrypt_buffer(&AesCtx, paddedBuffer, paddedMessageLength);
	PRINTF(" complete");

	PRINTF("\n\rGenerating and embedding CRC32...");
	InitCrc32(base, 0xFFFFFFFFU);
	CRC_WriteData(base, (uint8_t*)&paddedBuffer[0], paddedMessageLength);
	checksum = CRC_Get32bitResult(base);
	paddedBuffer[paddedMessageLength] = checksum;
	paddedBuffer[paddedMessageLength+1] = checksum >> 8;
	paddedBuffer[paddedMessageLength+2] = checksum >> 16;
	paddedBuffer[paddedMessageLength+3] = checksum >> 24;
	readyMessageLength = paddedMessageLength + 4;
	PRINTF(" complete");

	err = netconn_write(connection, paddedBuffer, readyMessageLength, NETCONN_COPY);

	if(err != 0){
		PRINTF("\n\rAn error ocurred, message was not send");
		return 1;
	}

	PRINTF("\n\rMessage sent\n\r");
	return 0;

}

uint8_t secureReceive(SecureConnPtr connection, void* receiveBuffer, uint16_t* length){
	struct netbuf *buf;

	uint8_t key[16] = {0x00, 0x02, 0x09, 0x10, 0x1F, 0xF3, 0xFF, 0xBC,
					   0xCF, 0x25, 0x39, 0xAB, 0xB9, 0xC8, 0x79, 0xAC};
	uint8_t iv[16] =  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint32_t paddedMessageLength;
	uint32_t readyMessageLength;
	uint8_t paddedBuffer[512] = {0};
	CRC_Type *base = CRC0;
	uint32_t checksum, recvChecksum;
	err_t err;
	struct AES_ctx AesCtx;

	while ((err = netconn_recv(connection, &buf)) != ERR_OK) {}
	PRINTF("\n\rReceived message");
	do {
		 netbuf_data(buf, &receiveBuffer, length);
		 char* cdata = (char*)receiveBuffer;
	} while (netbuf_next(buf) >= 0);
	netbuf_delete(buf);

	uint8_t* receivedData = (uint8_t*)receiveBuffer;

	/* Receive Checksum */
	recvChecksum = (uint32_t)((receivedData[(*length)-4] << 0) |
							  (receivedData[(*length)-3] << 8) |
							  (receivedData[(*length)-2] << 16) |
							  (receivedData[(*length)-1] << 24));

	PRINTF("\n\rCRC32 check...");
	/* Calculate Checksum */
	InitCrc32(base, 0xFFFFFFFFU);
	CRC_WriteData(base, (uint8_t*)&receivedData[0], (*length)-4);
	checksum = CRC_Get32bitResult(base);

	if(checksum != recvChecksum){
		PRINTF("FAILED");
		return 1;
	}
	PRINTF("success");
	AES_init_ctx_iv(&AesCtx, key, iv);

	AES_CBC_decrypt_buffer(&AesCtx, (uint8_t*) &receiveBuffer[0], (*length)-4);
	AES_CBC_decrypt_buffer(&AesCtx, &receivedData[0], (*length)-4);
	PRINTF("\n\rDecrypted Information saved");

//	for(int i = 0; i < (*length)-4; i++){
//		paddedBuffer[i] = (*(uint8_t*)(receiveBuffer+i));
//	   PRINTF("%c", paddedBuffer[i]);
//	}

	return 0;
}

uint8_t deinitSecureConnection(SecureConnPtr connection){
	netconn_close(connection);
    netconn_delete(connection);
}






