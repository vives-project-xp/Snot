#include "LocationCreator.h"

Location* LocationCreator::createInstance(MbedJSONValue & json){
        
        Location * location = new Location(
            json["long"].get<double>(),
            json["lat"].get<double>(),
            json["radius"].get<double>()
        );
    
        return location;
}

