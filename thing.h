#ifndef thing_compiled
#define thing_compiled
#include "general.h"

class thing {
    public:
        std::string name;
        std::string description;
        thing();
        thing(std::string n, std::string d);
        thing(const Json::Value &info);
        virtual bool Json2Object(const Json::Value &info);
        virtual ~thing() {};
        virtual Json::Value dump2JSON()const;
};

#endif