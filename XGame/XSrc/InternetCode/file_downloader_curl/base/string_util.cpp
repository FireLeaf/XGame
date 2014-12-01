#include "string_util.h"
#include <Windows.h>

namespace base
{
namespace Internal
{
    inline UINT MiGetConversionACP() throw()
    {
    #ifdef _CONVERSION_DONT_USE_THREAD_LOCALE
        return CP_ACP;
    #else
        return CP_THREAD_ACP;
    #endif
    }

    LPWSTR MiC2W(LPCSTR lp)
    {
        int nWLen = 0;
        wchar_t *pWStr = NULL;

        nWLen = MultiByteToWideChar(MiGetConversionACP(), 0, lp, -1, NULL, 0);
        pWStr = (wchar_t *)malloc(nWLen * sizeof(wchar_t));
        nWLen = MultiByteToWideChar(MiGetConversionACP(), 0, lp, -1, pWStr, nWLen);

        return pWStr;
    }

    LPSTR MiW2C(LPCWSTR lp)
    {
        int nALen = 0;
        char *pAStr = NULL;

        nALen = WideCharToMultiByte(MiGetConversionACP(), 0, lp, -1, NULL, 0, NULL, NULL);
        pAStr = (char*)malloc(nALen * sizeof(char));
        WideCharToMultiByte(MiGetConversionACP(), 0, lp, -1, pAStr, nALen, NULL, NULL);

        return pAStr;
    }

    LPWSTR MiUE2W(LPCSTR lp)
    {
        int nWLen = 0;
        wchar_t *pWStr = NULL;

        nWLen = MultiByteToWideChar(CP_UTF8, 0, lp, -1, NULL, 0);
        pWStr = (wchar_t *)malloc(nWLen * sizeof(wchar_t));
        nWLen = MultiByteToWideChar(CP_UTF8, 0, lp, -1, pWStr, nWLen);

        return pWStr;
    }

    LPSTR MiW2UE(LPCWSTR lp)
    {
        int nALen = 0;
        char *pAStr = NULL;

        nALen = WideCharToMultiByte(CP_UTF8, 0, lp, -1, NULL, 0, NULL, NULL);
        pAStr = (char*)malloc(nALen * sizeof(char));
        WideCharToMultiByte(CP_UTF8, 0, lp, -1, pAStr, nALen, NULL, NULL);

        return pAStr;
    }

    LPSTR MiUE2C(LPCSTR lp)
    {
        LPWSTR lpW = MiUE2W(lp);
        if(lpW == NULL)
            return NULL;
        LPSTR lpRet = MiW2C(lpW);
        free(lpW);
        return lpRet;
    }

    LPSTR MiC2UE(LPCSTR lp)
    {
        LPWSTR lpW = MiC2W(lp);
        if(lpW == NULL)
            return NULL;
        LPSTR lpRet = MiW2UE(lpW);
        free(lpW);
        return lpRet;
    }

    LPWSTR MiW2W(LPCWSTR lp)
    {
        if(lp == NULL)
            return NULL;
        wchar_t *pWStr = (wchar_t *)malloc( (wcslen(lp) + 1) * sizeof(wchar_t) );
        wcscpy_s(pWStr, (wcslen(lp) + 1), lp);
        return pWStr;
    }

    LPSTR MiC2C(LPCSTR lp)
    {
        if(lp == NULL)
            return NULL;
        char *pAStr = (char *)malloc( (strlen(lp) + 1) * sizeof(char) );
        strcpy_s(pAStr, (strlen(lp) + 1), lp);
        return pAStr;
    }
  } //namespace internal
} // namespace base

using namespace base;

tstring& StringUtil::Trim(tstring &s, const TCHAR * lpTrimChars)
{
    std::string::size_type left = s.find_first_not_of( lpTrimChars );
    if ( left == tstring::npos )
    {
        return s;
    }
    s.erase(0, left - 0);

   tstring::size_type right = s.find_last_not_of( lpTrimChars );
   if ( right == tstring::npos )
   {
       return s;
    }
   s.erase(right+1);
   return s;
}


bool StringUtil::StringSplit( const tstring &str, const tstring &delim, std::vector<tstring>& result )
{
    tstring::size_type pos = 0;
    tstring::size_type length = str.length( );
    tstring elt;

    TCHAR quoteChar = _T('\0');
    while( pos < length ){
        if ( quoteChar == _T('\0') &&( str[pos] == _T('"') || str[pos] == _T('\'') ) ){
            quoteChar = str[pos++];
            continue; // Skip the quote
        }
        else if ( quoteChar == _T('\0') && str[pos] == L'\\' && pos + 1 < length &&
           ( str[pos + 1] == _T('\'') || str[pos + 1] == _T('"') ) ){
                ++pos;
        }
        else if ( quoteChar != _T('\0') && str[pos] == L'\\' && pos + 1 < length && str[pos + 1] == quoteChar ){
            ++pos;
        }
        else if ( quoteChar != _T('\0') && str[pos] == quoteChar ){
            ++pos;
            quoteChar = _T('\0');
            continue; // Skip the end quote
        }
        else if ( delim.find( str[pos] ) != tstring::npos ){
            if ( quoteChar == _T('\0') ){
                ++pos;
                if ( elt.length( ) > 0 ){
                    result.push_back( elt );
                    elt = L"";
                }
                continue;
            }
        }

        if ( pos < length ){
            elt += str[pos++];
        }
    }

    if ( elt.length( ) > 0 ){
        result.push_back( elt );
    }
    if ( quoteChar != _T('\0') ){
        return false; // Unmatched quote.
    }
    return true;
}

std::string StringUtil::WstringToString(const std::wstring &s)
{
    char* szA = Internal::MiW2C( s.c_str() );
    std::string s_a = szA;
    free( szA );
    return s_a;
}

std::wstring StringUtil::StringToWstring(const std::string &s)
{
    wchar_t* szW = Internal::MiC2W( s.c_str() );
    std::wstring s_w = szW;
    free( szW );
    return s_w;
}

std::string StringUtil::ToString(const tstring &s)
{
#if defined(UNICODE) || defined(_UNICODE)
    return WstringToString( s );
#else
    return s;
#endif
}

std::wstring StringUtil::TstringToWstring(const tstring &s)
{
#if defined(UNICODE) || defined(_UNICODE)
    return s;
#else
    return StringToWstring( s );
#endif
}

tstring StringUtil::ToString(const std::string &s)
{
#if defined(UNICODE) || defined(_UNICODE)
    return StringToWstring(s);
#else
    return s;
#endif
}

tstring StringUtil::WstringToTstring(const std::wstring &s)
{
#if defined(UNICODE) || defined(_UNICODE)
    return s;
#else
    return WstringToString( s );
#endif
}