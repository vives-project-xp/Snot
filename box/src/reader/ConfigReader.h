#pragma once

class ConfigReader{

    public:
        virtual void read(char * buffer, int bufferSize) = 0;

};
