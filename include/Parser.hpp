#pragma once
#include <string>
#include <vector>
#include <map>

namespace HTTP
{
    enum RequestType
    {
        GET,
        POST,
        HEAD,
        UNKNOWN
    };

    class Request
    {
    private:
        std::string m_rawStr;
        RequestType m_reqType;
        std::string m_uriPath;
        double m_httpVersion;
        std::map<std::string, std::string> m_headers;
        std::string m_message;

    public:
        Request();
        Request(std::string _rawStr);

        std::string CreateRaw();
        void AddHeader(std::string _key, std::string _value);

        std::string GetRaw();
        RequestType GetType();
        std::string GetPath();
        double GetVersion();
        std::map<std::string, std::string> GetHeaders();
        std::string GetMessage();

        void SetRaw(std::string _rawStr);
        void SetType(RequestType _reqType);
        void SetPath(std::string _uriPath);
        void SetVersion(double _httpVersion);
        void SetHeaders(std::map<std::string, std::string> _headers);
        void SetMessage(std::string _message);

        ~Request();
    };
} // namespace HTTP
