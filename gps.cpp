#include "gps.h"

gps::gps() {
    name = "";
    x = 0;
    y = 0;
}
gps::gps(std::string n, int init_x, int init_y) {
    name = n;
    x = init_x;
    y = init_y;
}
gps::gps(const Json::Value &info) {
    Json2Object(info);
}
bool gps::Json2Object(const Json::Value &info) {
    if(!info) return false;
    name = info["name"].asCString();
    x = info["x"].asInt();
    y = info["y"].asInt();
    return true;
}
Json::Value gps::dump2JSON()const {
    Json::Value v;
    v["name"] = this->name;
    v["x"] = this->x;
    v["y"] = this->y;
    return v;
}