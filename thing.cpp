#include "thing.h"

thing::thing() {
    name = "";
    description = "";
}
thing::thing(std::string n, std::string d) {
    name = n;
    description = d;
}
thing::thing(const Json::Value &info) {
    Json2Object(info);
}
bool thing::Json2Object(const Json::Value &info) {
    if(!info) return false;
    name = info["name"].asCString();
    description = info["name"].asCString();
    return true;
}
Json::Value thing::dump2JSON()const {
    Json::Value v;
    v["name"] = this->name;
    v["description"] = this->description;
    return v;
}
