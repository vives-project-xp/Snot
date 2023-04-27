#include "Location.h"


Location::Location(double longitude, double latitude, double radius){
    this->longitude = longitude;
    this->latitude = latitude;
    this->radius = radius;
}

double Location::getLongitude(){
    return longitude;
}

double Location::getLatitude(){
    return latitude;
}

double Location::getRadius(){
    return radius;
}

double Location::getDistanceToTarget(Board * board){
    return 0.0;
    // return board->gps->distance_to_target(latitude, longitude);
}

