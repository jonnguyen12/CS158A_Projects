//
//  Network.cpp
//  Phuc's Project 3
//
//  Created by Phuc Nguyen on 5/11/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Node.cpp"

using namespace std;

//Global static variables
static int indexOfMin = 0;
static vector<double> xVal;
static Node node;
static int numOfStations = 20;
static int MAX_TIME_SLOT = 20000;
static double min = 0.0;
static Packet minPacket, collidedPacket;
static int numOfSuccessfulPackets;
static int timeSuccessful;
static vector<Node> nodes;
static vector<Packet> packets;
static vector<Packet> collidedPackets;
double currentTime = 0.0;

struct {
    bool operator()(int a, int b)
    {
        return a < b;
    }
} customLess;

class Network
{
public:
    static void generatePackets(double anotherLambda)
    {
        for (int i = 0; i < numOfStations; i++) {
            node =  Node(i, anotherLambda);
            nodes.push_back(node);
            nodes.at(i).send(anotherLambda);
        }
    }
    
    static void simulate(double lambda)
    {
        numOfSuccessfulPackets = 0;
        timeSuccessful = 0;
        nodes.clear();
        packets.clear();
        
        double difference;
        generatePackets(lambda);
        packets = getSortedPacketList();
        minPacket = packets.at(0);
        
        while (currentTime <= MAX_TIME_SLOT) {
            bool noCollision = true;
            
            for (int j = 0; j < numOfStations; j++) {
                difference = packets.at(j).getContentionInterval() - minPacket.getContentionInterval();
                
                if (difference <= 1 && j != minPacket.getStationName()) {
                    noCollision = false;
                    collidedPacket = packets.at(j);
                    break;
                }
            } //end for loop
            
            if (noCollision) {
                currentTime += 8;
                numOfSuccessfulPackets++;
                packets.at(0).setStationName(currentTime);
            }
            else {
                currentTime++;
                minPacket.incrementCollision();
                collidedPacket.incrementCollision();
                
                nodes.at(minPacket.getStationName()).backoff(minPacket.getCollisionCount(), minPacket.getContentionInterval());
                nodes.at(collidedPacket.getStationName()).backoff(collidedPacket.getCollisionCount(), collidedPacket.getContentionInterval());
                
                packets = getSortedPacketList();
                minPacket = packets.at(0);
            }
            
            generatePackets(lambda);
        }
        
    }
    
    static vector<Packet> getSortedPacketList()
    {
        vector<Packet> sortedList;
        for (auto &n : nodes)
        {
            sortedList.push_back(n.getNextPacketToBeTransmitted());
            
        }
//        sort(sortedList.begin(), sortedList.end());
        return sortedList;
    }
    
};


int main()
{
    return 0;
}