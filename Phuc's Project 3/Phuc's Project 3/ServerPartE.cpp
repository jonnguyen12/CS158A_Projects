//
//  ServerPartE.cpp
//  Phuc's Project 3
//
//  Created by Phuc Nguyen on 5/11/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>

using namespace std;

//Constants
#define USAGE "ether N"
#define MAX 2 // max of stations
const double SLOT_TIME = 0.8;   //slot time
#define SLOTS_COUNT 5000 // number of slots


//STATION
class station {
public:
    bool finished;
    int stationAverage;
    int finishCounter;
    int finishRank[100];
    void reset()
    {
        _NextAttempt = _CollisionCount = 0;
        finished = false;
    }
    
    bool transmit(int T) {return _NextAttempt = T;}
    void collide()
    {
        //updates station after a collision
        _CollisionCount++;
        _NextAttempt += 1 + backoff(_CollisionCount);
        //the 1 above is for the current slot
    }
private:
    int _NextAttempt;
    int _CollisionCount;
    static int backoff(int k) {
        //choose random number 0..2^k-1; 1e choose k random bits
        unsigned short r  = rand();
        unsigned short mask = 0xFFFF >> (16-k); //mask = 2^k-1
        return int (r & mask);
    }
};

station S[MAX];
int station1avg = 0;
int station2avg = 0;

//run does a single connection
// N = # of stations, index = index of each station
int run(int N, int index)
{
    int stationRank = 0;
    int time = SLOT_TIME;
    int i;
    int firstTime = 0;
    for (i = 0; i < N; i++) {
        S[i].reset();
    }
    
    while (1) {
        int count = 0; // number of attempts at this time
        int j = -1; //count the number of attempts, save j as index of one of them
        for (i = 0; i < N; i++) {
            if (S[i].transmit(time)) {
                j = i;
                ++count;
            }
        }
        
        if (count == 1) {
            //we are done
            stationRank++;
            bool isFinished = true;
            S[j].finished = true;
            
            for (i = 0; i < N; i++) {
                if (S[i].finished == false) {
                    isFinished = false;
                }
            }
            
            if (stationRank == 1) {
                station1avg += time;
                S[j].stationAverage += time;
                S[j].finishRank[S[j].finishCounter] = 1;
                S[j].finishCounter++;
                firstTime = time;
            }
            
            if (stationRank == 2)
            {
                station2avg += time;
                S[j].stationAverage += time;
                S[j].finishRank[S[j].finishCounter] = 2;
                S[j].finishCounter++;
            }
            if (isFinished) {
                return time;
            }
        }
        
        else if (count > 1) { //collision occurs
            for (i = 0; i < N; i++) {
                if (S[i].transmit(time))S[i].collide();
            }
            
        }
        ++time;
    }
}


//Print system error message if there is any
void printError(const char* message)
{
    perror(message);
    exit(1);
}




//Main
int main (int argc, char* argv[])
{
    
    //Properties
    int sock;
    int serverLength;
    int returnValue;
    int portNumber;
    socklen_t clientLength;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[2048];
    
    // Count time
    double slotTime = 0.8;
    int timeSlots = 20;
    double start = 0;
    double end = 0;
    int collisionCount = 0;
    int successfulPackets = 0;
    
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
    
    int count = 1;
    while (1) {
        
        
        start += 0.1;
        
        if (start >= 0.8) {
            start = 0;
            returnValue = sendto(sock,"1 Collision.\n", 17, 0, (struct sockaddr*)&client, clientLength);
            collisionCount++;
            continue;
        } else {
            //Receive from Client
            successfulPackets++;
            returnValue = recvfrom(sock, buffer, 2048, 0, (struct sockaddr*)&client, &clientLength);
            
            if (returnValue < 0) {
                printError("Error! Can't receive from client.\n");
            }
            printf("Packet #%d received: %s\n", count++, buffer);
            
            //Send to client
            returnValue = sendto(sock,"Packet received.\n", 17, 0, (struct sockaddr*)&client, clientLength);
            
            if (returnValue < 0) {
                printError("Error! Can't send to client");
            }
//            char * success_msg;
//            sprintf(success_msg, "Successful packets = %d.\n", successfulPackets);
//            if (sendto(sock,&success_msg, 50, 0, (struct sockaddr*)&client, clientLength) < 0)
//            {
//                puts("Error! at line 221");
//            }

        }
        //Clear the buffer
        bzero(buffer, strlen(buffer));
        
        printf("Collision counts = %d\n", collisionCount);
        printf("Successful packets = %d.\n", successfulPackets);
        
    }
    return 0;
}

