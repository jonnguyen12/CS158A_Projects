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

//Print system error message if there is any
void printError(const char* message)
{
    perror(message);
    exit(1);
}

int main(int argc, char* argv[])
{
    //Properties
    int socketFileDescriptor;
    int newSocketFileDescriptor;
    int portNumber;
    socklen_t clientLength;
    char buffer[256];
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int packet;
    
    //Check if user does not provide port number
    if (argc < 2) {
        fprintf(stderr, "Error! Must provide port number.\n");
        exit(1);
    }
    
    //Initialize the socket with DGRAM
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketFileDescriptor < 0) {
        printError("Error! can't open socket.\n");
    }
    
    //Zero out the server address
    bzero((char*) &serverAddress, sizeof(serverAddress));
    
    //Get port number and convert it from string to int
    portNumber = atoi(argv[1]);
    
    //Set serverAddress properties
    serverAddress.sin_family = AF_INET; //AF_INET is required
    serverAddress.sin_addr.s_addr = INADDR_ANY; //s_addr contains host IP address. INADDR_ANY can get this address.
    serverAddress.sin_port = htons(portNumber); //htons convert port number to Network Byte Order
    
    //Bind
    if (bind(socketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        printError("Error! can't bind.\n");
    }
    
    //Listen
    listen(socketFileDescriptor, 5); //5 is the maximum number of connections that can wait
    
    clientLength = sizeof(clientAddress); //get the size of client address
    
    //Accept
    newSocketFileDescriptor = accept(socketFileDescriptor, (struct sockaddr*) &clientAddress, &clientLength);
    
    if (newSocketFileDescriptor < 0) {
        printError("Error! Can't accept\n");
    }
    
    //Zero out the buffer
    bzero(buffer, 256);
    
    //Read from the client
    packet = read(newSocketFileDescriptor, buffer, 200);
    
    if (packet < 0) {
        printError("Error! Can't read\n,");
    }
    
    printf("Message: %s\n", buffer);
    
    //Write to the client
    packet = write(newSocketFileDescriptor, "Message received.\n", 20);
    
    if (packet < 0) {
        printError("Error! Can't write.\n");
    }
    
    
    //Close the connection
    close(newSocketFileDescriptor);
    close(socketFileDescriptor);
    return 0;
    
    
}
