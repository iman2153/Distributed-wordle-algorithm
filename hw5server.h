#ifndef JSONRPC_CPP_STUB_HW5SERVER_H_
#define JSONRPC_CPP_STUB_HW5SERVER_H_
#include "general.h"
#include <jsonrpccpp/server.h>

class hw5Server : public jsonrpc::AbstractServer<hw5Server>
{
    public:
        hw5Server(jsonrpc::AbstractServerConnector &conn, jsonrpc::serverVersion_t type = jsonrpc::JSONRPC_SERVER_V2) : jsonrpc::AbstractServer<hw5Server>(conn, type)
        {
            this->bindAndAddMethod(jsonrpc::Procedure("action", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_OBJECT, NULL), &hw5Server::actionI);
        }

        inline virtual void actionI(const Json::Value &request, Json::Value &response)
        {
            response = this->action(request);
        }
        
        virtual Json::Value action(const Json::Value& info) = 0;
};

#endif
