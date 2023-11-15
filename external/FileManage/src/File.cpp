#include "core/File.h"

using namespace FileManage;

File::File(Path __filePathStr) : m_filePath(__filePathStr)
{
}

std::string File::GetPath()
{
    return m_filePath.GetPath();
}

void File::Create()
{
    std::string path = m_filePath.GetPath();

    if (!std::filesystem::exists((m_filePath.GetDirectory()).GetPath()))
    {
        FileManage::Folder(m_filePath.GetDirectory()).CreateFolder();
    }

    if (!std::filesystem::exists(path))
    {
        std::ofstream file(path);
        file.close();
    }
}

void File::Delete()
{
    std::string path = m_filePath.GetPath();

    if (std::filesystem::exists(path))
    {
        std::filesystem::remove(path);
    }
}

/*std::string File::ReadFile()
{
    std::string path = m_filePath.GetPath();
    std::string output = "";

    if (std::filesystem::exists(path))
    {
        std::ifstream file(path, std::ifstream::binary);

        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        if (length == 0)
        {
            return output;
        }

        char *buffer = new char[length + 1];

        file.read(buffer, length + 1);

        output = (std::string)buffer;

        delete[] buffer;
        file.close();
    }
    return output;
}*/

std::string File::ReadFile()
{
    std::string path = m_filePath.GetPath();
    std::string line;
    std::string lines = "";

    std::ifstream file(path);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            lines.append('\n' + line);
        }
        file.close();
    }
    return lines;
}

void File::WriteFile(std::string __msg)
{
    std::string path = m_filePath.GetPath();

    if (std::filesystem::exists(path))
    {
        std::fstream file(path, std::ofstream::out | std::ofstream::app);
        file << __msg.c_str();
        file.close();
    }
}

void File::RenameFile(std::string __name)
{
    std::string path = m_filePath.GetPath();

    if (std::filesystem::exists(path))
    {
        std::filesystem::rename(path, __name);
    }
}