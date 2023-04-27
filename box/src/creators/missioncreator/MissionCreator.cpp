#include "MissionCreator.h"
#include "LocationCreator.h"
#include "LocationMission.h"
#include "KeyMission.h"

Mission* MissionCreator::createInstance(MbedJSONValue & json){

        std::string name = json["name"].get<std::string>();
        int id = json["id"].get<int>();
        std::string description = json["description"].get<std::string>();
        int typeId = json["typeid"].get<int>();

        switch(typeId){
            case 10: {
                LocationMission * mission = new LocationMission(name, id, description, typeId);
                for(int j=0; j < json["locations"].size(); j++){
                    mission->addTargetLocation(LocationCreator::createInstance( json["locations"][j]));
                }
                return mission;
                break;
            }

            case 20: {
                return new KeyMission(name, id, description, typeId);
                break;
            }
        }
        return NULL;
}
