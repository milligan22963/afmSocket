/**
 * Name: afmServerSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_SERVER_SOCKET
#define _H_AFM_SERVER_SOCKET

#include <atomic>
#include <list>
#include <thread>

#include "afmSocket.h"
#include "afmClientSocket.h"

namespace afm {
  namespace communications {

    using ClientSockets = std::list<AfmClientSocketSPtr>;

    class AfmServerSocket : public AfmSocket
    {
    public:
        AfmServerSocket();

        virtual bool initialize(const SocketOptions &options) override;
        virtual void shutdown() override;

    protected:
        void processing();

    private:
        std::atomic<bool>   m_threadRunning;
        std::thread         m_processingThread;
        struct sockaddr_in  m_serverAddress;
        ClientSockets       m_clients;
    };

    using AfmServerSocketSPtr = std::shared_ptr<AfmServerSocket>; 
  }
}
#endif