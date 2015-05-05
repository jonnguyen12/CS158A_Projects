//
//  main.c
//  CS158A_Project2
//
//  Created by Phuc Nguyen on 3/9/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>

//Print system error message if there is any
void printError(const char* message)
{
    perror(message);
    exit(1);
}

int main(int argc, char* argv[])
{
    //Properties
    int sock;
    int serverLength;
    int returnValue;
    int portNumber;
    socklen_t clientLength;
    struct sockaddr_in server;
    struct sockaddr_in client;
    
    int bytes_received = 0;
    
    char buffer[17000];
    
    if (argc < 2) {
        fprintf(stderr, "Error! Must provide port number.\n");
        exit(1);
    }
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock < 0) {
        printError("Error! Cant't open socket.\n");
    }
    
    serverLength = sizeof(server);
    clientLength = sizeof(client);
    
    
    bzero(&server, serverLength);
    portNumber = atoi(argv[1]);
    
    server.sin_family = AF_INET;
    //	inet_aton("10.189.249.188", server.sin_addr);
    // server.sin_addr.s_addr = "10.189.249.188";
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portNumber);
    
    if (bind(sock, (struct sockaddr *) &server, serverLength) < 0) {
        printError("Error! Can't bind.\n");
    }
    
    while (1) {
        returnValue = recvfrom(sock, buffer, 17000, 0, (struct sockaddr*)&client, &clientLength);
        
        if (returnValue < 0) {
            printError("Error! CAn't receive from client.\n");
        }
        
        bytes_received += returnValue;
        // write(1, "Datagram is received\n", 30);
        write(1, buffer, returnValue);
        
        returnValue = sendto(sock, "Message is received", 20, 0, (struct sockaddr*)&client, clientLength);
        
        if (returnValue < 0) {
            printError("Error! Can't send to client");
        }
        printf("bytes received = %d", bytes_received);
        
    }
    
    
    return 0;
    
}
