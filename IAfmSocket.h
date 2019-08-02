/**
 * Name: IAfmSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_IAFM_SOCKET
#define _H_IAFM_SOCKET

#include <json.hpp>

#include <atomic>
#include <cstdint>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "AfmSocketDefines.h"
#include "ISocketListener.h"

namespace afm {
    namespace communications {
        class IAfmSocket
        {
        public:
            virtual ~IAfmSocket()
            {

            }

            virtual bool initialize(const SocketOptions &options) = 0;
            virtual void shutdown() = 0;
            virtual void addListener(ISocketListenerSPtr pSocketListener) = 0;
            virtual void removeListener(ISocketListenerSPtr pSocketListener) = 0;

            virtual bool write(const SocketBuffer &data) = 0;
            virtual SocketBuffer read() = 0;
            virtual SocketBuffer readWait(uint32_t milliseconds) = 0;
            virtual SocketBuffer transfer(const SocketBuffer &data) = 0;
            virtual SocketBuffer transferWait(const SocketBuffer &data, uint32_t milliseconds) = 0;
        };

        using ISocketListeners = std::list<ISocketListenerSPtr>;
        using IAfmSocketSPtr = std::shared_ptr<IAfmSocket>;
    }
}
#endif