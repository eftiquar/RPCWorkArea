//
//  RPCServer.cpp
//  RPCWork
//
//  Created by zayan on 4/7/18.
//  Copyright © 2018 Eftiquar. All rights reserved.
//

#include "RPCServer.hpp"
#include <assert.h>
#include <iostream>
//#include <atomic>
#include <memory>
#include <map>
#include <mutex>
using std::mutex;
using std::map;
using std::shared_ptr;
//using atomic_counter = std::atomic_size_t;

namespace RPCServer
{
////////////////////////////
    using HANDLE = void*;
    auto ServerConnectionDeleter = [](const HANDLE* ptrConnection)
    {
      if(ptrConnection)
      {
          //close the connection with remote server using *ptrConnection
          delete ptrConnection;
      }
    };
    
    HANDLE GetService(HANDLE hServer)
    {
        //placeholder, ask remote server to give us the handle
        return nullptr;
    }
    void ReleaseService(HANDLE hServer, HANDLE hService)
    {
        //perform cleanup
    }
////////////////////////////////////////
    class RPCServerStub;
    class RPCModelServerSvc : public IRPCModelServerSvc
    {
        friend class RPCServerStub;
        friend IRPCServiceBase* QueryService(const RPCServer::RPC_SERVICE_GUID svcGuid);
    public:
        void Release() override;
        void LoadModel() override;
        void PerformInference() override;
        void UnloadModel() override;
        RPCModelServerSvc(const RPCModelServerSvc& rpcModelSserver) =delete ;
        const RPCModelServerSvc& operator=(const RPCModelServerSvc&) = delete ;
        
        
    protected:
        
        RPCModelServerSvc(HANDLE hServerConnection, HANDLE hSvcConnection);
        virtual ~RPCModelServerSvc();
    private:
        
        HANDLE m_hServerConnection;
        HANDLE m_hSvcConnection;
    };
    RPCModelServerSvc::RPCModelServerSvc(HANDLE hServerConnection,HANDLE hsvcConnection):m_hServerConnection(hServerConnection),m_hSvcConnection(hsvcConnection)
    {
        std::cout << "Constructing RPCModelServerSvc" << std::endl;
    }
    RPCModelServerSvc::~RPCModelServerSvc()
    {
        std::cout << "Destructing ~RPCModelServerSvc" << std::endl;
        if(m_hSvcConnection && m_hServerConnection)
        {
         //cleanup remote server the tuple <m_hServerConnection,m_hSvcConnection> will locate server and this service
        }
        
    }
    void RPCModelServerSvc::Release()
    {
        delete this;
    }
    void RPCModelServerSvc::LoadModel()
    {
        std::cout << "Load Model" << std::endl;
    }
    void RPCModelServerSvc::UnloadModel()
    {
        std::cout << "Unload Model" << std::endl;
    }
    void RPCModelServerSvc::PerformInference()
    {
        std::cout << "PerformInference" << std::endl;
    }
/////////////////////////
    class RPCServerStub: public IRPCServer
    {
        friend IRPCServer* CreateServer(const wchar_t * const connectString );
    public:
        shared_ptr<IRPCServiceBase> QueryService(const RPC_SERVICE_GUID svcGuid) override;
        RPCServerStub(const RPCServerStub&) = delete ;
        RPCServerStub operator=(const RPCServerStub&) = delete ;
        
    protected:
        RPCServerStub(const wchar_t * const connectString);
        ~RPCServerStub();
    private:
        const wchar_t * const m_ConnectString;
        std::unique_ptr<HANDLE,decltype(ServerConnectionDeleter)> m_spConnection;
        map<RPC_SERVICE_GUID,shared_ptr<IRPCServiceBase>> m_serviceMap;
        mutex m_mapMutex;
    };
    RPCServerStub::~RPCServerStub()
    {
         std::cout << "Destructing ~RPCServerStub" << std::endl;
        //releaseconnection(m_hConnection);
    }
    RPCServerStub::RPCServerStub(const wchar_t * const connectString):m_ConnectString(connectString),m_spConnection(new HANDLE(nullptr),ServerConnectionDeleter)
    {
        //m_hConnection = use grpc to connect to remote server ,m_hConnection identifies session with server
        std::cout << "Constructing RPCServerStub" << std::endl;
    }
    shared_ptr<IRPCServiceBase> RPCServerStub::QueryService(const RPC_SERVICE_GUID svcGuid)
    {
        {
            std::lock_guard<std::mutex> mapLock(m_mapMutex);
            auto foundIt = m_serviceMap.find(svcGuid);
            if(foundIt != m_serviceMap.end())
            {
                return foundIt->second;
            }
        }
        
        //assert(m_hConnection);
        HANDLE hSvc = GetService(m_spConnection.get());
        
         // dont want to call expensive functions with lock held. so calling RPCModelServerSvc outside lock
        
        shared_ptr<IRPCServiceBase> result = std::shared_ptr<IRPCServiceBase>( new RPCModelServerSvc( m_spConnection.get(),hSvc),
                                                                                [](IRPCServiceBase* pBaseSvc)
                                                                                {
                                                                                    assert(pBaseSvc);
                                                                                    if(pBaseSvc)
                                                                                        pBaseSvc->Release();
                                                                                });
        {
            std::lock_guard<std::mutex> mapLock(m_mapMutex);
            //this is double checked locking,
            auto foundIt = m_serviceMap.find(svcGuid);
            if(foundIt != m_serviceMap.end())
            {
                
                return foundIt->second;
            }
            m_serviceMap[svcGuid] = result;
        }
        return result;
        
    }
    
    IRPCServer* CreateServer(const wchar_t * const connectString )
    {
        //C++ 11 onwards, this construct is thread safe.
        static RPCServerStub serverStub (connectString);
        return &serverStub;
    }
}
