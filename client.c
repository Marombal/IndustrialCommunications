#include <stdio.h>

#include "ModbusAP.h"

int main(void){
    
    int nread;
    uint16_t read[3];
    nread = Read_h_regs("127.0.0.1", 502, 1, 2, read);
    if(nread < 0){
        if(nread == -10) printf("Function not suported \n");
        else if(nread == -30) printf("Quantity of registers not suported \n");
        else if(nread == -20) printf("Adress not suported \n");
        else if(nread == -40) printf("Request process failed \n");
        else printf("RHR error \n");
    }
    else
	    printf("Read -> (%d) %02x %02x %02x\n", nread, read[0], read[1], read[2]);

    uint16_t read2;
    nread = Read_h_regs("127.0.0.1", 502, 1, 1, &read2);
    if(nread < 0){
        if(nread == -10) printf("Function not suported \n");
        else if(nread == -30) printf("Quantity of registers not suported \n");
        else if(nread == -20) printf("Adress not suported \n");
        else if(nread == -40) printf("Request process failed \n");
        else printf("RHR error \n");
    }
    else
	    printf("Read -> (%d) %02x \n", nread, read2);


    int nwrite;
    uint16_t write[3] = {11, 1, 3};
    nwrite = Write_multiple_regs("127.0.0.1", 502, 5, 3, write);
    if(nwrite < 0){
        if(nwrite == -10) printf("Function not suported \n");
        else if(nwrite == -30) printf("Quantity of registers not suported \n");
        else if(nwrite == -20) printf("Adress not suported \n");
        else if(nwrite == -40) printf("Request process failed \n");
        else printf("WMR error \n");
    }
    else   
        printf("Write -> (%d)\n", nwrite);

    uint16_t w1 = (read[0] + read[1]);
    nwrite = Write_multiple_regs("127.0.0.1", 502, 9, 1, &w1);
    if(nwrite < 0){
        if(nwrite == -10) printf("Function not suported \n");
        else if(nwrite == -30) printf("Quantity of registers not suported \n");
        else if(nwrite == -20) printf("Adress not suported \n");
        else if(nwrite == -40) printf("Request process failed \n");
        else printf("WMR error \n");
    }
    else   
        printf("Write -> (%d)\n", nwrite);

}