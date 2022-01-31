

#if defined UNICODE
#error("Compile with 'Use Multi-byte character set'");
#endif

static_assert(__cplusplus > 202002L) ;   // Or /Zc:__cplusplus hasn't been specified

#include <Windows.h>
#include "utf8.h"



/*
ntdll!NlsActiveCodePageIsUTF8  : not exported

Read in
    RtlMultiByteToUnicodeSize
    RtlAnsiStringToUnicodeString
    RtlMultiByteToUnicodeN
    RtlUnicodeToMultiByteSize
    RtlUnicodeToMultiByteN
    RtlAnsiCharToUnicodeChar

    RtlpIsUtf8Process
        RtlpDidUnicodeToOemWork
        RtlUpcaseUnicodeToOemN
        RtlUnicodeToOemN
        RtlOemToUnicodeN
        RtlUpcaseUnicodeToMultiByteN
        RtlConsoleMultiByteToUnicodeN
        RtlIsValidOemCharacter

    LdrpLoadDependentModule

Written in

    RtlResetRtlTranslations         (2 spots)




ntdlL!NlsOemCodePageIsUTF8

Read in

    RtlpIsUtf8Process


Written in

    RtlResetRtlTranslations         (also 2 spots)  

*/








int main()
{
    SetConsoleOutputCP(CP_UTF8);

    consoleDetails();
    print();

    fileSystemStuff();
    registryStuff();

}