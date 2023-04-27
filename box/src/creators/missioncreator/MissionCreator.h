#pragma once

#include "Mission.h"
#include "MbedJSONValue.h"

class MissionCreator {

    public:
        static Mission * createInstance(MbedJSONValue & json);

};
