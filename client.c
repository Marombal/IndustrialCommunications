#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024

int main(){

    char *ip = "127.0.0.1";
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;

    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("[-]Socket error\n");
        exit(1);
    }
    printf("[+]TCP server socket created\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    printf("Connected to the server.\n");
    printf("Enter a string: ");
    char str[MAX];

    scanf("%s", str); 
    
    bzero(buffer, 1024);
    strcpy(buffer, str);
    printf("Client says: %s \n", str);

    send(sock, buffer, strlen(buffer), 0);

    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server said: %s \n", buffer);


    close(sock);
    printf("Disconnected from the server. \n\n");

    return 0;
}