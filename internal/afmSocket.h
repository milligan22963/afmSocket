/**
 * Name: afmSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_SOCKET
#define _H_AFM_SOCKET

#include <string>
#include <thread>
#include <sys/socket.h>

#include "IAfmSocket.h"

namespace afm {
  namespace communications {
    class AfmSocket : public IAfmSocket
    {
    public:
        AfmSocket();
        virtual ~AfmSocket();

        virtual bool initialize(const SocketOptions &options) override;
        virtual void shutdown() override;

        virtual void addListener(ISocketListenerSPtr pSocketListener) final;
        virtual void removeListener(ISocketListenerSPtr pSocketListener) final;


        virtual bool write(const SocketBuffer &data) override;
        virtual SocketBuffer read() override;
        virtual SocketBuffer readWait(uint32_t milliseconds) override;
        virtual SocketBuffer transfer(const SocketBuffer &data) override;
        virtual SocketBuffer transferWait(const SocketBuffer &data, uint32_t milliseconds) override;

    protected:
        int32_t getSocketHandle() const { return m_socketHandle; }
        void setSocketHandle(int32_t socketHandle) { m_socketHandle = socketHandle; }
        uint16_t getPort() const { return m_port; }
        std::string getUrl() const { return m_url; }
        std::string getUrlAddress();
        const ISocketListeners &getSocketListeners() const { return m_socketListeners; }

    private:
        size_t read(SocketBuffer &buffer);
        size_t readWait(SocketBuffer &buffer, uint32_t milliseconds);
        void read_processing();

    private:
      std::atomic<bool> m_threadRunning;
      int32_t           m_socketHandle = sc_closedSocket;
      std::string       m_url = "localhost";
      uint16_t          m_port = 9001;
      ISocketListeners  m_socketListeners;
      std::thread       m_processingThread;
    };

    using AfmSocketSPtr = std::shared_ptr<AfmSocket>; 
  }
}
#endif