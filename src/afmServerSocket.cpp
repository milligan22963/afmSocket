/**
 * Name: afmServerSocket.cpp
 * 
 * Copyright: 2019 AFM Software
 */

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "afmServerSocket.h"

namespace afm {
    namespace communications {
        AfmServerSocket::AfmServerSocket()
            : AfmSocket()
        {

        }

        bool AfmServerSocket::initialize(const SocketOptions &options)
        {
            bool success = false;

            if (AfmSocket::initialize(options) == true) {
                int socketOption = 1;

                m_serverAddress.sin_family = AF_INET;
                m_serverAddress.sin_addr.s_addr = INADDR_ANY;
                m_serverAddress.sin_port = htons(getPort());

                if (setsockopt(getSocketHandle(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption, sizeof(socketOption)) == 0) {
                    if (bind(getSocketHandle(), (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) == 0) {

                        if (listen(getSocketHandle(), 20) == 0) {
                            m_threadRunning = true;
                            m_processingThread = std::thread(&AfmServerSocket::processing, this);
                            success = true;
                        }
                    }
                }
            }
            return success;
        }

        void AfmServerSocket::shutdown()
        {
            m_clients.clear();

            if (m_threadRunning == true) {
                m_threadRunning = false;
                m_processingThread.join();
            }

            AfmSocket::shutdown();
        }

        void AfmServerSocket::processing()
        {
            socklen_t addressSize = sizeof(m_serverAddress);

            while (m_threadRunning == true) {
                uint16_t client = accept(getSocketHandle(), (struct sockaddr *)&m_serverAddress, &addressSize);
                if (client != sc_closedSocket) {
                    // spawn it
                    AfmClientSocketSPtr pSocket = std::make_shared<AfmClientSocket>(client);

                    for (auto listener : getSocketListeners()) {
                        pSocket->addListener(listener);
                    }
                    m_clients.push_back(pSocket);
                }
                sleep(1); // pause a bit
            }
        }
    }
}
