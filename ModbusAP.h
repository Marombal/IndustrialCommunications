#ifndef _MODBUSAP_H
#define _MODBUSAP_H

#include "ModbusTCP.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>	
#include <stdlib.h>

#define max_number_of_registers 65536
#define max_quantity_of_registers 123
#define ADPU_max_size 256 // 123*2 + 6 + extra
#define Write_multiple_request_function_code 0x10
#define Write_multiple_request_rejection_code 0x90
#define Read_holding_registers_function_code 0x03
#define Read_holding_registers_rejection_code 0x83

#define STATE_START 0
#define STATE_ERROR -1
#define STATE_RESPONSE 1
#define STATE_EXCEPTION 4
#define STATE_OK 2


/* Read_h_regs()
 * Reads the registers values from the server
 * Args:
 *  char *server_add - server's address ip in formtar of a string (e.g. "127.0.0.1");
 *  unsigned int port - TCP port for the connection (e.g. 502);
 *  - st_r = start address of the holding registers - 1, because of uint16_t that can only
 *      store values from 0 to 65535;
 *  - n_r - number of holding registers to be read by the server;
 *  - val - vector that will be returned with the pretended values. For reference, the
 *      length of the vector is n_r.
 */
int Read_h_regs(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val);

int Write_multiple_regs(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val);

#endif