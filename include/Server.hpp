#pragma once
#include "Socket.h"
#include <string>
#include <vector>
#include <thread>

namespace HTTP
{
    class WebServer
    {
    private:
        std::vector<std::thread> m_threads;
        short m_threadCount;
        GNetworking::Socket *m_serverSock;

        void Connection();

    public:
        WebServer(std::string _ip, short _port, short _threads);
        ~WebServer();

        virtual void HandleConnection(GNetworking::Socket *_clientSock);
    };

} // namespace HTTP
