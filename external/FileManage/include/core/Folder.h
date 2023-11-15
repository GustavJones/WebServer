#pragma once
#include "Path.h"
#include <filesystem>

namespace FileManage
{
    /**
     * @brief Class to manipulate folders
     * @author Gustav Jones
     * @since Wed Jun 28 2023
     */
    class Folder
    {
    private:
        Path m_Path;

    public:
        /**
         * @brief Creates a Folder object
         * @param __directoryPath Path of the folder
         */
        Folder(Path __directoryPath);

        /**
         * @brief Returns the path of the Folder object
         * @return FileManage::Path path of the folder
         */
        Path GetPath();

        /**
         * @brief Creates a folder at the folder path
         * @return (void)
         */
        void CreateFolder();

        /**
         * @brief Deletes the folder at the folder path
         * @return (void)
         */
        void DeleteFolder();

        /**
         * @brief Renames the folder at the folder path
         * @param __str folder new name
         * @return (void)
         */
        void RenameFolder(std::string __str);
    };

} // namespace FileManage