//
//  client.c
//  CS158A_Project2
//
//  Created by Phuc Nguyen on 3/10/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//Print the system error if there is any
void printError (char * message)
{
    perror(message);
    exit(1);
}


int main(int argc, char* argv[])
{
    //Properties
    int sock;
    int returnValue;
    int portNumber;
    socklen_t serverLength;
    struct sockaddr_in server, client;
    struct hostent *serverInfo;
    char buffer[1024];
    
    if (argc != 3) {
        fprintf(stderr, "Error! must provide host name and port number\n");
        exit(1);
    }
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printError("Error! Can't create socket.\n");
    }
    
    portNumber = atoi(argv[2]);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(portNumber);
    
    bcopy((char*)serverInfo->h_addr, (char*)&server.sin_addr, serverInfo->h_length);

    serverLength = sizeof(server);
    
    printf("Enter a message:\n");
    
    bzero(buffer, 1024);
    fgets(buffer, 1023, stdin);
    
    returnValue = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server, serverLength);
    
    if (returnValue < 0) {
        printError("Error! Can't send to server\n");
    }
    
    returnValue = recvfrom(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server, &serverLength);
    
    if (returnValue < 0) {
        printError("Error! Can't receive from server\n");
    }
    
    write(1, "Received", 20);
    write(1, buffer, serverLength);

    close(sock);
    
    return 0;
}