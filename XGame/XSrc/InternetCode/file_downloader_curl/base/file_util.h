// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include <Windows.h>
#include <vector>
#include <fstream>

namespace base {

#if !defined(tfstream)
# if defined(UNICODE) || defined(_UNICODE)
#   define tfstream std::wfstream
# else
#   define tfstream std::fstream
# endif
#endif

#if !defined(tifiletream)
# if defined(UNICODE) || defined(_UNICODE)
#   define tifiletream std::wifstream
# else
#   define tifiletream std::ifstream
# endif
#endif

#if !defined(tofiletream)
# if defined(UNICODE) || defined(_UNICODE)
#   define tofiletream std::wofstream
# else
#   define tofiletream std::ofstream
# endif
#endif

class FileUtil
{
public:
  // File Operation
    static BOOL IsFileExists(LPCTSTR lpFile);
    static BOOL DeleteFile(LPCTSTR lpFile);
    static LONG GetFileSize(LPCTSTR lpFile);

    // Directory Operation
    static BOOL IsDirectoryExists(LPCTSTR lpDir);
    static BOOL CreateDirectory(LPCTSTR lpDir);

    // Path Operation
    static BOOL IsSlash(TCHAR c);
    static TCHAR GetDefaultSlash();
    static LPCTSTR GetSlashs();
    static BOOL IsAbsolutePath(LPCTSTR lpPath);

    static BOOL DeletePath(LPCTSTR lpPath, BOOL bSilent = TRUE);

    static BOOL SplitePath(LPCTSTR lpPath, LPTSTR lpDrive, DWORD cchDrive, LPTSTR lpDir, DWORD cchDir, 
        LPTSTR lpFileName, DWORD cchFileName, LPTSTR lpExt, DWORD cchExt);
    static BOOL AppendPath(LPTSTR lpDestPath, DWORD cchDestPath, LPCTSTR lpAppendPath);

};

} //namespace base