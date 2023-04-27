#pragma once

#include "Location.h"
#include "MbedJSONValue.h"

class LocationCreator{
    
    public:
        static Location * createInstance(MbedJSONValue & json);    

};