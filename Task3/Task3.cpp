#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <sstream>
#include "Metadata.h"
#include "Threadpool.h"

int main()
{
    Metadata md;
    md.SearchForFiles();
    md.ReadFiles();
    //md.PrintFiles();
    Threadpool test(md.files);
    test.Calculate();
    test.PrintData();

}
