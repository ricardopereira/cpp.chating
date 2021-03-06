#pragma once
#include <Windows.h>
#include <string>
#include <tchar.h>
#include <sstream>

typedef std::basic_string<TCHAR> sTchar_t;
typedef std::basic_ostringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> oTcharStream_t;
