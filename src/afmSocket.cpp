/**
 * Name: afmSocket.cpp
 * 
 * Copyright: 2019 AFM Software
 */

#include <iostream>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "JSONUtil.h"

#include "afmSocket.h"

namespace afm {
    namespace communications {
        AfmSocket::AfmSocket()
            : m_threadRunning(false)
            , m_socketConnected(false)
        {
        }

        AfmSocket::~AfmSocket()
        {
            shutdown();
        }

        bool AfmSocket::initialize(const SocketOptions &options)
        {
            bool success = false;
            
            if (extractValue(options, sc_socketURL, m_url) == true) {
                if (extractValue(options, sc_socketPort, m_port) == true) {
                    m_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
                    std::cout << "Socket has been created.\n";
                    if (m_socketHandle != sc_closedSocket) {
                        success = true;
                    }
                }
            }

            return success;
        }

        void AfmSocket::shutdown()
        {
            if (m_threadRunning == true) {
                m_threadRunning = false;
                m_processingThread.join();
            }

            for (auto listener : m_socketListeners) {
                listener->onDisconnected();
            }
            m_socketListeners.clear();

            if (m_socketHandle != sc_closedSocket) {
                ::close(m_socketHandle);
                m_socketHandle = sc_closedSocket;
            }
        }

        void AfmSocket::addListener(ISocketListenerSPtr pSocketListener)
        {
            m_socketListeners.push_back(pSocketListener);
            if (m_threadRunning == false) {
                m_threadRunning = true;
                m_processingThread = createProcessingThread();
            }
        }

        void AfmSocket::removeListener(ISocketListenerSPtr pSocketListener)
        {
            m_socketListeners.remove(pSocketListener);
            if (m_socketListeners.size() == 0) {
                if (m_threadRunning == true) {
                    m_threadRunning = false;
                    m_processingThread.join();
                }
            }
        }

        bool AfmSocket::write(const SocketBuffer &data)
        {
            bool success = true;
            std::cout << "Socket Handle: " << m_socketHandle << "\n";
            std::cout << "Preparing to write: " << (int)data.size() << " bytes\n";
            if (::write(m_socketHandle, data.data(), data.size()) > 0) {
                for (auto listener : m_socketListeners) {
                    listener->onDataWritten(data);
                }
            } else {
                socketFailure();
                success = false;
            }

            return success;
        }

        SocketBuffer AfmSocket::read()
        {
            SocketBuffer data;

            size_t bytesRead = read(data);

            if (bytesRead == 0) {
                // error?
            }

            return data;
        }

        SocketBuffer AfmSocket::readWait(uint32_t milliseconds)
        {
            SocketBuffer data;

            size_t bytesRead = readWait(data, milliseconds);

            if (bytesRead == 0) {
                // error?
            }

            return data;
        }

        SocketBuffer AfmSocket::transfer(const SocketBuffer &data)
        {
            write(data);

            return read();
        }

        SocketBuffer AfmSocket::transferWait(const SocketBuffer &data, uint32_t milliseconds)
        {
            write(data);

            return readWait(milliseconds);
        }

        bool AfmSocket::connect()
        {
            bool success = false;
            struct sockaddr_in hostAddress;

            hostAddress.sin_family = AF_INET;
            hostAddress.sin_port = htons(m_port);

            if (inet_pton(AF_INET, getUrlAddress().c_str(), &hostAddress.sin_addr) > 0) {
                if (::connect(m_socketHandle, (struct sockaddr *)&hostAddress, sizeof(hostAddress)) >= 0) {
                    success = true;
                    for (auto listener : m_socketListeners) {
                        listener->onConnected();
                    }
                }
            }

            return success;
        }

        std::string AfmSocket::getUrlAddress()
        {
            std::string hostAddress;
            struct hostent *pHostEntry = nullptr;
            struct in_addr **ppAddressList = nullptr;
                
            if ((pHostEntry = gethostbyname(m_url.c_str())) != nullptr) {
                ppAddressList = (struct in_addr **)pHostEntry->h_addr_list;
                for (int addressIndex = 0; ppAddressList[addressIndex] != nullptr; addressIndex++) 
                {
                    char buffer[32];
                    strncpy(buffer, inet_ntoa(*ppAddressList[addressIndex]), 31);
                    hostAddress = buffer;
                    break;
                }
            }
            return hostAddress;
        }

        std::thread AfmSocket::createProcessingThread()
        {
            return std::thread(&AfmSocket::read_processing, shared_from_this());
        }

        ssize_t AfmSocket::read(SocketBuffer &buffer)
        {
            ssize_t bytesRead = 0;

            buffer.clear();
            
            uint8_t data_buffer[1024];
            bytesRead = ::read(m_socketHandle, data_buffer, 1023);
            if (bytesRead > 0) {
                buffer.reserve(bytesRead);
                std::copy(data_buffer, data_buffer + bytesRead, std::back_inserter(buffer));
            } else if (bytesRead == -1) {
                socketFailure();
            }
            return bytesRead;
        }

        ssize_t AfmSocket::readWait(SocketBuffer &buffer, uint32_t milliseconds)
        {
            ssize_t bytesRead = 0;
            time_t seconds = milliseconds / 1000;
            suseconds_t microseconds = (milliseconds - (seconds * 1000)) * 1000;

            struct timeval tv = {seconds, microseconds};
            fd_set readDescriptorSet;
            fd_set exceptionDescriptorSet;
            
            FD_ZERO(&readDescriptorSet);
            FD_ZERO(&exceptionDescriptorSet);

            FD_SET(m_socketHandle, &readDescriptorSet);
            FD_SET(m_socketHandle, &exceptionDescriptorSet);

            if (select(m_socketHandle + 1, &readDescriptorSet, nullptr, &exceptionDescriptorSet, &tv) != -1) {
                if (FD_ISSET(m_socketHandle, &readDescriptorSet)) {
                    bytesRead = read(buffer);
                } else if (FD_ISSET(m_socketHandle, &exceptionDescriptorSet)) {
                    socketFailure();
                }
            } else {
                socketFailure();
            }
            return bytesRead;
        }

        void AfmSocket::socketFailure()
        {
            m_socketConnected = false;

            // may need to do this prior to this call
            m_lastError = errno;

            for (auto listener : m_socketListeners) {
                listener->onError(m_lastError);
            }
        }

        void AfmSocket::read_processing()
        {
            SocketBuffer incomingData;

            while (m_threadRunning == true) {
                if (m_socketConnected == true) {
                    if (readWait(incomingData, 1000) > 0) {
                        for (auto listener : m_socketListeners) {
                            listener->onDataReceived(incomingData);
                        }
                    }
                } else {
                    for (int count = 0; count < sm_connectionDelay; count++) {
                        sleep(1); // so we don't hammer the connection
                    }
                    m_socketConnected = connect();
                }
            }
        }
    }
}
