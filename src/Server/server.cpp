#include "Server.hpp"
#include "GFiles/File.hpp"
#include "Parser.hpp"
#include <iostream>

void HTTP::WebServer::HandleConnection(GNetworking::Socket *_clientSock)
{
    std::string htmlPage;
    std::string input = _clientSock->Recv();
    try
    {
        Request req(input);

        std::cout << req.GetRaw() << "\n\n";

        if (req.GetType() == HTTP::RequestType::GET)
        {
            if (req.GetPath() == "/" || req.GetPath() == "/index.html")
            {
                GFiles::File htmlFile(GFiles::Path("index.html"));

                char *buffer = htmlFile.read();
                if (buffer != nullptr)
                {
                    htmlPage = (std::string)buffer;
                }
                else
                {
                    htmlPage = "";
                }

                if (htmlFile.exists())
                {
                    std::cout << "File Exists" << '\n'
                              << '\n';
                }
            }
            else
            {
                std::string path = req.GetPath();
                GFiles::File htmlFile(GFiles::Path(path.erase(0, 1)));

                char *buffer = htmlFile.read();
                if (buffer != nullptr)
                {
                    htmlPage = (std::string)buffer;
                }
                else
                {
                    htmlPage = "";
                }

                if (htmlFile.exists())
                {
                    std::cout << "File Exists" << '\n'
                              << '\n';
                }
            }

            Response resp;
            resp.SetVersion(1.0);
            if (htmlPage != "")
            {
                resp.SetCode(200);
                resp.SetMessage(htmlPage);
                _clientSock->Send(resp.CreateRaw("OK"));
            }
            else
            {
                resp.SetCode(404);
                _clientSock->Send(resp.CreateRaw("Not Found"));
            }
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