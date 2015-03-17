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
    struct sockaddr_in server, client;
    struct hostent *serverInfo;
    char buffer[1024];
    
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
//    inet_aton("10.189.249.188", &server.sin_addr);
    bcopy((char*)serverInfo->h_addr, (char*)&server.sin_addr, serverInfo->h_length);

    //Get server length
    serverLength = sizeof(server);
    
    //printf("Enter a message:\n");
    
    //Zero out the buffer
    bzero(buffer, 1024);
    //fgets(buffer, 1023, stdin);
    char bChar[1000] = {'b'};
//	bzero(bChar, 1);
	//bChar[0] = 'b';
//	int k = 0;
//	for (k = 0; k < (640); k++) {
//		bChar[k] = 'b';
//	}
    //Send to server
	struct timeval start, end;
    double t1, t2;
    float averageRTT = 0;

	int l = 0;
    int count = 100;
	
    for (l = 0; l < count; l++) {
        if (gettimeofday(&start, NULL)) {
            printf("Error start time failed.\n");
        }
        
        
		returnValue = sendto(sock, bChar, strlen(bChar), 0, (struct sockaddr*)&server, serverLength);
        if (returnValue < 0) {
            printError("Error! Can't send to server\n");
        }
     
        //Receive from server
        returnValue = recvfrom(sock, bChar, strlen(bChar), 0, (struct sockaddr*)&server, &serverLength);
        if (returnValue < 0) {
            printError("Error! Can't receive from server\n");
        }
        
        if (gettimeofday(&end, NULL)) {
            printf("Error time end failed\n");
        }
    }
//    t1 += start.tv_sec + (start.tv_usec / 1000000.0);
//    t2 += end.tv_sec + (end.tv_usec / 1000000.0);
    t1 += start.tv_usec / 1000.0;
    t2 += end.tv_usec / 1000.0;
    
    averageRTT = (t2 - t1) / count;
    printf("time of the day:%g\n", averageRTT);
    
    write(1, "Received\n", 20);
    write(1, buffer, serverLength);

    close(sock);
    
    return 0;
}
