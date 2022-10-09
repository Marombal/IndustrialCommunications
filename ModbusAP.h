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
#define min_number_of_registers 1
#define max_quantity_of_registers_write 123
#define max_quantity_of_registers_read 125
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
 *  uint32_t st_r - starting address of the holding registers considering the registers from 1 to 65536/register number not offset.
 *  uint32_t n_r - number of holding registers to be read from the server;
 *  uint16_t val - vector were the returning values will be stored.
 * 
 *  Example of usage. Read two registers starting from the 5th register (4offset) from the server with ip 127.0.0.1 listening on port 502 and saving them on vector val 
 *  
 *  Read_h_regs("127.0.0.1", 502, 5, 2, val); // being val a uint16_t vector
 *     
 */
int Read_h_regs(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val);

/* Write_multiple_regs()
 * Writes in servers' registers
 * Args:
 *  char *server_add - server's address ip in formtar of a string (e.g. "127.0.0.1");
 *  unsigned int port - TCP port for the connection (e.g. 502);
 *  uint32_t st_r - starting address considering registers from 1 to 65536/register number not offset.
 *  uint32_t n_r - number of registers to be write on the server;
 *  uint16_t val - vector were the values to be written to the server should be.
 * 
 *  Example of usage. Write three registers (present on the vector val) starting from the 2th register (1offset) to the server with ip 127.0.0.1 listening on port 502 
 *  
 *  Write_multiple_regs("127.0.0.1", 502, 2, 3, val); // being val a uint16_t vector with the values that you wanna write stored
 *     
 */
int Write_multiple_regs(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val);

#endif