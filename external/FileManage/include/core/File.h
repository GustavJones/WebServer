#pragma once
#include "Path.h"
#include "Folder.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace FileManage
{
    /**
     * @brief Simple class to manipulate files easily
     * @author Gustav Jones
     * @since Wed Jun 28 2023
     */
    class File
    {
    private:
        Path m_filePath;

    public:
        /**
         * @brief Creates a new file object to read, write, create, delete and more
         * @param __filePathStr path to a file on disk
         */
        File(Path __filePathStr);

        /**
         * @brief Gets the input path of a file object
         * @return std::string
         */
        std::string GetPath();

        /**
         * @brief Creates a new File if it doesn't exists
         * @return (void)
         */
        void Create();

        /**
         * @brief Deletes a File if it exists
         * @return (void)
         */
        void Delete();

        /**
         * @brief Reads a file contents
         * @return std::string
         */
        std::string ReadFile();

        /**
         * @brief Writes text to a file
         * @param __msg text to add to file
         * @return (void)
         */
        void WriteFile(std::string __msg);

        /**
         * @brief Renames a file to desired name
         * @param __name new file name
         * @return (void)
         */
        void RenameFile(std::string __name);
    };

} // namespace FileManage