#include "person.h"

person::person(const Json::Value &info) {
    Json2Object(info);
}
bool person::Json2Object(const Json::Value &info) {
    if(!info) return false;
    name = info["name"].asCString();
    gender = info["gender"].asCString();
    age = info["age"].asInt();
    location = gps(info);
    if(info["inventory"]) {
        for(int i = 0; i < info["inventory"].size(); i++) {
            gainThing(thing(info["inventory"][i]));
        }
    }
    return true;
}
Json::Value person::dump2JSON()const {
    Json::Value v;
    v["name"] = this->name;
    v["gender"] = this->gender;
    v["age"] = this->age;
    v["location"] = this->location.dump2JSON();
    Json::Value inven;
    for(int i = 0; i < inventory.size(); i++) {
        inven[i] = inventory[i].dump2JSON();
    }
    v["inventory"] = inven;
    return v;
}
Json::Value person::gainThing(thing t) {
    inventory.push_back(t);
    Json::Value v;
    v["actionType"] = "gainThing";
    v["thing"] = t.dump2JSON();
    v["person"] = this->dump2JSON();
    return v;
}
Json::Value person::loseThing(std::string n) {
    Json::Value v;
    for(int i = 0; i < inventory.size(); i++) {
        if(inventory[i].name == n) {
            v["actionType"] = "loseThing";
            v["thing"] = inventory[i].dump2JSON();
            inventory.erase(inventory.begin() + i);
            v["person"] = this->dump2JSON();
            return v;
        }
    }
    return v;
}
Json::Value person::moveTo(const gps& newLocation) {
    Json::Value v;
    location = newLocation;
    v["actionType"] = "moveTo";
    v["newLocation"] = newLocation.dump2JSON();
    v["person"] = this->dump2JSON();
    return v;
}
Json::Value person::encouter(person p) {
    Json::Value v;
    v["actionType"] = "encouter";
    v["person2"] = p.dump2JSON();
    v["person1"] = this->dump2JSON();
    return v;
}
