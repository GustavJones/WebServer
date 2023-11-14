#include "Server.hpp"
#include "Parser.hpp"
#include <iostream>

void HTTP::WebServer::HandleConnection(GNetworking::Socket *_clientSock)
{
    std::cout << "Client Connected" << '\n';
    while (true)
    {
        std::string input = _clientSock->Recv();

        if (input == "")
        {
            std::cout << "Client Disconnected" << '\n';
            return;
        }

        try
        {
            Request req(input);

            std::cout << req.GetRaw() << "\n\n\n\n";

            if (req.GetType() == HTTP::RequestType::GET)
            {
                Response resp;
                resp.SetVersion(1.0);
                resp.SetCode(200);
                resp.SetMessage("<html>\n<head>\n<title>Hello, World!</title>\n</head>\n<body>\n<p>Hello, World</p>\n</body>\n</html>");

                _clientSock->Send(resp.CreateRaw("OK"));
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

int main(int argc, char const *argv[])
{
    HTTP::WebServer server("127.0.0.1", 4587, 2);
    return 0;
}