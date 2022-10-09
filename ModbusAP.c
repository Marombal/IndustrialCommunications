#include "ModbusAP.h"

void print_ADPU(uint8_t *buffer, unsigned int size){
    printf("ADPU: ");
    for(int i = 0; i < size; i++) printf("%02X ", buffer[i]);
    printf("\n");
}

void print_val(uint16_t *buffer, unsigned int size){
    printf("val: ");
    for(int i = 0; i < size; i++){
         printf("%02X %02X ", (buffer[i] >> 8), (buffer[i] & 0xFF));}
    printf("\n");
}

int Read_h_regs(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val){
    // Check Parameters
    if((st_r > max_number_of_registers) || (st_r < 0)){
        printf("[-] Bad parameters (st_r)\n");
        return -1;
    }
    if((n_r > max_number_of_registers) || (n_r < 0)){
        printf("[-] Bad parameters (n_r)\n");
        return -1;
    }
    if(n_r + st_r > max_number_of_registers){
        printf("[-] Bad parameters (st_r + n_r)\n");
        return -1;
    }
    if(!val){
        //printf("[-] Bad parameters (val)\n");
        //return -1;
    }
    if(port < 0){
        printf("[-] Bad parameters (port)\n");
        return -1;
    }

    // Assembles APDU (MODBUS PDU)
    uint8_t ADPU[ADPU_max_size];
    uint8_t Func_code, HI_byte_st, LO_byte_st, HI_byte_quantity, LO_byte_quantity; // 5 bytes overhead

    Func_code = Read_holding_registers_function_code;
    HI_byte_st = (st_r - 1) >> 8; // 0xF12E -> 0x00F1
    LO_byte_st = (st_r - 1) & 0xFF; // 0xF12E & 0x00FF -> 0x00E2
    HI_byte_quantity = n_r >> 8;
    LO_byte_quantity = n_r & 0xFF;

    ADPU[0] = Func_code;
    ADPU[1] = HI_byte_st;
    ADPU[2] = LO_byte_st;
    ADPU[3] = HI_byte_quantity;
    ADPU[4] = LO_byte_quantity;

    unsigned int ADPU_size = 5;
    uint8_t ADPU_R[ADPU_max_size];

    //print_ADPU(ADPU, ADPU_size); //Debug

    // Send_Modbus_request()
    int res = Send_Modbus_request(server_add, port, ADPU, ADPU_size, ADPU_R);

    // checks the reponse (APDU_R or error_code)
    if(res < 0){
        printf("[-] Send_Modbus_request() call failed\n");
        return res;
    }

    unsigned int STATE = STATE_START, Exception_code, ADPU_R_byte_count, read_registers = 0;

    while(STATE != STATE_ERROR){
        switch (STATE)
        {
        case (STATE_START):
            if(ADPU_R[0] == Read_holding_registers_function_code) STATE = STATE_RESPONSE;
            else if(ADPU_R[0] == Read_holding_registers_rejection_code) STATE = STATE_EXCEPTION;
            else STATE = STATE_ERROR;
            break;
        case (STATE_RESPONSE):
            ADPU_R_byte_count = ADPU_R[1];
            for(int i = 0; i < ADPU_R[1]/2; i++){
                val[i] = (uint16_t) (ADPU_R[2*i+2] << 8) + (uint16_t) (ADPU_R[3+2*i]);
                read_registers++;
            }
            return read_registers;
            break;
        case (STATE_EXCEPTION):
            Exception_code = ADPU_R[1];
            return -10* Exception_code;
            break;
        default:
            STATE = STATE_ERROR;
            break;
        }
    }
    // returns: number of read registers - ok, <0 - Error

    return read_registers;
}

int Write_multiple_regs(char *server_add, unsigned int port, uint32_t st_r, uint16_t n_r, uint16_t *val){
    // Check Parameters
    // port
    if((st_r > max_number_of_registers) || (st_r < 0)){
        printf("[-] Bad parameters (st_r)\n");
        return -1;
    }
    if((n_r > 123) || (n_r < 0)){
        printf("[-] Bad parameters (n_r)\n");
        return -1;
    }
    if(n_r + st_r > max_number_of_registers){
        printf("[-] Bad parameters (st_r + n_r)\n");
        return -1;
    }
    if(!val){
        //printf("[-] Bad parameters (val)\n");
        //return -1;
    }
    
    // Assembles ADPU (Modbus PDU)
    uint8_t ADPU[ADPU_max_size];
    uint8_t Func_code, HI_byte_st, LO_byte_st, HI_byte_quantity, LO_byte_quantity, Byte_count, HI_reg, LO_Reg;

    Func_code = Write_multiple_request_function_code;
    HI_byte_st = (st_r - 1) >> 8;
    LO_byte_st = (st_r - 1) & 0xFF;
    HI_byte_quantity = n_r >> 8;
    LO_byte_quantity = n_r & 0xFF;
    Byte_count = 2 * n_r;


    ADPU[0] = Func_code;
    ADPU[1] = HI_byte_st;
    ADPU[2] = LO_byte_st;
    ADPU[3] = HI_byte_quantity;
    ADPU[4] = LO_byte_quantity;
    ADPU[5] = Byte_count;

    for(int i = 0; i < n_r; i++){ //2 * n_r
        ADPU[2*i + 6] = (uint8_t) (val[i] >> 8);
        ADPU[2*i+1 + 6] = (uint8_t) (val[i] & 0xFF);
    }
        
    unsigned int ADPU_size = (6 + 2 * n_r);
    uint8_t ADPU_R[ADPU_max_size];

    //Debug
    //print_val(val, (2*n_r));
    //print_val(val, (n_r));
    //print_ADPU(ADPU, (6 + 2*n_r));

    // Send_Modbus_request()
    int res = Send_Modbus_request(server_add, port, ADPU, ADPU_size, ADPU_R);

    // Checks the response (APDU_R or error_code)
    if(res < 0){
        printf("[-] Send_Modbus_request() call failed\n");
        return res;
    }

    int STATE = STATE_START, Exception_code, ADPU_R_byte_count, write_registers = 0, HI_starting_address, LO_starting_address, HI_quantity_of_registers, LO_quantity_of_registers;
    
    while(STATE != STATE_ERROR){
        switch (STATE)
        {
        case (STATE_START):
            if(ADPU_R[0] == Write_multiple_request_function_code) STATE = STATE_RESPONSE;
            else if(ADPU_R[0] == Write_multiple_request_rejection_code) STATE = STATE_EXCEPTION; 
            else STATE = STATE_ERROR;
            break;
        case (STATE_RESPONSE):
            HI_starting_address = ADPU_R[1];
            LO_starting_address = ADPU_R[2];
            HI_quantity_of_registers = ADPU_R[3];
            LO_quantity_of_registers = ADPU_R[4];
            write_registers = (int) (ADPU_R[3] << 8) + (int) (ADPU_R[4]);
            return write_registers;
            break;
        case (STATE_EXCEPTION):
            Exception_code = ADPU_R[1];
            return -10* Exception_code;
            break;
        default:
            STATE = STATE_ERROR;
            break;
        }
    }

    // Return: number of written register - ok ; <0 - Error
    return write_registers;
}
