#include "Server.hpp"
#include "FileManage.h"
#include "Parser.hpp"
#include <iostream>

void HTTP::WebServer::HandleConnection(GNetworking::Socket *_clientSock)
{
    std::string htmlPage;
    std::string input = _clientSock->Recv();
    try
    {
        Request req(input);

        std::cout << req.GetRaw() << "\n\n\n\n";

        if (req.GetType() == HTTP::RequestType::GET)
        {
            if (req.GetPath() == "/" || req.GetPath() == "/index.html")
            {
                FileManage::File htmlFile("index.html");
                htmlPage = htmlFile.ReadFile();
            }
            else
            {
                std::string path = req.GetPath();
                FileManage::File htmlFile(path.erase(0, 1));
                htmlPage = htmlFile.ReadFile();
            }

            Response resp;
            resp.SetVersion(1.0);
            resp.SetCode(200);
            // resp.SetMessage("<html>\n<head>\n<title>Hello, World!</title>\n</head>\n<body>\n<p>Hello, World</p>\n</body>\n</html>");
            resp.SetMessage(htmlPage);

            _clientSock->Send(resp.CreateRaw("OK"));
            _clientSock->Close();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char const *argv[])
{
    std::string address, port;

    std::cout << "Enter server address: ";
    std::getline(std::cin, address);

    std::cout << "Enter server port: ";
    std::getline(std::cin, port);

    HTTP::WebServer server(address, std::stoi(port), 3);
    return 0;
}