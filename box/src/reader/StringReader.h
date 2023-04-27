#pragma once
#include "ConfigReader.h"
#include <string>

class StringReader : public ConfigReader {
    private:
        std::string configuration;

    public:
        StringReader(std::string configuration);

    public:
        virtual void read(char * buffer, int bufferSize);

};
