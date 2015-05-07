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
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

const int RUNCOUNT = 5;
const int N = 20;

using namespace std;

//Properties
int sock;
int returnValue;
int portNumber;
unsigned int serverLength;
struct sockaddr_in server, client;
struct hostent *serverInfo;
char buffer[1024];



// X = X(lambda) is our random variable
double X(double lambda) {
    double u;
    do {
        u = double(rand()) / RAND_MAX;
    } while (u == 0);
    double val = -log(u) * lambda;
    return val;
    
}

double run(double lambda)
{
    int i = 0;
    double time = 0.8;
    double prevtime = -1;
    double nexttime = 0;
    time = X(lambda);
    nexttime = time + X(lambda);
    
    //while collision: adjacent times within +/- 1 slot
    while (time - prevtime < 1 || nexttime - time < 1) {
        prevtime = time;
        time = nexttime;
        nexttime += X(lambda);
        
    }
    
    return time;
    
}

void runLambda() {
    int i;
    double sum, lambda;
    double min, currentValue, max;
    for (lambda = N; lambda >= 2; lambda -= 2) {
        sum = 0;
        min = 0;
        max = 0;
        currentValue = 0;
        for (i = 0; i < RUNCOUNT; i++) {
            currentValue = run(lambda);
            
            if (i == 0) {
                min = currentValue;
                max = currentValue;
            }
            
            sum += currentValue;
            
            if (currentValue < min) {
                min = currentValue;
            }
            
            if (currentValue > max) {
                max = currentValue;
            }
            printf("%.3f : %.3f\n", lambda, sum/RUNCOUNT);
        }
        
        printf("%.3f : miminum value = %.3f\n", lambda, min);
        printf("%.3f : maximum value = %.3f\n", lambda, max);
        
    }
}



//Print the system error if there is any
void printError (char * message)
{
    perror(message);
    exit(1);
}


int main(int argc, char* argv[])
{
    
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
    inet_aton("10.189.250.7", &server.sin_addr);
    
    //Get server length
    serverLength = sizeof(server);
    
    //Zero out the buffer
    bzero(buffer, 1024);
    
    //Creating 1KB Packet
    int messageSize = 1000;
    int count = 100;
    
    char bChar[messageSize];
    
    for (int i = 0; i < messageSize; i++) {
        bChar[i] = 'P';
    }
    
    //Variables for lambda
    double time = 0; // time for each packet to be sent
    int lambda;
    
    // N = 20, 18, 16...4
    for (lambda = N; lambda >= 4; lambda -= 2) {
        for (int i = 0; i < RUNCOUNT; i++) {
            
            //Send packet to server each slot time
            time = run(lambda);
            printf("\nTime to run a packet = %.5f\n", time);
            
            //Send to server
            returnValue = sendto(sock, bChar, strlen(bChar), 0, (struct sockaddr*)&server, serverLength);
            if (returnValue < 0) {
                printError("Error! Can't receive from server.\n");
            }
            
            //Receive from server
            returnValue = recvfrom(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server, &serverLength);
            if (returnValue < 0) {
                printError("Can't receive from server.\n");
            }
            
            printf("Server sent: %s\n", buffer);
            
            // 0 = success, 1 = collission
            if (buffer[0] == '0') {
                puts("Success");
            } else if (buffer[0] == '1')
            {
                puts("Server has collisions");
                do {
                    bzero(buffer, strlen(buffer));
                    
                    puts("Resending...");
                    time = run(lambda);
                    printf("time = %.5f\n", time);
                    sendto(sock, bChar, strlen(bChar), 0, (struct sockaddr*)&server, serverLength);
                    returnValue = recvfrom(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server, &serverLength);
                    if (returnValue < 0) {
                        printf("Can't receive from server. Error at line %d", __LINE__ );
                    }
                } while (buffer[0] == '1');
            }
            
            //clear the buffer
            bzero(buffer, strlen(buffer));
            
        } //End for loop for RUNCOUNT
        
    } //End for loop for each lambda
    
    close(sock);
    
    return 0;
}
