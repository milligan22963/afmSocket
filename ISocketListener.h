/**
 * Name: ISocketListener.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_ISOCKET_LISTENER
#define _H_ISOCKET_LISTENER

#include <memory>

#include "AfmSocketDefines.h"

namespace afm {
    namespace communications {

        class ISocketListener
        {
        public:
            virtual ~ISocketListener()
            {
            }

            virtual void onConnected() = 0;
            virtual void onDataReceived(const SocketBuffer &socketBuffer) = 0;
            virtual void onDataWritten(const SocketBuffer &socketBuffer) = 0;
            virtual void onError(int socketError) = 0;
            virtual void onDisconnected() = 0;
        };

        using ISocketListenerSPtr = std::shared_ptr<ISocketListener>; 
    }
}
#endif