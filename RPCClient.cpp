//
//  RPCClient.cpp
//  RPCWork
//
//  Created by zayan on 4/7/18.
//  Copyright © 2018 Eftiquar. All rights reserved.
//

#include "RPCClient.hpp"
#include "RPCServer.hpp"
#include <exception>
using namespace RPCServer;
using std::exception;

namespace RPCClient
{    

    shared_ptr<IRPCServiceBase> GetModelService(const wchar_t* const connectString)
    {
        try
        {

            IRPCServer* rpcServerStub = CreateServer(connectString);
            return rpcServerStub->QueryService(RPCServer::RPC_SERVICE_GUID::RPC_MODEL_SERVER);
        }
        catch( exception & ex)
        {
        
        }
        return nullptr;
}

}
