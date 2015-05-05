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
    //    serverInfo->h_addr = "10.189.249.188";
    inet_aton("10.189.251.219", &server.sin_addr);
    //    bcopy((char*)serverInfo->h_addr, (char*)&server.sin_addr, serverInfo->h_length);
    
    //Get server length
    serverLength = sizeof(server);
    
    //printf("Enter a message:\n");
    
    //Zero out the buffer
    bzero(buffer, 1024);
    //fgets(buffer, 1023, stdin);
    int messageSize = 1000;
    int count = 100;
    
    char bChar[messageSize];
    
    for (int i = 0; i < messageSize; i++) {
        bChar[i] = 'A';
    }
    
    //Send to server
    struct timeval start, end;
    double t1, t2;
    float averageRTT = 0;
    
    int l = 0;
    int bytes_sent = 0;
    int sizeOfPacket = 0;
    double time = 0;
    int lambda;
    
    for (lambda = N; lambda >= 4; lambda -= 2) {
        
        for (l = 0; l < count; l++) {
            if (gettimeofday(&start, NULL)) {
                printf("Error start time failed.\n");
            }
            
            //Send packet to server each slot time
            time = run(lambda);
            printf("time = %.5f", time);
            
            returnValue = sendto(sock, bChar, strlen(bChar), 0, (struct sockaddr*)&server, serverLength);
            if (returnValue < 0) {
                printError("Error! Can't send to server\n");
            }
            bytes_sent += returnValue;
            
            
            //Receive from server
            returnValue = recvfrom(sock, bChar, strlen(bChar), 0, (struct sockaddr*)&server, &serverLength);
            if (returnValue < 0) {
                puts("Error! Can't receive from server\n");
                do {
                    puts("Resending...");
                    time = run(lambda);
                    printf("time = %.5f\n", time);
                    returnValue = sendto(sock, bChar, strlen(bChar), 0, (struct sockaddr*)&server, serverLength);
                } while (returnValue < 0);
            } else {
                puts("package sent");
            }
            
            
            sizeOfPacket += sizeof(bChar);
            
            if (gettimeofday(&end, NULL)) {
                printf("Error time end failed\n");
            }
            
            t1 = start.tv_usec;
            t2 = end.tv_usec;
            averageRTT += t2 - t1;
        }
    }
    
    //    t1 += start.tv_sec + (start.tv_usec / 1000000.0);
    //    t2 += end.tv_sec + (end.tv_usec / 1000000.0);
    
    
    averageRTT = averageRTT / count;
    printf("time of the day:%g\n", averageRTT);
    printf("size of packet = %d\n", sizeOfPacket);
    printf("bytes sent = %d\n", bytes_sent);
    printf("packet loss = %d\n", sizeOfPacket - bytes_sent);
    printf("%s\n",bChar);
    
    write(1, "Received\n", 20);
    write(1, &bChar, serverLength);
    
    close(sock);
    
    return 0;
}
