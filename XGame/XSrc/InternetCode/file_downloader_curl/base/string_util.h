// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Windows Header Files:
#include <tchar.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#if !defined(tstring)
# if defined(UNICODE) || defined(_UNICODE)
#    define tstring std::wstring
#else
#   define tstring std::string
# endif
#endif

#if !defined(tostringstream)
# if defined(UNICODE) || defined(_UNICODE)
#   define tostringstream std::wostringstream
# else
#   define tostringstream std::ostringstream
# endif
#endif

#if !defined(tstringstream)
# if defined(UNICODE) || defined(_UNICODE)
#   define tstringstream std::wstringstream
# else
#   define tstringstream std::stringstream
# endif
#endif

namespace base {

class StringUtil
{
public:
    static tstring& Trim(tstring &s, const TCHAR * lpTrimChars = _T(" \t\r\n"));
    static bool StringSplit( const tstring &str, const tstring &delim, std::vector<tstring>& result );
    //ANSI<==>UNIODE
    static std::string  WstringToString( const std::wstring &s );
    static std::wstring StringToWstring( const std::string &s );
    static std::string  StringUtil::ToString( const tstring &s );
    static std::wstring TstringToWstring( const tstring &s );
    static tstring      StringUtil::ToString( const std::string &s );
    static tstring      WstringToTstring( const std::wstring &s );
};

} //namespacee base