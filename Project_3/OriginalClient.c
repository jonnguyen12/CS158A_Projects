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
#include <sys/time.h>

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
    unsigned int serverLength;
    struct sockaddr_in server, from;
    struct hostent *serverInfo;
    char buffer[1024];
    const int PACKET_SIZE = 1000;
    char packet[PACKET_SIZE];
    const int RUNCOUNT = 5000;
    
    //Check if the user does not provide host name and port number
    if (argc != 3) {
        fprintf(stderr, "Error! must provide host name and port number\n");
        exit(1);
    }
    
    //Init a socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printError("Error! Can't create socket.\n");
    }
    
    //Get port number
    portNumber = atoi(argv[2]);
    
    //Get host
    serverInfo = gethostbyname(argv[1]);
    
    //Set server properties
    server.sin_family = AF_INET;
    server.sin_port = htons(portNumber);
    //    serverInfo->h_addr = "10.189.249.188";
//    inet_aton("24.6.129.25", &server.sin_addr);
    bcopy((char*)serverInfo->h_addr, (char*)&server.sin_addr, serverInfo->h_length);
    
    //Get server length
    serverLength = sizeof(struct sockaddr_in);
    
    //printf("Enter a message:\n");
    
    //Zero out the buffer
    bzero(buffer, 1024);
    
    for (int i = 0; i < 1000; i++) {
        packet[i] = 'P';
    }
    
    int count = 100;
    
    for (int i = 0; i < RUNCOUNT; i++) {
        
        //Send a packet to server
        returnValue = sendto(sock, packet, PACKET_SIZE, 0, (struct sockaddr*)&server, serverLength);
        
        if (returnValue < 0) {
            printError("Error! Can't send to server\n");
        }
        
        printf("Sent packet #%d: %s.\n", i + 1, packet);
        puts("------------------\n\n");
        
        //Receive from server
        returnValue = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&from, &serverLength);
        if (returnValue < 0) {
            printError("Error! Can't receive from server\n");
        }
        printf("Server's message:");
        puts(buffer);
        bzero(buffer, strlen(buffer));
        
    }
    
    
    //Close socket
    close(sock);
    
    return 0;
}
