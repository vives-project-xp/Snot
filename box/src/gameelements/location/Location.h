#pragma once

#include "Board.h"

class Location{
    private:
        double longitude;
        double latitude;
        double radius;
        
    
    public: 
        Location(double longitude, double latitude, double radius);
        
    public:
        double getLongitude();
        double getLatitude();
        double getRadius();
        double getDistanceToTarget(Board * board);
    
        
};