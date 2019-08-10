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


        bool AfmClientSocket::initialize(const SocketOptions &options)
        {
            bool success = false;

            if (AfmSocket::initialize(options) == true) {
                success = connect();

                if (success == false) {
                    shutdown();
                }
            }

            return success;
        }
    }
}
