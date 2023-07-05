#ifndef person_compiled
#define person_compiled
#include "general.h"
#include "thing.h"
#include "gps.h"

class person {
    public:
        std::string name;
        std::string gender;
        int age;
        gps location;
        std::vector<thing> inventory;
        person(){};
        person(const Json::Value &info);
        virtual bool Json2Object(const Json::Value &info);
        virtual ~person() {};
        virtual Json::Value dump2JSON()const;
        virtual Json::Value gainThing(thing t);
        virtual Json::Value loseThing(std::string n);
        virtual Json::Value moveTo(const gps& newLocation);
        virtual Json::Value encouter(person p);
};

#endif
