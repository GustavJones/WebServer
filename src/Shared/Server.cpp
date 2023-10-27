#include "Server.hpp"
#include "Socket.h"
#include <string>

namespace HTTP
{
    WebServer::WebServer(std::string _ip, short _port, short _threads) : m_threadCount(_threads)
    {
        m_serverSock = new GNetworking::Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        m_serverSock->Bind(_ip, _port);
        m_serverSock->Listen();

        for (int i = 0; i < _threads; i++)
        {
            m_threads.push_back(std::thread(&WebServer::Connection, this));
        }
    }

    void WebServer::Connection()
    {
        while (true)
        {
            GNetworking::Socket *clientSock = m_serverSock->Accept();
            HandleConnection(clientSock);
        }
    }

    WebServer::~WebServer()
    {
        for (int i = 0; i < m_threadCount; i++)
        {
            m_threads[i].join();
        }

        m_serverSock->Close();
    }
} // namespace HTTP
