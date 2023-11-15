#include "FileManage.h"
#include <iostream>

int main()
{
    FileManage::Folder TestFolder((FileManage::Path) "TestFolder");
    TestFolder.CreateFolder();
    // TestFolder.DeleteFolder();
    TestFolder.RenameFolder("TestRename");

    std::cout << "Program Ended" << std::endl;
    return 0;
}