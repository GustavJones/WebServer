#include "Parser.hpp"
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
    // HTTP::Request test1(
    //     (std::string) "    GET    https://this.com/is/a/test.txt      HTTP/1.1\r\n" +
    //     (std::string) "header1: value1\r\n" +
    //     (std::string) "header2: value2\r\n" +
    //     (std::string) "header3: value3,\r\n" +
    //     (std::string) " value4, value5, value6\r\n" +
    //     (std::string) "header4: value7\r\n" +
    //     (std::string) "\n" +
    //     (std::string) "this is the message\n");

    // HTTP::Request test2;
    // test2.SetType(HTTP::POST);
    // test2.SetPath("/index.html");
    // test2.SetVersion(1.1);
    // test2.AddHeader("Host", "127.0.0.1:80");
    // test2.AddHeader("User-Agent", "Test/1234");
    // test2.AddHeader("Header1", "Value");
    // test2.AddHeader("Header2", "Value2");
    // test2.AddHeader("Header3", "Value3");
    // test2.AddHeader("Header4", "Value4");

    // test2.SetMessage("This is the message of the request line");

    // std::cout << test2.CreateRaw() << "\n\n\n";

    HTTP::Response test3(
        (std::string) "    HTTP/1.1 200    OK\r\n" +
        (std::string) "header1: value1\r\n" +
        (std::string) "header2: value2,\r\n" +
        (std::string) " value3\r\n" +
        (std::string) "\n" +
        (std::string) "this is the message\n");

    HTTP::Response test4;
    test4.SetVersion(1.0);
    test4.SetCode(200);

    test4.AddHeader("Host", "127.0.0.1:80");
    test4.AddHeader("User-Agent", "WIndows/1234");

    test4.SetMessage("this is the message");

    std::cout << test4.CreateRaw("OK") << "\n\n\n";

    return 0;
}
