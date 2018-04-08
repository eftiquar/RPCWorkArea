//
//  main.cpp
//  RPCWork
//
//  Created by zayan on 4/7/18.
//  Copyright © 2018 Eftiquar. All rights reserved.
//

#include <iostream>
#include "RPCClient.hpp"
#include <assert.h>
using RPCClient::GetModelService;
using RPCServer::IRPCModelServerSvc;
using RPCServer::IRPCServiceBase;

int main(int argc, const char * argv[]) {

    shared_ptr<IRPCServiceBase> modelsvcBase = GetModelService(L"localhost:1443");
    assert(modelsvcBase);
    IRPCModelServerSvc* pmodelSvc  = static_cast<IRPCModelServerSvc*>(modelsvcBase.get());
    pmodelSvc->LoadModel();
    pmodelSvc->PerformInference();
    
    return 0;
}
