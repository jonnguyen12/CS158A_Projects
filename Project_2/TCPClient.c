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
#include <netinet/in.h>
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
    int socketFileDescriptor;
    int portNumber;
    int packet;
    struct sockaddr_in serverAddress;
    struct hostent *server; //Server info
    
    //Check if user does not provide hostname and port number
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    //Get port number and convert it from string to int
    portNumber = atoi(argv[2]);
    
    //Init socket
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketFileDescriptor < 0) {
        printError("Error! Can't open socket.\n");
    }
    
    //Get host name
    server  = gethostbyname(argv[1]);
    
    if (server == NULL) {
        fprintf(stderr, "Error! can't get host.\n");
        exit(0);
    }
    
    //Zero out the server address
    bzero((char*) &serverAddress, sizeof(serverAddress));
    
    //Set the server address properties
    serverAddress.sin_family = AF_INET;
//    bcopy((char*)server->h_addr, (char*)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);
    inet_aton("10.189.250.185", &serverAddress.sin_addr);
    
    //Connect to server
    if (connect(socketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        printError("Error! Can't connect");
    }
    
    //Send a message
    printf("Enter a message\n");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    
    //Write to server
    packet = write(socketFileDescriptor, buffer, strlen(buffer));
    
    if (packet < 0) {
        printError("Error! Can't write.\n");
        
    }
    
    //Read from server
    bzero(buffer, 256);
    packet = read(socketFileDescriptor, buffer, 255);
    
    if (packet < 0) {
        printError("Error! Can't read\n");
    }
    
    //Print out server message
    printf("%s\n", buffer);
    
    //Close the connection
    close(socketFileDescriptor);
    
    return 0;
}