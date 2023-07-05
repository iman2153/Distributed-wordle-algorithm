#ifndef gps_compiled
#define gps_compiled
#include "general.h"

class gps {
    public:
        std::string name;
        int x;
        int y;
        gps(std::string n, int init_x, int init_y);
        gps();
        gps(const Json::Value &info);
        virtual bool Json2Object(const Json::Value &info);
        virtual ~gps() {};
        virtual Json::Value dump2JSON()const;
};

#endif