//
//  Node.cpp
//  Phuc's Project 3
//
//  Created by Phuc Nguyen on 5/11/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <queue>
#include "Packet.cpp"
#include <math.h>

using namespace std;

class Node {

private:
    double lamda;
    double time;
    double delayValue;
    bool collided;
    int randomNumber;
    int name;
    queue<Packet> packetQueue;
public:
    Node();
    Node(int newName, double newLambda) {
        name = newName;
        lamda = newLambda;
        time = 0.0;
        collided = false;
    }
    
    double backoff (int numberOfTimesCollided, double currentTime)
    {
        double n = numberOfTimesCollided;
        int value = pow(2, n) - 1;
        delayValue =  rand() % value;
        return currentTime + delayValue;
        
    }
    
    double poisson (double anotherLambda)
    {
        double u;
        double val;
        
        u = rand();
        val = (-1) * log(u) * anotherLambda;
        Packet p(name, val);
        packetQueue.push(p);
        return val;
    }
    
    void send(double anotherLambda)
    {
        time += poisson(anotherLambda);
    }
    
    Packet createPacket(int i, double time)
    {
        return Packet(i, time);
    }
    
    void addPacketsToQueue(Packet aPacket)
    {
        packetQueue.push(aPacket);
    }
    
    Packet getNextPacketToBeTransmitted()
    {
        Packet p = packetQueue.front();
        packetQueue.pop();
        return p;
    }
    
    double getLambda()
    {
        return lamda;
        
    }
    
    double getTime()
    {
        return time;
    }
    
    int compareTo(Node t)
    {
        if (t.time < time) {
            return 1;
        } else if (t.time == time) {
            return 0;
        }
        else
            return -1;
    }


};
