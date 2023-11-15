#include "core/Path.h"

using namespace FileManage;

Path::Path(std::string __path) : m_Path(__path) {}

Path::Path(const char *__path) : m_Path(__path) {}

std::string Path::GetPath()
{
    return m_Path;
}

void Path::SetPath(std::string __path)
{
    m_Path = __path;
}

Path Path::GetDirectory()
{
    if (m_Path.find('.') != -1)
    {
        int fileStartIndex = m_Path.find_last_of('/');

        return m_Path.substr(0, fileStartIndex);
    }
    else
        return m_Path;
}

Path Path::GetFileWithExtension()
{
    int fileStartIndex = m_Path.find_last_of('/') + 1;
    // std::cout << m_Path.substr(fileStartIndex + 1, m_Path.length() - fileStartIndex) << std::endl;

    return m_Path.substr(fileStartIndex, m_Path.length() - fileStartIndex);
}

Path Path::GetFileWithoutExtension()
{
    std::string fileName = this->GetFileWithExtension().GetPath();
    int extensionStartIndex = fileName.find_last_of('.');

    return fileName.substr(0, extensionStartIndex);
}

Path Path::GetExtension()
{
    std::string fileName = this->GetFileWithExtension().GetPath();

    int extensionStartIndex = fileName.find_last_of('.') + 1;
    if (extensionStartIndex != -1)
    {
        return fileName.substr(extensionStartIndex, fileName.length() - extensionStartIndex);
    }
    else
        return fileName;
}