/**
 * Name: afmClientSocket.cpp
 * 
 * Copyright: 2019 AFM Software
 */

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "afmClientSocket.h"

namespace afm {
    namespace communications {

        AfmClientSocket::AfmClientSocket()
            : AfmSocket()
        {

        }

        AfmClientSocket::AfmClientSocket(int32_t socketHandle)
            : AfmSocket()
        {
            setSocketHandle(socketHandle); // already have a socket
        }

        std::thread AfmClientSocket::createProcessingThread()
        {
            return std::thread(&AfmClientSocket::read_processing, shared_from_base<AfmClientSocket>());
        }
    }
}
