#include "Server.hpp"
#include <iostream>

void HTTP::WebServer::HandleConnection(GNetworking::Socket *_clientSock)
{
    std::cout << "Client Connected" << '\n';
    while (true)
    {
        std::string Req = _clientSock->Recv();

        if (Req == "")
        {
            std::cout << "Client Disconnected" << '\n';
            return;
        }
    }
}

int main(int argc, char const *argv[])
{
    HTTP::WebServer server("127.0.0.1", 4587, 2);
    return 0;
}
