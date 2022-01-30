
#if defined UNICODE
#error("Compile with 'Use Multi-byte character set'");
#endif

static_assert(__cplusplus > 202002L) ;   // Or /Zc:__cplusplus hasn't been specified

#include <Windows.h>

#include <filesystem>
#include <string>
#include <fstream>

#include "print.h"


using namespace std::literals;

auto utf8Bytes(auto const &utf8String)
{
    return reinterpret_cast<char const*>(utf8String.c_str());
}


void consoleDetails()
{
    auto const console=GetStdHandle(STD_OUTPUT_HANDLE);

    print(L"Console codepage is : {}\n",    GetConsoleOutputCP());

    CONSOLE_FONT_INFOEX font{sizeof(font)};
    GetCurrentConsoleFontEx (console,FALSE,&font);

    print(L"Console typeface is : {}\n\n",font.FaceName);
}


void print()
{
    auto const console=GetStdHandle(STD_OUTPUT_HANDLE);
    auto const welsh  = u8"ỻ"s;                      //   or   u8"\u1efb"s      

    auto writeConsole = [&](auto const &str)
    {
        WriteConsoleA(console,
                      str.c_str(),
                      static_cast<DWORD>(str.size()),
                      nullptr,nullptr);
    };

    writeConsole("WriteConsole : "s);
    writeConsole(welsh);
    writeConsole("\n"s);

    printf("printf       : %s\n",utf8Bytes(welsh));
    std::cout  << "cout         : " << utf8Bytes(welsh)  << "\n";
    print("format       : {}\n",utf8Bytes(welsh));

    print("\n");
}


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


auto const welshFileName  = u8"_ỻ_.welsh"s;                      //   or   u8"\u1efb"s      


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


int main()
{
    SetConsoleOutputCP(CP_UTF8);

    consoleDetails();
    print();
    
    createWithWindows();
    findWithWindows();
    findWithFilesystem();


    createWithFstream();
    findWithWindows();
    findWithFilesystem();


}