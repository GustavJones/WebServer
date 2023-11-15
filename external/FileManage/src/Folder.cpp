#include "core/Folder.h"

using namespace FileManage;

Folder::Folder(Path __directoryPath) : m_Path(__directoryPath.GetDirectory())
{
}

Path Folder::GetPath()
{
    return m_Path.GetPath();
}

void Folder::CreateFolder()
{
    std::filesystem::create_directory(m_Path.GetPath());
}

void Folder::DeleteFolder()
{
    std::filesystem::remove(m_Path.GetPath());
}

void Folder::RenameFolder(std::string __str)
{
    int originalFolderIndex = m_Path.GetPath().find_last_of('/') + 1;
    std::string newPath = m_Path.GetPath().substr(0, originalFolderIndex).append(__str);

    std::filesystem::rename(m_Path.GetPath(), newPath);
    m_Path.SetPath(newPath);
}