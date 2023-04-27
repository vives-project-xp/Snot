#include "GameCreator.h"
#include "MbedJSONValue.h"
#include "MissionCreator.h"
#include "Mission.h"
#include "Location.h"
#include <string>

Game * GameCreator::createInstance(char * buffer){

    Game * game = new Game();

    MbedJSONValue json;
    parse(json, buffer);

    game->addBasicValues(
        json["name"].get<std::string>(),
        json["box_description"].get<std::string>(),
        json["length"].get<int>(),
        json["time"].get<int>()
    );

    for(int i=0; i < json["missions"].size(); i++){
        game->addMission( MissionCreator::createInstance(json["missions"][i]) );
    }

    return game;

}
