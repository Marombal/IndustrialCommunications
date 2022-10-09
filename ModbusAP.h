
#include <arpa/inet.h>	//inet_addr


int Read_h_regs(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val);

int Write_multiple_request(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val);