#include "StringReader.h"
#include <string>

StringReader::StringReader(std::string configuration){
    this->configuration = configuration;
}

void StringReader::read(char * buffer, int bufferSize){
    if(bufferSize > configuration.length()){
        configuration.copy(buffer, configuration.length(), 0);
    }
}
