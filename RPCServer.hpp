//
//  RPCServer.hpp
//  RPCWork
//
//  Created by zayan on 4/7/18.
//  Copyright © 2018 Eftiquar. All rights reserved.
//

#ifndef RPCServer_hpp
#define RPCServer_hpp

#include <stdio.h>
#include <memory>
using std::shared_ptr;



namespace RPCServer
{
    class IRPCServer;
    enum class RPC_SERVICE_GUID
    {
        RPC_MODEL_SERVER
        
    };
    class IRPCServiceBase
    {
    public:
        
        virtual void Release()=0;
    protected:
        virtual ~IRPCServiceBase(){};
        
    };
    class IRPCModelServerSvc : public IRPCServiceBase
    {
        
    public :
        virtual void LoadModel()= 0;
        virtual void PerformInference() =0;
        virtual void UnloadModel() = 0;
        
    protected :
        virtual ~IRPCModelServerSvc(){};
    };

    class IRPCServer
    {
    public:
        
        virtual shared_ptr<IRPCServiceBase> QueryService(const RPC_SERVICE_GUID svcGuid) =0;
    };
    
    IRPCServer* CreateServer(const wchar_t * const connectString );
}


#endif /* RPCServer_hpp */
