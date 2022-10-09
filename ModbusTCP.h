#include <stdio.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write
#include <stdlib.h>


int Send_Modbus_request(char *server_add, unsigned int port, uint8_t *APDU, unsigned int APDUlen, uint8_t *APDU_R);