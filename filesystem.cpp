#include <Windows.h>

#include <string>

#include <filesystem>
#include <fstream>

using namespace std::literals;

#include "utf8.h"
#include "print.h"



auto findFilesA()
{
    std::vector<std::string>    files;

    WIN32_FIND_DATAA file{};
    auto find=FindFirstFileA("*.welsh",&file);

    if(find!=INVALID_HANDLE_VALUE)
    {
        do
        {
            files.push_back( file.cFileName );
        } while(FindNextFileA(find,&file));

        FindClose(find);
    }

    return files;
}



auto findFilesW()
{
    std::vector<std::wstring>    files;

    WIN32_FIND_DATAW file{};
    auto find=FindFirstFileW(L"*.welsh",&file);

    if(find!=INVALID_HANDLE_VALUE)
    {
        do
        {
            files.push_back( file.cFileName );
        } while(FindNextFileW(find,&file));

        FindClose(find);
    }

    return files;
}


void cleanFiles()
{
    auto names = findFilesW();

    for(auto &name : names)
    {
        DeleteFileW(name.c_str());
    }


}


auto const welshFileName      = u8"_ỻ_.welsh"s;                      //   or   u8"\u1efb"s      


void createWithWindows()
{

    DeleteFileA(utf8Bytes(welshFileName));

    auto file = CreateFileA(utf8Bytes(welshFileName),
                            FILE_ALL_ACCESS,
                            0,
                            nullptr,
                            OPEN_ALWAYS,
                            0,nullptr);

    if(file == INVALID_HANDLE_VALUE)
    {
        print("CreateFileA failed {}\n",GetLastError());
    }

    CloseHandle(file);

}


void findWithWindows()
{

    auto names  = findFilesA();
    auto namesW = findFilesW();

    if(    names.size() != 1
       ||  namesW.size() != 1)
    {
        print("Wrong number of files\n");
    }
    else if(namesW[0].size() != 9)
    {
        print("Wide name isn't {}  length is {}, not 9\n",utf8Bytes(welshFileName), namesW[0].size());
    }
    else if(names[0].size() != 11)
    {
        print("Utf8 name isn't {}  length is {}, not 11\n",utf8Bytes(welshFileName), namesW[0].size());
    }
    else
    {
        print( "FindFirstFile   found {}\n",names[0]);
    }
}


void findWithFilesystem()
{
    for(auto &entry : std::filesystem::directory_iterator{std::filesystem::current_path()})
    {
        if(entry.path().extension() == ".welsh")
        {
            print("std::filesystem found {}\n",entry.path().filename().string());
        }
    }
}


void createWithFstream()
{
    DeleteFileA(utf8Bytes(welshFileName));

    std::ofstream   file{welshFileName};

    if(!file)
    {
        print("ofstream failed\n");
    }
}



void tryLong()
{
    auto const longWelshFileName  = u8"u8_ỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻ_.welsh"s;           // ~100 codepoints. ~308 bytes.  File operations will succeed. But FindFirstFileA will fail since name is too big for  char[260]
    auto const longWelshFileNameW =  L"L_ỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻỻ_.welsh"s;


    print("Length of {} = u8:{}  L:{}\n",utf8Bytes(longWelshFileName),longWelshFileName.size(), longWelshFileNameW.size());   // 308


    std::ofstream  u8{longWelshFileName};
    std::ofstream  L{longWelshFileNameW};


    if(!u8)
    {
        print("Create of u8 version failed\n");
    }

    if(!L)
    {
        print("Create of u8 version failed\n");
    }

    auto names = findFilesA();

    for(auto &name : names)
    {
        print("Found {}\n",name);
    }


}



void fileSystemStuff()
{

    cleanFiles();
    
    createWithWindows();
    findWithWindows();
    findWithFilesystem();

    cleanFiles();

    createWithFstream();
    findWithWindows();
    findWithFilesystem();

    cleanFiles();

    tryLong();
}