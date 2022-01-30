
#if defined UNICODE
#error("Compile with 'Use Multi-byte character set'");
#endif

static_assert(__cplusplus > 202002L) ;   // Or /Zc:__cplusplus hasn't been specified

#include <Windows.h>

#include <filesystem>
#include <string>

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




void windowsAPI()
{
    auto const welsh  = u8"_ỻ_.welsh"s;                      //   or   u8"\u1efb"s      

    DeleteFileA(utf8Bytes(welsh));

    auto file = CreateFileA(utf8Bytes(welsh),
                            FILE_ALL_ACCESS,
                            0,
                            nullptr,
                            OPEN_ALWAYS,
                            0,nullptr);

    if(file == INVALID_HANDLE_VALUE)
    {
        print("CreateFileA failed {}\n",GetLastError());
        return;
    }

    auto names  = findFilesA();
    auto namesW = findFilesW();

    if(    names.size() != 1
       ||  namesW.size() != 1
       ||  namesW[0].size() != 9)
    {
        print("Oops");
    }

    print( "Found utf8 {}\n",names[0]);


    return;
}


int main()
{
    consoleDetails();
    print();
    windowsAPI();
}