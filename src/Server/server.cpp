#include "Server.hpp"
#include "GFiles/File.hpp"
#include "Parser.hpp"
#include <iostream>

void HTTP::WebServer::HandleConnection(GNetworking::Socket *_clientSock)
{
    std::string input = _clientSock->Recv();

    HTTP::Request req((char *)input.c_str(), input.length());
    std::cout << req.GetRaw() << "\n\n";

    while (req.path.find("%20") != -1)
    {
        int pos = req.path.find("%20");
        req.path.replace(req.path.begin() + pos, req.path.begin() + pos + 3, " ");
    }

    // Requests
    if (req.type == HTTP::RequestType::GET)
    {
        HTTP::WebServer::Get(req, _clientSock);
    }
    else if (req.type == HTTP::RequestType::POST)
    {
        HTTP::WebServer::Post(req, _clientSock);
    }
    else if (req.type == HTTP::RequestType::HEAD)
    {
        HTTP::WebServer::Head(req, _clientSock);
    }
}

// Get Requests

void HTTP::WebServer::Get(HTTP::Request &req, GNetworking::Socket *_clientSock)
{
    GFiles::Path *reqPath;

    if (req.path == "/")
    {
        reqPath = new GFiles::Path("index.html");
    }
    else
    {
        reqPath = new GFiles::Path(req.path.erase(0, 1));
    }

    std::string pathExtension = reqPath->extension();

    HTTP::Response resp;
    int contentSize;

    if (pathExtension == "html")
    {
        // HTML return
        GFiles::File htmlPage(reqPath->path);
        if (htmlPage.exists())
        {
            std::string content = htmlPage.read();

            resp.code = 200;
            resp.version = HTTP::Version(1, 1);
            resp.AddHeader("Content-Type", "text/html; charset=utf-8");
            resp.SetMsg((char *)content.c_str(), content.length());

            char *respRaw = resp.CreateRaw("OK");
            _clientSock->Send(respRaw);
        }
    }
    else if (pathExtension == "css")
    {
        // CSS Return
        GFiles::File cssFile(reqPath->path);
        if (cssFile.exists())
        {
            std::string content = cssFile.read();

            resp.code = 200;
            resp.version = HTTP::Version(1, 1);
            resp.AddHeader("Content-Type", "text/css");
            resp.SetMsg((char *)content.c_str(), content.length());

            char *respRaw = resp.CreateRaw("OK");
            _clientSock->Send(respRaw);
        }
    }
    else if (pathExtension == "ico")
    {
        // Ico Return
        GFiles::File icoFile(reqPath->path, true);
        if (icoFile.exists())
        {
            char *content = icoFile.read();
            contentSize = icoFile.size();

            resp.code = 200;
            resp.version = HTTP::Version(1, 1);
            resp.AddHeader("Content-Type", "image/x-icon");
            resp.AddHeader("Content-Length", std::to_string(contentSize));
            resp.SetMsg(content, contentSize);

            char *output = resp.CreateRaw("OK");

            _clientSock->Send(output, resp.rawLen);
        }
    }
    else if (pathExtension == "png")
    {
        // PNG Return
        GFiles::File pngFile(reqPath->path, true);
        if (pngFile.exists())
        {
            char *content = pngFile.read();
            contentSize = pngFile.size();

            resp.code = 200;
            resp.version = HTTP::Version(1, 1);
            resp.AddHeader("Content-Type", "image/png");
            resp.AddHeader("Content-Length", std::to_string(contentSize));
            resp.SetMsg(content, contentSize);

            char *output = resp.CreateRaw("OK");

            _clientSock->Send(output, resp.rawLen);
        }
    }
    else if (pathExtension == "jpg")
    {
        // JPG Return
        GFiles::File jpgFile(reqPath->path, true);
        if (jpgFile.exists())
        {
            char *content = jpgFile.read();
            contentSize = jpgFile.size();

            resp.code = 200;
            resp.version = HTTP::Version(1, 1);
            resp.AddHeader("Content-Type", "image/jpg");
            resp.AddHeader("Content-Length", std::to_string(contentSize));
            resp.SetMsg(content, contentSize);

            char *output = resp.CreateRaw("OK");

            _clientSock->Send(output, resp.rawLen);
        }
    }

    delete reqPath;
    _clientSock->Close();
}

void HTTP::WebServer::Post(HTTP::Request &req, GNetworking::Socket *_clientSock)
{
    _clientSock->Close();
}

void HTTP::WebServer::Head(HTTP::Request &req, GNetworking::Socket *_clientSock)
{
    _clientSock->Close();
}

int main(int argc, char const *argv[])
{
    std::string address, port;
    address = "10.0.0.150";
    port = "80";

    std::cout << "Enter server address: ";
    std::getline(std::cin, address);

    std::cout << "Enter server port: ";
    std::getline(std::cin, port);

    HTTP::WebServer server(address, std::stoi(port), 3);
    return 0;
}