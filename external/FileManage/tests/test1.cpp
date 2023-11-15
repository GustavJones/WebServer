#include "FileManage.h"

int main()
{
    FileManage::File TestFile((FileManage::Path) "Test1.txt");
    TestFile.Create();
    TestFile.GetPath();
    std::cout << TestFile.ReadFile() << std::endl;

    return 0;
}