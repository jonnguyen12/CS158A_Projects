//
//  Node2.cpp
//  Phuc's Project 3
//
//  Created by Phuc Nguyen on 5/11/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <math.h>
#include "Packet.cpp"
#include <string>

using namespace std;

class Node2 {
private:
    double BACKOFF_CONSTANT = 51.2;
    double time;
    double lambda;
    int collisionCount;
    bool initialized = false;
    
    bool collided;
    priority_queue<Packet> packetQueue;
    
    
public:
    //Init a new node at time t
    Node2 (double newLambda)
    {
        collisionCount = 0;
        lambda = newLambda;
        time = poisson(newLambda);
        collided = false;
    }
    
    //Returns a random value
    double poisson (double aLambda){
        double u, val;
        u = rand();
        val = -1 * log(u) * aLambda;
        return val;
    }
    
    // Adds a poisson x value to current time
    void send (double anotherLambda)
    {
        collisionCount = 0;
        if (initialized) {
            time += poisson(anotherLambda);
            
        } else {
            initialized = !initialized;
            
        }
    }
    
    int compareTo(Node2 anotherNode)
    {
        if (time > anotherNode.time) {
            return 1;
        } else if (time == anotherNode.time) {
            return 0;
        }
        else
            return -1;
    }
    
    double getLambda()
    {
        return lambda;
    }
    
    double getTime() {return time;}
    
    void collide()
    {
        time += 1 + backoff();
        printf("collision count: %d.\n", collisionCount);
    }
    
    bool isCollided() {return collided;}
    
    void didCollide()
    {
        collided = true;
    }
    
    void notCollided()
    {
        collided = false;
    }
    
    string toString()
    {
        return "Start: " + to_string(time) + "\n";
    }
    
    double backoff ()
    {
        collisionCount++;
        int randomNumber = rand() % collisionCount;
        double delay = randomNumber * (pow(2, collisionCount) - 1);
        return delay;
    }
    
    
    double backoff (int numberCollision)
    {
        int randomNumber = rand() % numberCollision;
        double delay = randomNumber * (pow(2, collisionCount) - 1);
        return delay;
    }
    
    Packet createPacket(int i, double time)
    {
        return Packet(i, time);
    }
    
    void addPacketsToQueue(Packet anotherPacket)
    {
        packetQueue.push(anotherPacket);
    }
    
    Packet getNextPacketToBeTransmitted()
    {
        Packet p = packetQueue.top();
        packetQueue.pop();
        return p;
    }
};
