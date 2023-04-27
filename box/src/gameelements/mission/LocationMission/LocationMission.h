#pragma once

#include "Mission.h"
#include "Location.h"

class LocationMission : public Mission{

    private:
        std::vector<Location *> locations;

    public:
        LocationMission(std::string name, int id, std::string description, int typeId);
        virtual void debug();
        // virtual bool isComplete();
        virtual void toLcd();

    public:
        double getLatitude();
        double getLongitude();
        double getRadius();
        double getDistanceToTarget();
        int getIndexOfClosestArea();
        void addTargetLocation(Location* location);

};
