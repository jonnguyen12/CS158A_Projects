//
//  Packet.cpp
//  Phuc's Project 3
//
//  Created by Phuc Nguyen on 5/11/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

class Packet {
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
    double poisson (double aLambda)
    
};