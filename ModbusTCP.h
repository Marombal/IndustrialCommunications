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


/* Send_Modbus_request()
 * This function is responsible from preparion the MBAP header, create a socket and send (from that socket) the request to the server
 * Args:
 *  char *server_add - server's address ip in formtar of a string (e.g. "127.0.0.1");
 *  unsigned int port - TCP port for the connection (e.g. 502);
 *  uint8_t *APDU - The array with the request to be send
 *  unsigned int APDUlen - length of the previus array
 *  uint8_t *APDU_R - stream where the server's response will be stored(value returned by the argument).
 */
int Send_Modbus_request(char *server_add, unsigned int port, uint8_t *APDU, unsigned int APDUlen, uint8_t *APDU_R);

#endif