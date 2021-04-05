#include <chrono> 
#include "Metadata.h"
#include "Threadpool.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Not enought arguments!\n";
    }
    else
    {
        //Timer start
        auto start = std::chrono::high_resolution_clock::now();
        //Adding directory path
        Metadata md(argv[1]);
        //Adding extra extensions
        for (int i = 2; i < argc; ++i)
        {
            md.AddExtAsArg(argv[i]);
        }
        //Search for files with needed extensions
        md.SearchForFiles();
        //Passing found files
        Threadpool test(md.files);
        //Concurently calculate quantity of each line type
        test.Calculate();
        //test.PrintData();
        test.PrintToFile();
        //Timer end
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << duration.count() << " milliseconds\n";
    }
}
