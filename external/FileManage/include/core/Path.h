#pragma once
#include <string>

namespace FileManage
{
    /**
     * @brief Class to safely store paths and manipulate them easily
     * @author Gustav Jones
     * @since Wed Jun 28 2023
     */
    class Path
    {
    private:
        std::string m_Path;

    public:
        /**
         * @brief Create a new Path object
         * @param __path string object with "/" seperator
         */
        Path(std::string __path);

        /**
         * @brief Create a new Path object
         * @param __path const char * with "/" seperator
         */
        Path(const char *__path);

        /**
         * @brief Returns the stored path string
         * @return std::string path
         */
        std::string GetPath();

        /**
         * @brief Set the path string of the object
         * @param __path string path with "/" seperator
         * @return (void)
         */
        void SetPath(std::string __path);

        /**
         * @brief Remove the file from the path
         * @return FileManage::Path directory without a file
         */
        Path GetDirectory();

        /**
         * @brief Extract the file name from a path with it's extension
         * @return FileManage::Path name of the file
         */
        Path GetFileWithExtension();

        /**
         * @brief Extract the file name from a path without it's extension
         * @return FileManage::Path name of the file
         */
        Path GetFileWithoutExtension();

        /**
         * @brief Extract the extension from the file
         * @return FileManage::Path extension of the file without "."
         */
        Path GetExtension();
    };

} // namespace FileManage
