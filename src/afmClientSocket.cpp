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
                struct sockaddr_in hostAddress;

                hostAddress.sin_family = AF_INET;
                hostAddress.sin_port = htons(getPort());

                if (inet_pton(AF_INET, getUrlAddress().c_str(), &hostAddress.sin_addr) > 0) {
                    if (connect(getSocketHandle(), (struct sockaddr *)&hostAddress, sizeof(hostAddress)) >= 0) {
                        success = true;
                    }
                }

                if (success == false) {
                    shutdown();
                }
            }

            return success;
        }
    }
}
