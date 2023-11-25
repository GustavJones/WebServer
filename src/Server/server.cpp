#include "Server.hpp"
#include "GFiles/File.hpp"
#include "Parser.hpp"
#include <iostream>

void HTTP::WebServer::HandleConnection(GNetworking::Socket *_clientSock)
{
    std::string input = _clientSock->Recv().c_str();

    HTTP::Request req(input);
    std::cout << req.GetRaw() << "\n\n";

    // Requests
    if (req.GetType() == HTTP::RequestType::GET)
    {
        HTTP::WebServer::Get(req, _clientSock);
    }
    else if (req.GetType() == HTTP::RequestType::POST)
    {
        HTTP::WebServer::Post(req, _clientSock);
    }
    else if (req.GetType() == HTTP::RequestType::HEAD)
    {
        HTTP::WebServer::Head(req, _clientSock);
    }
}

// Get Requests

void HTTP::WebServer::Get(HTTP::Request &req, GNetworking::Socket *_clientSock)
{
    GFiles::Path *reqPath;

    if (req.GetPath() == "/")
    {
        reqPath = new GFiles::Path("index.html");
    }
    else
    {
        reqPath = new GFiles::Path(req.GetPath().erase(0, 1));
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

            std::map<std::string, std::string> headers;
            headers["Content-Type"] = "text/html; charset=utf-8";

            resp.SetCode(200);
            resp.SetVersion(1.1);
            resp.SetHeaders(headers);
            resp.SetMessage(content);

            char *respRaw = (char *)resp.CreateRaw("OK").c_str();
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

            std::map<std::string, std::string> headers;
            headers["Content-Type"] = "text/css";

            resp.SetCode(200);
            resp.SetVersion(1.1);
            resp.SetHeaders(headers);
            resp.SetMessage(content);

            char *respRaw = (char *)resp.CreateRaw("OK").c_str();
            _clientSock->Send(respRaw);
        }
    }
    else if (pathExtension == "ico")
    {
        // Ico Return
        GFiles::File icoFile(reqPath->path);
        if (icoFile.exists())
        {
            char *content = icoFile.read();

            std::map<std::string, std::string> headers;
            headers["Content-Type"] = "image/x-icon";

            resp.SetCode(200);
            resp.SetVersion(1.1);
            resp.SetHeaders(headers);
            resp.SetMessage(content);

            contentSize = icoFile.size();
            _clientSock->Send((char *)resp.CreateRaw("OK").c_str(), contentSize);
        }
    }
    else if (pathExtension == "png")
    {
        // PNG Return
        GFiles::File pngFile(reqPath->path);
        if (pngFile.exists())
        {
            char *content = pngFile.read();

            std::map<std::string, std::string> headers;
            headers["Content-Type"] = "image/png";

            resp.SetCode(200);
            resp.SetVersion(1.1);
            resp.SetHeaders(headers);
            resp.SetMessage(content);

            contentSize = pngFile.size();
            _clientSock->Send((char *)resp.CreateRaw("OK").c_str(), contentSize);
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

    // std::cout << "Enter server address: ";
    // std::getline(std::cin, address);

    // std::cout << "Enter server port: ";
    // std::getline(std::cin, port);

    HTTP::WebServer server(address, std::stoi(port), 3);
    return 0;
}