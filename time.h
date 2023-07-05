#ifndef time_compiled
#define time_compiled
#include "general.h"

class _time {
    public:
        int clock;
        _time();
        _time(int t);
        _time(const Json::Value &info);
        virtual bool Json2Object(const Json::Value &info);
        virtual ~_time() {};
        virtual Json::Value dump2JSON()const;
        void clockTick();
};

#endif