#pragma once 
#include <string>

// A utf8 string literal ( u8"..." )  has type `char8_t const*`  

// So a utf8 string ( u8"..."s ) has stype  std::basic_string<char8_t>  which aren't implicitly convertable to std::string




inline [[nodiscard]]
auto utf8Bytes(std::basic_string<char8_t> const &utf8String)
{
    return reinterpret_cast<char const*>(utf8String.c_str());
}





void consoleDetails();
void print();

void fileSystemStuff();
void registryStuff();
