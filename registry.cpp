#include <Windows.h>

#include <string>


using namespace std::literals;

#include "utf8.h"
#include "print.h"


void registryStuff()
{
    auto const welshKeyName      = u8"SOFTWARE\\_ỻ_welsh"s;                      //   or   u8"\u1efb"s      

    HKEY key{};
    DWORD disposition{};

    auto error = RegCreateKeyExA(HKEY_CURRENT_USER,
                                 utf8Bytes(welshKeyName),
                                 0,nullptr,
                                 0,
                                 KEY_ALL_ACCESS,
                                 nullptr,
                                 &key,
                                 &disposition);

    if(error != ERROR_SUCCESS)
    {
        print("RegCreateKeyExA failed {}\n",error);
        return;
    }


}