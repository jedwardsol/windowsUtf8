#include <Windows.h>

#include <string>

using namespace std::literals;

#include "utf8.h"
#include "print.h"




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

