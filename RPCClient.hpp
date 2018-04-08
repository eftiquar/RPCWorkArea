//
//  RPCClient.hpp
//  RPCWork
//
//  Created by zayan on 4/7/18.
//  Copyright © 2018 Eftiquar. All rights reserved.
//

#ifndef RPCClient_hpp
#define RPCClient_hpp
#include <stdio.h>
#include "RPCServer.hpp"
#include <memory>
using std::shared_ptr;
using RPCServer::IRPCServiceBase;
namespace RPCClient
{
    shared_ptr<IRPCServiceBase> GetModelService(const wchar_t* const connectString);
    
}

#endif /* RPCClient_hpp */
