#include "FileManage.h"
#include <iostream>

int main()
{
    FileManage::Path TestPath("C:/CXXLibs/FileManage/tests/test3.cpp");
    std::cout << TestPath.GetDirectory().GetPath() << std::endl;
    std::cout << TestPath.GetFileWithoutExtension().GetPath() << std::endl;
    std::cout << TestPath.GetFileWithExtension().GetPath() << std::endl;
    std::cout << TestPath.GetExtension().GetPath() << std::endl;
    return 0;
}