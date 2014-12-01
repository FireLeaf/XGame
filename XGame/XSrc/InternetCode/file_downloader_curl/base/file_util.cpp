#include "file_util.h"
#include <ShellAPI.h>
#include <tchar.h>

#pragma comment(lib, "shell32.lib")
#include <Shlwapi.h>
using namespace base;

//////////////////////////////////////////////////////////////////////////
BOOL FileUtil::IsFileExists(LPCTSTR lpFile)
{
    DWORD dwAttr = ::GetFileAttributes(lpFile);
    return (dwAttr != INVALID_FILE_ATTRIBUTES && !(dwAttr&FILE_ATTRIBUTE_DIRECTORY));
}

BOOL FileUtil::DeleteFile(LPCTSTR lpFile)
{
    if(!IsFileExists(lpFile))
        return TRUE;
    return ::DeleteFile(lpFile);
}


LONG FileUtil::GetFileSize(LPCTSTR lpFile)
{
    HANDLE hFile = ::CreateFile(lpFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
        return 0;
    DWORD dwSize = ::GetFileSize(hFile, NULL);
    ::CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    return dwSize;
}

BOOL FileUtil::IsDirectoryExists(LPCTSTR lpDir)
{
    DWORD dwAttr = ::GetFileAttributes(lpDir);
    return (dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL FileUtil::CreateDirectory(LPCTSTR lpDir)
{
    if(lpDir == NULL || _tcslen(lpDir) == 0)
        return FALSE;


    //LPCTSTR lpPointer = lpPathBuffer;
    //for(; *lpPointer != _T('\0'); lpPointer++)
    //{
    //    if(*lpPointer == _T('\\') || *lpPointer == _T('/'))
    //    {
    //        *lpPointer = _T('\0');
    //        if(!IsDirectoryExists(lpPathBuffer))
    //        {
    //            ::CreateDirectory(lpPathBuffer, NULL);
    //        }
    //        *lpPointer = _T('/');
    //    }
    //}

    //if(IsDirectoryExists(lpDir))
    //    return TRUE;
    return ::CreateDirectory(lpDir, NULL);
}

BOOL FileUtil::DeletePath(LPCTSTR lpPath, BOOL bSilent /*= TRUE*/)
{
    SHFILEOPSTRUCT FileOp={0};
    FileOp.fFlags = /*FOF_ALLOWUNDO | */FOF_NOCONFIRMATION;
    if(bSilent)
        FileOp.fFlags |= FOF_SILENT;

    FileOp.pFrom = lpPath; // Warning : don't uses FOF_MULTIDESTFILES, or can't use CMiRegularPath
    FileOp.pTo = NULL;
    FileOp.wFunc = FO_DELETE;
    int nRet = ::SHFileOperation(&FileOp);
    return (nRet == 0 || nRet == 2);
}

