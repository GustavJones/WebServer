#include "Parser.hpp"
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string requestRaw;
    requestRaw = (std::string) "    GET    /index.html      HTTP/1.1\r\n" +
                 (std::string) "header1: value1\r\n" +
                 (std::string) "header2: value2\r\n" +
                 (std::string) "header3: value3,\r\n" +
                 (std::string) " value4, value5, value6\r\n" +
                 (std::string) "header4: value7\r\n" +
                 (std::string) "\n" +
                 (std::string) "this is the message\n";

    HTTP::Request test1((char *)requestRaw.c_str(), requestRaw.length());

    HTTP::Request test2;
    test2.type = HTTP::POST;
    test2.path = "/index.html";
    test2.version = HTTP::Version(1, 1);
    test2.AddHeader("Host", "127.0.0.1:80");
    test2.AddHeader("User-Agent", "Test/1234");
    test2.AddHeader("Header1", "Value");
    test2.AddHeader("Header2", "Value2");
    test2.AddHeader("Header3", "Value3");
    test2.AddHeader("Header4", "Value4");

    std::string reqmsg = "This is the message of the request line";

    test2.SetMsg((char *)reqmsg.c_str(), reqmsg.length());

    std::cout << test2.CreateRaw() << "\n\n\n";

    std::string responseRaw;
    responseRaw = (std::string) "    HTTP/1.1 200    OK\r\n" +
                  (std::string) "header1: value1\r\n" +
                  (std::string) "header2: value2,\r\n" +
                  (std::string) " value3\r\n" +
                  (std::string) "\n" +
                  (std::string) "this is the message\n";

    HTTP::Response test3((char *)responseRaw.c_str(), responseRaw.length());

    HTTP::Response test4;
    test4.version = HTTP::Version(1, 1);
    test4.code = 200;

    test4.AddHeader("Host", "127.0.0.1:80");
    test4.AddHeader("User-Agent", "WIndows/1234");

    std::string respmsg = "this is the message";

    test4.SetMsg((char *)respmsg.c_str(), respmsg.length());

    std::cout << test4.CreateRaw("OK") << "\n\n\n";

    return 0;
}
