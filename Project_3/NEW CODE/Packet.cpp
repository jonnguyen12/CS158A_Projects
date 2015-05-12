//
//  Packet.cpp
//  Phuc's Project 3
//
//  Created by Phuc Nguyen on 5/11/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <string>

namespace std
{
    class Packet
    {
    private:
        int stationName = 0;
        double contentionInterval = 0;
        double transmittedTime = 0;
        int collisionCount = 0;
        
    public:
        Packet(int nameOfStation, double generatedTime)
        {
            contentionInterval = generatedTime;
            stationName = nameOfStation;
            transmittedTime = 0.0;
            collisionCount = 0;
        }
        
        double getContentionInterval()
        {
            return contentionInterval;
        }
        
        int getStationName()
        {
            return stationName;
        }
        
        double getStationTime()
        {
            return transmittedTime;
        }
        
        void setContentionInterval(double interval)
        {
            contentionInterval = interval;
        }
        
        void setStationName(int name)
        {
            stationName = name;
        }
        
        void incrementCollision()
        {
            collisionCount++;
        }
        
        void setStationTime(double time)
        {
            transmittedTime = time;
        }
        
        int getCollisionCount()
        {
            return collisionCount;
        }
        
        string getString()
        {
            return to_string(contentionInterval) + " " + to_string(stationName);
        }
        
        int compareTo(Packet t)
        {
            if (t.contentionInterval < contentionInterval) {
                return -1;
            } else if (t.contentionInterval == contentionInterval) {
                return 0;
            }
            else
                return 1;
        }
    };
}
