#pragma once
#include "Socket.h"
#include "Parser.hpp"
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
        virtual void Get(HTTP::Request &req, GNetworking::Socket *_clientSock);
        virtual void Post(HTTP::Request &req, GNetworking::Socket *_clientSock);
        virtual void Head(HTTP::Request &req, GNetworking::Socket *_clientSock);
    };

} // namespace HTTP
