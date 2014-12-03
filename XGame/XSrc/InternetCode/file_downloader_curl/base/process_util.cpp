#include "process_util.h"
#include <tchar.h>

using namespace base;

Process::Process()
{
    stdin_write_handle_ = NULL;
    stdout_read_handle_ = NULL;
    stderr_read_handle_ = NULL;
    process_handle_ = NULL;
}
Process::~Process()
{
    Destroy();
}

BOOL Process::Create(LPCTSTR lpCommandLine, WORD wShowWindow /* = SW_SHOW */,
    LPCTSTR lpCurrentDirectory /*= NULL*/, BOOL bUseStdHandle /*= FALSE*/)
{
    BOOL bRet = FALSE;

    LPTSTR lpCommandLineCopy = NULL;
    HANDLE hStdInR = NULL, hStdInW = NULL;
    HANDLE hStdOutR = NULL, hStdOutW = NULL;
    HANDLE hStdErrR = NULL, hStdErrW = NULL;
    PROCESS_INFORMATION pi = {0};
    BOOL bResult = FALSE;
    do
    {
        if (!(lpCommandLine != NULL && _tcslen(lpCommandLine) > 0))
        {
            break;
        }

        lpCommandLineCopy = (LPTSTR)malloc( (_tcslen(lpCommandLine) + 1) * sizeof(TCHAR) );
        _tcscpy_s(lpCommandLineCopy, (_tcslen(lpCommandLine) + 1), lpCommandLine);

        STARTUPINFO si = { sizeof(si) };
        GetStartupInfo(&si);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = wShowWindow;

        if(bUseStdHandle)
        {
            SECURITY_ATTRIBUTES sa = { sizeof(sa) };
            sa.bInheritHandle = TRUE;
            CreatePipe(&hStdInR, &hStdInW, &sa, 0);
            CreatePipe(&hStdOutR, &hStdOutW, &sa, 0);
            CreatePipe(&hStdErrR, &hStdErrW, &sa, 0);

            si.hStdInput = hStdInR;
            si.hStdOutput = hStdOutW;
            si.hStdError = hStdErrW;
            si.dwFlags |= STARTF_USESTDHANDLES;
        }


        if (!CreateProcess(NULL, lpCommandLineCopy, NULL, NULL, TRUE, 0, NULL,
            lpCurrentDirectory, &si, &pi))
        {
            break;
        }

        process_handle_ = pi.hProcess;
        pi.hProcess = NULL;

        if(bUseStdHandle)
        {
            stdin_write_handle_ = hStdInW;
            hStdInW = NULL;
            stdout_read_handle_ = hStdOutR;
            hStdOutR = NULL;
            stderr_read_handle_ = hStdErrR;
            hStdErrR = NULL;
        }

        bResult = TRUE;
    }
    while (false);


    if(pi.hProcess != NULL)
    {
        CloseHandle(pi.hProcess);
        pi.hProcess = NULL;
    }
    if(pi.hThread != NULL)
    {
        CloseHandle(pi.hThread);
        pi.hThread = NULL;
    }

    if(hStdErrW != NULL)
    {
        CloseHandle(hStdErrW);
        hStdErrW = NULL;
    }

    if(hStdErrR != NULL)
    {
        CloseHandle(hStdErrR);
        hStdErrR = NULL;
    }
    if(hStdOutW != NULL)
    {
        CloseHandle(hStdOutW);
        hStdOutW = NULL;
    }

    if(hStdOutR != NULL)
    {
        CloseHandle(hStdOutR);
        hStdOutR = NULL;
    }

    if(hStdInW != NULL)
    {
        CloseHandle(hStdInW);
        hStdInW = NULL;
    }

    if(hStdInR != NULL)
    {
        CloseHandle(hStdInR);
        hStdInR = NULL;
    }

    if(lpCommandLineCopy != NULL)
    {
        free(lpCommandLineCopy);
        lpCommandLineCopy = NULL;
    }

    return bResult;
}

BOOL Process::Destroy()
{
    if(!IsRuning())
    {
        if(process_handle_ != NULL)
        {
            CloseHandle(process_handle_);
            process_handle_ = NULL;
        }
        return TRUE;
    }

    TerminateProcess(process_handle_, ~0);

    CloseHandle(process_handle_);
    process_handle_ = NULL;

    Detach();

    return TRUE;
}

BOOL Process::Join(DWORD dwMilliseconds /* = INFINITE */, DWORD *pdwExitCode /* = NULL */)
{
    if(process_handle_ == NULL)
    {
        return FALSE;
    }

    DWORD dwRet = WaitForSingleObject(process_handle_, dwMilliseconds);
    if(dwRet != WAIT_OBJECT_0)
        return FALSE;

    DWORD dwExitCode = 0;
    if(!GetExitCodeProcess(process_handle_, &dwExitCode))
    {
        dwExitCode = ~0;
    }
    if(pdwExitCode != NULL)
        *pdwExitCode = dwExitCode;

    CloseHandle(process_handle_);
    process_handle_ = NULL;

    Detach();

    return TRUE;
}

HANDLE Process::Detach()
{
    HANDLE hProcess = process_handle_;
    if(stdin_write_handle_ != NULL)
    {
        CloseHandle(stdin_write_handle_);
        stdin_write_handle_ = NULL;
    }
    if(stdout_read_handle_ != NULL)
    {
        CloseHandle(stdout_read_handle_);
        stdout_read_handle_ = NULL;
    }
    if(stderr_read_handle_ != NULL)
    {
        CloseHandle(stderr_read_handle_);
        stderr_read_handle_ = NULL;
    }
    process_handle_ = NULL;
    return hProcess;
}

BOOL Process::Wait(DWORD dwMilliseconds /*= INFINITE*/)
{
    return WaitForSingleObject(process_handle_, dwMilliseconds) != WAIT_TIMEOUT;
}


BOOL Process::GetExitCode(DWORD *lpExitCode)
{
    if(process_handle_ == NULL || lpExitCode == NULL)
        return FALSE;
    return ::GetExitCodeProcess(process_handle_, lpExitCode);
}

BOOL Process::Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
{
    if(stdin_write_handle_ == NULL)
        return FALSE;
    return WriteFile(stdin_write_handle_, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL);
}

BOOL Process::Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead
    , DWORD dwMilliseconds /* = INFINITE */, BOOL bOnStdErr /*= FALSE*/)
{
    if(stdout_read_handle_ == NULL)
        return FALSE;

    HANDLE hStdR = bOnStdErr ? stderr_read_handle_ : stdout_read_handle_;

    if(dwMilliseconds != INFINITE)
    {
        DWORD dwTotalBytesAvail = 0;
        DWORD dwTickCount = ::GetTickCount();
        do
        {
            dwTotalBytesAvail = 0;
            if(!PeekNamedPipe(hStdR, NULL, 0, NULL, &dwTotalBytesAvail, NULL))
                return FALSE;
            if(dwTotalBytesAvail == 0)
            {
                Sleep(100);
            }
        } while(dwTotalBytesAvail == 0 && ::GetTickCount() - dwTickCount < dwMilliseconds);


        if(dwTotalBytesAvail == 0)
        {
            if(lpNumberOfBytesRead != NULL)
                *lpNumberOfBytesRead = 0;
            return TRUE;
        }
    }
    return ReadFile(hStdR, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, NULL);
}


BOOL Process::IsRuning()
{
    return WaitForSingleObject(process_handle_, 0) == WAIT_TIMEOUT;
}


