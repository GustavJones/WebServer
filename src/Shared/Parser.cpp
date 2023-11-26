#include "Parser.hpp"
#include <iostream>

HTTP::Version::Version()
    : major(0), minor(0)
{
}

HTTP::Version::Version(int _major, int _minor)
    : major(_major), minor(_minor)
{
}

HTTP::Version::~Version()
{
}

HTTP::Request::Request(char *_raw, int _len)
    : rawLen(_len), msgLen(0)
{
    m_output = new char[2];

    int uriStartPos, uriEndPos;
    int httpStartPos, httpEndPos;
    int headerKeyStartPos, headerKeyEndPos;
    int headerValueStartPos, headerValueEndPos;
    int prevNewlinePos = 0;

    int i = 0;

    for (int i = 0; i < _len; i++)
    {
        raw.push_back(_raw[i]);
    }

    // Convert LF to CRLF
    for (int i = 0; i < raw.size(); i++)
    {
        if (raw[i] == '\n' && raw[i - 1] != '\r')
        {
            raw.insert(raw.begin() + i, '\r');
        }
    }

    // Get Type e.g. GET/POST/HEAD
    i = 0;
    do
    {
        switch (raw[i])
        {
        case 'G':
            type = RequestType::GET;
            break;

        case 'P':
            type = RequestType::POST;
            break;

        case 'H':
            type = RequestType::HEAD;
            break;

        default:
            type = RequestType::UNKNOWN;
            break;
        }

        i++;
    } while (raw[i - 1] == ' ' || raw[i - 1] == '\n' || raw[i - 1] == '\r');

    // Get URI start position
    while (true)
    {
        if (raw[i] == '/')
        {
            uriStartPos = i;
            break;
        }
        i++;
    }

    // Get HTTP Version start && end position
    while (true)
    {
        if (raw[i] == 'H' && raw[i + 1] == 'T' && raw[i + 2] == 'T' && raw[i + 3] == 'P' && raw[i + 4] == '/')
        {
            httpStartPos = i;
            int temp = i;
            while (raw[temp] != ' ' && raw[temp] != '\r' && raw[temp + 1] != '\n')
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
        if (raw[i] != ' ')
        {
            uriEndPos = i;
            break;
        }
        i--;
    }

    // Path
    for (int i = uriStartPos; i < uriEndPos + 1; i++)
    {
        path += raw[i];
    }

    // Version
    std::string versionTemp;
    int versionTempPos;

    for (int i = httpStartPos; i < httpEndPos + 1; i++)
    {
        versionTemp += raw[i];
    }

    versionTemp.erase(0, 5);

    versionTempPos = versionTemp.find('.');

    version.major = std::stoi(versionTemp.substr(0, versionTempPos));
    version.minor = std::stoi(versionTemp.substr(versionTempPos + 1, versionTemp.length() - versionTempPos - 1));

    // Get Headers
    i = 0;
    while (i != raw.size())
    {
        // Seperate Lines
        if (raw[i] == '\r' && raw[i + 1] == '\n')
        {
            std::string line;
            for (int j = prevNewlinePos; j < i + 2; j++)
            {
                line += raw[j];
            }

            // Check if not in header lines
            if (prevNewlinePos == 0)
            {
                prevNewlinePos = i + 2; // + 2 for \r\n
                i++;
                continue;
            }
            else if (line == "\r\n")
            {
                i += 2;
                break;
            }

            // Check if new header line
            if (line[0] != ' ' || line.find(':') != -1)
            {
                // Get Header Key start && end pos
                int temp = 0;
                while (line[temp] == ' ')
                {
                    temp++;
                }

                headerKeyStartPos = temp;

                while (line[temp] != ' ' && line[temp] != ':')
                {
                    temp++;
                }

                headerKeyEndPos = temp - 1;

                // Get Header Value start && end pos
                while (line[temp] == ':' || line[temp] == ' ')
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
                headers.emplace_back(Header(headerKey, headerValue));
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

                headers[headers.size() - 1].second = headers[headers.size() - 1].second + headerValue;
            }

            // Update Line beginning
            prevNewlinePos = i + 2;
        }

        if (raw[i] == '\r' && raw[i + 1] == '\n' && raw[i + 2] == '\r' && raw[i + 3] == '\n')
        {
            i += 4;
            break;
        }

        i++;
    }

    // Get Message
    while (i < raw.size())
    {
        msg.push_back(raw[i]);
        msgLen++;
        i++;
    }
}

HTTP::Request::Request()
    : rawLen(0), msgLen(0)
{
    m_output = new char[2];
}

char *HTTP::Request::CreateRaw()
{
    // Add Request type
    std::string temp;
    raw.clear();

    switch (type)
    {
    case RequestType::GET:
        temp = "GET ";
        for (char c : temp)
        {
            raw.push_back(c);
        }

        break;

    case RequestType::POST:
        temp = "POST ";
        for (char c : temp)
        {
            raw.push_back(c);
        }
        break;

    case RequestType::HEAD:
        temp = "HEAD ";
        for (char c : temp)
        {
            raw.push_back(c);
        }
        break;

    default:
        return nullptr;
        break;
    }

    for (char c : path)
    {
        raw.push_back(c);
    }
    raw.push_back(' ');

    for (char c : "HTTP/")
    {
        if (c == 0)
        {
            break;
        }

        raw.push_back(c);
    }
    std::string major, minor;
    major = std::to_string(version.major);
    minor = std::to_string(version.minor);

    for (char c : major)
    {
        raw.push_back(c);
    }

    raw.push_back('.');

    for (char c : minor)
    {
        raw.push_back(c);
    }
    raw.push_back('\r');
    raw.push_back('\n');

    for (const Header header : headers)
    {
        for (char c : header.first)
        {
            raw.push_back(c);
        }

        raw.push_back(':');
        raw.push_back(' ');

        for (char c : header.second)
        {
            raw.push_back(c);
        }

        raw.push_back('\r');
        raw.push_back('\n');
    }

    raw.push_back('\r');
    raw.push_back('\n');

    for (char c : msg)
    {
        raw.push_back(c);
    }

    delete[] m_output;
    m_output = new char[raw.size() + 1];

    for (int i = 0; i < raw.size(); i++)
    {
        m_output[i] = raw[i];
    }

    m_output[raw.size()] = 0;

    return m_output;
}

void HTTP::Request::AddHeader(std::string _key, std::string _value)
{
    headers.emplace_back(Header(_key, _value));
}

char *HTTP::Request::GetRaw()
{
    delete[] m_output;
    m_output = new char[raw.size() + 1];

    for (int i = 0; i < raw.size(); i++)
    {
        m_output[i] = raw[i];
    }

    m_output[raw.size()] = 0;

    return m_output;
}

char *HTTP::Request::GetMsg()
{
    delete m_output;
    m_output = new char[msg.size() + 1];

    for (int i = 0; i < msg.size(); i++)
    {
        m_output[i] = msg[i];
    }

    m_output[msg.size()] = 0;

    return m_output;
}

void HTTP::Request::SetRaw(char *_raw, int _len)
{
    raw.clear();

    for (int i = 0; i < _len; i++)
    {
        raw.push_back(_raw[i]);
    }

    rawLen = _len;
}

void HTTP::Request::SetMsg(char *_msg, int _len)
{
    msg.clear();

    for (int i = 0; i < _len; i++)
    {
        msg.push_back(_msg[i]);
    }

    msgLen = _len;
}

HTTP::Request::~Request()
{
    delete[] m_output;
}

HTTP::Response::Response(char *_raw, int _len)
    : rawLen(_len), msgLen(0)
{
    m_output = new char[2];

    int i = 0;
    int versionStartPos, versionEndPos, versionPeriodPos;
    int codeStartPos, codeEndPos;
    int headerKeyStartPos, headerKeyEndPos;
    int headerValueStartPos, headerValueEndPos;

    for (int i = 0; i < _len; i++)
    {
        raw.push_back(_raw[i]);
    }

    // Convert LF to CRLF
    for (int i = 0; i < raw.size(); i++)
    {
        if (raw[i] == '\n' && raw[i - 1] != '\r')
        {
            raw.insert(raw.begin() + i, '\r');
        }
    }

    // Get HTTP Version
    while (true)
    {
        if (raw[i] == 'H' && raw[i + 1] == 'T' && raw[i + 2] == 'T' && raw[i + 3] == 'P' && raw[i + 4] == '/')
        {
            versionStartPos = i + 5;

            for (int j = versionStartPos; j < raw.size(); j++)
            {
                if (raw[j] == ' ')
                {
                    versionEndPos = j - 1;
                    break;
                }
            }

            break;
        }
        i++;
    }

    while (i < versionEndPos)
    {
        if (raw[i] == '.')
        {
            versionPeriodPos = i;
            break;
        }
        i++;
    }

    i = versionEndPos + 1;

    int major, minor;
    std::string versionTemp;

    for (int j = versionStartPos; j < versionPeriodPos; j++)
    {
        versionTemp += raw[j];
    }

    major = std::stoi(versionTemp);
    versionTemp = "";

    for (int j = versionPeriodPos + 1; j < versionEndPos + 1; j++)
    {
        versionTemp += raw[j];
    }

    minor = std::stoi(versionTemp);

    version.major = major;
    version.major = minor;

    // Get Code
    while (raw[i] == ' ')
    {
        i++;
    }

    codeStartPos = i;

    while (raw[i] != ' ' && raw[i] != '\r' && raw[i + 1] != '\n')
    {
        i++;
    }
    codeEndPos = i - 1;

    std::string codeStr;

    for (int j = codeStartPos; j < codeEndPos + 1; j++)
    {
        codeStr += raw[j];
    }

    code = std::stoi(codeStr);

    // Get Individual lines
    int prevNewlinePos = 0;

    // Get Headers
    i = 0;
    while (i != raw.size())
    {
        // Seperate Lines
        if (raw[i] == '\r' && raw[i + 1] == '\n')
        {
            std::string line;

            for (int j = prevNewlinePos; j < i + 2; j++)
            {
                line += raw[j];
            }

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
            if (line[0] != ' ' || line.find(':') != -1)
            {
                // Get Header Key start && end pos
                int temp = 0;
                while (line[temp] == ' ')
                {
                    temp++;
                }

                headerKeyStartPos = temp;

                while (line[temp] != ' ' && line[temp] != ':')
                {
                    temp++;
                }

                headerKeyEndPos = temp - 1;

                // Get Header Value start && end pos
                while (line[temp] == ':' || line[temp] == ' ')
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
                headers.emplace_back(Header(headerKey, headerValue));
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

                headers[headers.size() - 1].second = headers[headers.size() - 1].second + headerValue;
            }

            // Update Line beginning
            prevNewlinePos = i + 2;
        }

        if (raw[i] == '\r' && raw[i + 1] == '\n' && raw[i + 2] == '\r' && raw[i + 3] == '\n')
        {
            break;
        }

        i++;
    }

    // Get Message
    while (i < raw.size())
    {
        msg.push_back(raw[i]);
        i++;
    }
}

HTTP::Response::Response()
    : rawLen(0), msgLen(0)
{
    m_output = new char[2];
}

char *HTTP::Response::CreateRaw(const char *_responseDescription)
{
    std::string versionStr = std::to_string(version.major) + '.' + std::to_string(version.minor);
    raw.clear();

    for (char c : "HTTP/" + versionStr + ' ')
    {
        raw.push_back(c);
    }

    for (char c : std::to_string(code) + ' ' + _responseDescription + "\r\n")
    {
        raw.push_back(c);
    }

    for (const Header header : headers)
    {
        for (char c : header.first + ": " + header.second + "\r\n")
        {
            raw.push_back(c);
        }
    }

    raw.push_back('\r');
    raw.push_back('\n');

    for (char c : msg)
    {
        raw.push_back(c);
    }

    delete[] m_output;
    m_output = new char[raw.size() + 1];

    for (int i = 0; i < raw.size(); i++)
    {
        m_output[i] = raw[i];
    }

    m_output[raw.size()] = 0;

    return m_output;
}

void HTTP::Response::AddHeader(std::string _key, std::string _value)
{
    headers.emplace_back(Header(_key, _value));
}

char *HTTP::Response::GetRaw()
{
    delete[] m_output;
    m_output = new char[raw.size() + 1];

    for (int i = 0; i < raw.size(); i++)
    {
        m_output[i] = raw[i];
    }

    m_output[raw.size()] = 0;

    return m_output;
}

char *HTTP::Response::GetMsg()
{
    delete m_output;
    m_output = new char[msg.size() + 1];

    for (int i = 0; i < msg.size(); i++)
    {
        m_output[i] = msg[i];
    }

    m_output[msg.size()] = 0;

    return m_output;
}

void HTTP::Response::SetRaw(char *_raw, int _len)
{
    raw.clear();

    for (int i = 0; i < _len; i++)
    {
        raw.push_back(_raw[i]);
    }

    rawLen = _len;
}

void HTTP::Response::SetMsg(char *_msg, int _len)
{
    msg.clear();

    for (int i = 0; i < _len; i++)
    {
        msg.push_back(_msg[i]);
    }

    msgLen = _len;
}

HTTP::Response::~Response()
{
    delete[] m_output;
}
