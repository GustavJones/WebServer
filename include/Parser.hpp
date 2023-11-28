#pragma once
#include <string>
#include <vector>
#include <map>

namespace HTTP
{
    typedef std::pair<std::string, std::string> Header;

    enum RequestType
    {
        GET,
        POST,
        HEAD,
        UNKNOWN
    };

    class Version
    {
    public:
        Version();
        Version(int _major, int _minor);

        int major, minor;
        ~Version();
    };

    class Request
    {
    private:
        char *m_output;

    public:
        Request(char *_raw, int _len);
        Request();

        char *CreateRaw();
        void AddHeader(std::string _key, std::string _value);

        std::vector<char> raw;
        int rawLen;
        RequestType type;
        std::string path;
        Version version;
        std::vector<Header> headers;
        std::vector<char> msg;
        int msgLen;

        char *GetRaw();
        char *GetMsg();

        void SetRaw(char *_raw, int _len);
        void SetMsg(char *_msg, int _len);

        ~Request();
    };

    class Response
    {
    private:
        char *m_output;

    public:
        Response(char *_raw, int _len);
        Response();

        char *CreateRaw(const char *_responseDescription);
        void AddHeader(std::string _key, std::string _value);

        std::vector<char> raw;
        int rawLen;
        int code;
        Version version;
        std::vector<Header> headers;
        std::vector<char> msg;
        int msgLen;

        char *GetRaw();
        char *GetMsg();

        void SetRaw(char *_raw, int _len);
        void SetMsg(char *_msg, int _len);

        ~Response();
    };
} // namespace HTTP
