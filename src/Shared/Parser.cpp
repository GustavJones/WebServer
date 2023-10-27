#include "Parser.hpp"
#include <string>
#include <iostream>
#include <cmath>

namespace HTTP
{
    Request::Request()
        : m_rawStr(""), m_reqType(RequestType::UNKNOWN),
          m_uriPath("/"), m_httpVersion(0.0),
          m_headers(std::map<std::string, std::string>()),
          m_message("")
    {
    }

    Request::Request(std::string _rawStr) : m_rawStr(_rawStr)
    {
        int i = 0;
        int uriStartPos, uriEndPos;
        int httpStartPos, httpEndPos;
        int headerKeyStartPos, headerKeyEndPos;
        int headerValueStartPos, headerValueEndPos;

        // Convert LF to CRLF
        for (int i = 0; i < m_rawStr.length(); i++)
        {
            if (m_rawStr[i] == '\n' and m_rawStr[i - 1] != '\r')
            {
                m_rawStr.insert(i, "\r");
            }
        }

        // Get Type e.g. GET/POST/HEAD
        i = 0;
        do
        {
            switch (m_rawStr[i])
            {
            case 'G':
                m_reqType = RequestType::GET;
                break;

            case 'P':
                m_reqType = RequestType::POST;
                break;

            case 'H':
                m_reqType = RequestType::HEAD;
                break;

            default:
                m_reqType = RequestType::UNKNOWN;
                break;
            }

            i++;
        } while (m_rawStr[i - 1] == ' ' or m_rawStr[i - 1] == '\n' or m_rawStr[i - 1] == '\r');

        // Get URI start position
        while (true)
        {
            if (m_rawStr[i] == '/' or m_rawStr.substr(i, 4) == "http")
            {
                uriStartPos = i;
                break;
            }
            i++;
        }

        // Get HTTP Version start and end position
        while (true)
        {
            if (m_rawStr.substr(i, 5) == "HTTP/")
            {
                httpStartPos = i;
                int temp = i;
                while (m_rawStr[temp] != ' ' and m_rawStr[temp] != '\r' and m_rawStr[temp + 1] != '\n')
                {
                    temp++;
                }
                httpEndPos = temp - 1;

                break;
            }
            i++;
        }

        // Get URI end position
        i--;
        while (true)
        {
            if (m_rawStr[i] != ' ')
            {
                uriEndPos = i;
                break;
            }
            i--;
        }

        m_uriPath = m_rawStr.substr(uriStartPos, uriEndPos - uriStartPos + 1);
        m_httpVersion = std::stod(m_rawStr.substr(httpStartPos + 5, httpEndPos - httpStartPos - 5 + 1));

        // Get Individual lines
        int prevNewlinePos = 0;

        std::vector<std::pair<std::string, std::string>> headersTemp;

        // Get Headers
        i = 0;
        while (true)
        {
            // Seperate Lines
            if (m_rawStr.substr(i, 2) == "\r\n")
            {
                std::string line = m_rawStr.substr(prevNewlinePos, i - prevNewlinePos + 2);

                // Check if not in header lines
                if (prevNewlinePos == 0)
                {
                    prevNewlinePos = i + 2;
                    i++;
                    continue;
                }
                else if (line == "\r\n")
                {
                    i += 2;
                    break;
                }

                // Check if new header line
                if (line[0] != ' ' or line.find(':') != -1)
                {
                    // Get Header Key start and end pos
                    int temp = 0;
                    while (line[temp] == ' ')
                    {
                        temp++;
                    }

                    headerKeyStartPos = temp;

                    while (line[temp] != ' ' and line[temp] != ':')
                    {
                        temp++;
                    }

                    headerKeyEndPos = temp - 1;

                    // Get Header Value start and end pos
                    while (line[temp] == ':' or line[temp] == ' ')
                    {
                        temp++;
                    }

                    headerValueStartPos = temp;

                    while (line.substr(temp, 2) != "\r\n")
                    {
                        temp++;
                    }

                    headerValueEndPos = temp - 1;

                    // Get Headers substr
                    std::string headerKey = line.substr(headerKeyStartPos, headerKeyEndPos - headerKeyStartPos + 1);
                    std::string headerValue = line.substr(headerValueStartPos, headerValueEndPos - headerValueStartPos + 1);

                    // Add headers to a vector
                    headersTemp.emplace_back(std::pair<std::string, std::string>(headerKey, headerValue));
                }
                else
                {
                    int temp = 0;
                    headerValueStartPos = temp;

                    while (line.substr(temp, 2) != "\r\n")
                    {
                        temp++;
                    }
                    headerValueEndPos = temp - 1;

                    std::string headerValue = line.substr(headerValueStartPos, headerValueEndPos - headerValueStartPos + 1);

                    headersTemp[headersTemp.size() - 1].second = headersTemp[headersTemp.size() - 1].second + headerValue;
                }

                // Update Line beginning
                prevNewlinePos = i + 2;
            }

            i++;
        }

        // Add header vector to header map
        for (const std::pair<std::string, std::string> header : headersTemp)
        {
            m_headers.emplace(header);
        }

        // Get Message
        while (i < m_rawStr.length())
        {
            m_message = m_message + m_rawStr[i];
            i++;
        }
    }

    std::string Request::CreateRaw()
    {
        // Add Request type
        switch (m_reqType)
        {
        case RequestType::GET:
            m_rawStr = "GET ";
            break;

        case RequestType::POST:
            m_rawStr = "POST ";
            break;

        case RequestType::HEAD:
            m_rawStr = "HEAD ";
            break;

        default:
            return "";
            break;
        }

        m_rawStr = m_rawStr + m_uriPath + " ";
        std::string version = std::to_string(m_httpVersion);
        m_rawStr = m_rawStr + "HTTP/" + version.substr(0, 3) + "\r\n";

        for (const std::pair<std::string, std::string> header : m_headers)
        {
            m_rawStr = m_rawStr + header.first + ": " + header.second + "\r\n";
        }

        m_rawStr = m_rawStr + "\r\n";
        m_rawStr = m_rawStr + m_message;

        return m_rawStr;
    }

    void Request::AddHeader(std::string _key, std::string _value)
    {
        std::pair<std::string, std::string> temp(_key, _value);

        m_headers.insert(temp);
    }

    std::string Request::GetRaw()
    {
        return m_rawStr;
    }

    RequestType Request::GetType()
    {
        return m_reqType;
    }

    std::string Request::GetPath()
    {
        return m_uriPath;
    }

    double Request::GetVersion()
    {
        return m_httpVersion;
    }

    std::map<std::string, std::string> Request::GetHeaders()
    {
        return m_headers;
    }

    std::string Request::GetMessage()
    {
        return m_message;
    }

    void Request::SetRaw(std::string _rawStr)
    {
        m_rawStr = _rawStr;
    }

    void Request::SetType(RequestType _reqType)
    {
        m_reqType = _reqType;
    }

    void Request::SetPath(std::string _uriPath)
    {
        m_uriPath = _uriPath;
    }

    void Request::SetVersion(double _httpVersion)
    {
        m_httpVersion = _httpVersion;
    }

    void Request::SetHeaders(std::map<std::string, std::string> _headers)
    {
        m_headers = _headers;
    }

    void Request::SetMessage(std::string _message)
    {
        m_message = _message;
    }

    Request::~Request()
    {
    }
} // namespace HTTP
