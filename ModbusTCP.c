#include "ModbusTCP.h"

void print_PDU(char *buffer, int size){
    printf("PDU: ");
    for(int i = 0; i < size; i++) printf("%02X ", buffer[i]);
    printf("\n");
}

/**
 * MBAP
 * Transition Identifier 2 Bytes -> Generated on the function Send_Modbus_request
 * Protocol Identifier 2 Bytes -> 0x00 for the current version
 * Lenght 2Bytes -> Number of following bytes
 * Unit Identifier 1 Byte -> Identification of a remote slave connected on a serial line or on other buses. Initualized by a client. Recopied by the server from the received request
 * 
 */

int ID = 0;

int Send_Modbus_request(char *server_add, unsigned int port, uint8_t *APDU, unsigned int APDUlen, uint8_t *APDU_R){
    // Generates TI (trans.ID -> sequence number)
    ID = ID + 1;
    int TI = ID; 

    // Assembles PDU = APDU(SDU) + MBAP
    unsigned int PDUlen = 0;
    uint8_t PDU[PDU_max_size];
    uint8_t HI_Transaction_identifier, LO_Transaction_identifier, HI_Protocol_identifier, LO_Protocol_identifier, HI_Lenght, LO_Lenght, Unit_identifier;

    HI_Transaction_identifier = TI >> 8;
    LO_Transaction_identifier = TI & 0xFF;
    HI_Protocol_identifier = 0x00; LO_Protocol_identifier = 0x00;
    HI_Lenght = (APDUlen + 1) >> 8;                                     // +1 from Unit Identifier
    LO_Lenght = (APDUlen + 1) & 0xFF;                                   // +1 from Unit Identifier
    Unit_identifier = Unit_identifier_code;

    PDU[0] = HI_Transaction_identifier;
    PDU[1] = LO_Transaction_identifier;
    PDU[2] = HI_Protocol_identifier;
    PDU[3] = LO_Protocol_identifier;
    PDU[4] = HI_Lenght;
    PDU[5] = LO_Lenght;
    PDU[6] = Unit_identifier;

    for(int i = 0; i < APDUlen; i++)
        PDU[i + 7] = APDU[i];

    PDUlen = 7 + APDUlen;

    //Debug
    //print_PDU(PDU, PDUlen);

    // Opens TCP client socket and connects to server (fd = socket(); sockaddr_in ... ; connect(fd, s_addr...))
    int socket_desc, out, in;
    struct sockaddr_in server;
    char *out_buf, in_buf[PDU_max_size];

    //create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_desc < 0){
        printf("[-] Socket creation failed! \n");
        return -1;
    }
    //printf("[+] Socket Created with sucess \n"); //Debug

    // preprare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_add);
    server.sin_port = htons(port);

    //connect to remote server
    if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        printf("[-] Connection to the server failed. \n");
        return -1;
    }
    //printf("[+] Connected to the server at address (%s) / port (%d). \n", server_add, port); //Debug

    // sends Modbus TCP PDU
    //in = write(socket_desc, PDU, PDUlen);
    in = send(socket_desc, PDU, PDUlen, 0);
    if(in < 0){
        printf("[-] Error sending to socket. \n");
    }
    // else  printf("[+] Sent %d bytes to socket. \n", in); //debug
       
    
    uint8_t PDU_R[PDU_max_size];
    unsigned int PDU_Rlen = 0;
    // response o timeout
    out = recv(socket_desc, PDU_R, PDU_max_size, 0);
    //read(socket_desc, PDU_R, PDU_Rlen);
    if(out < 0){
        printf("[-] RECV failed. \n");
    }
    //else printf("[+] Server answered. \n"); //debug
        
    // If response, remove MBAP, PDU_R -> APDU_R

    // PDU_R analisys
    if(PDU_R[0] != HI_Transaction_identifier || PDU_R[1] != LO_Transaction_identifier){
        printf("[-] Incorrect Transition Identifier. PDU_R rejected. \n");
        close(socket_desc);
        return -1;
    }
    if(PDU_R[2] != HI_Protocol_identifier || PDU_R[3] != LO_Protocol_identifier){
        printf("[-] Incorrect Protocol Identifier. PDU_R rejected. \n");
        close(socket_desc);
        return -1;
    }
    if(PDU_R[6] != Unit_identifier){
        printf("[-] Incorrect Unit Identifier. PDU_R rejected. \n");
        close(socket_desc);
        return -1;
    }

    //printf("[+] MBAP okay. \n"); //debug
    unsigned int lenght = (PDU_R[4] << 8) | PDU_R[5];
    for(int i = 0; i < lenght - 1; i++){
        APDU_R[i] = PDU_R[i+7]; 
        PDU_Rlen++;                             // PDU_Rlen = lenght
    }

    // Closes TCP client Socket with server (close(fd))
    close(socket_desc);
    // Returns: APDU_R and 0 - ok, < 0 - Error (timeout)
    return 0;
}

