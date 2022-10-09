#ifndef _MODBUSTCP_H
#define _MODBUSTCP_H

#include <stdio.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>	
#include <stdlib.h>


#define Unit_identifier_code 0x01
#define PDU_max_size 263 // 252 (APDU) + 7 + extra

#define SERVER_PORT 502
#define SERVER_ADDR "127.0.0.1"

#define STATE_START 0
#define STATE_ERROR -1
#define STATE_RESPONSE 1
#define STATE_EXCEPTION 4
#define STATE_OK 2




int Send_Modbus_request(char *server_add, unsigned int port, uint8_t *APDU, unsigned int APDUlen, uint8_t *APDU_R);

#endif