#ifndef JSONRPC_CPP_STUB_HW5CLIENT_H_
#define JSONRPC_CPP_STUB_HW5CLIENT_H_

#include <jsonrpccpp/client.h>

class hw5Client : public jsonrpc::Client
{
    public:

        hw5Client(jsonrpc::IClientConnector &conn, jsonrpc::clientVersion_t type = jsonrpc::JSONRPC_CLIENT_V2) : jsonrpc::Client(conn, type) {}

        Json::Value action(const Json::Value& info) 
        {
            Json::Value result = this->CallMethod("action", info);
            return result;
        }
};

#endif
