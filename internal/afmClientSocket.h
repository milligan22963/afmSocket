/**
 * Name: afmClientSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_CLIENT_SOCKET
#define _H_AFM_CLIENT_SOCKET

#include "afmSocket.h"

namespace afm {
  namespace communications {

    class AfmClientSocket : public AfmSocket
    {
    public:
        AfmClientSocket();

        /**
         * @brief create a client socket with an existing socket handle.
         *        this will become owned by this class and should not be
         *        closed by anyone else.
         */
        AfmClientSocket(int32_t socketHandle);

        protected: 
          /**
           * @copydoc AfmSocket::createProcessingThread
           */
          virtual std::thread createProcessingThread() override;
    };

    using AfmClientSocketSPtr = std::shared_ptr<AfmClientSocket>; 
  }
}
#endif