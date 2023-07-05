#include "time.h"

_time::_time() {
    clock = 0;
}
_time::_time(int t) {
    clock = t;
}
_time::_time(const Json::Value &info) {
    Json2Object(info);
}
bool _time::Json2Object(const Json::Value &info) {
    if(!info) return false;
    clock = info["clock"].asInt();
    return true;
}
Json::Value _time::dump2JSON()const {
    Json::Value v;
    v["clock"] = this->clock;
    return v;
}
void _time::clockTick() {
    clock++;
}
