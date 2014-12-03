#pragma once
#include <Windows.h>

namespace base {

class Process
{
public:
    Process();
    virtual ~Process();

public:
    // Before Call Start again, you must first call Destroy() ro Join() or Detach() to Cleanup the prev state
    BOOL Create(LPCTSTR lpCommandLine, WORD wShowWindow = SW_SHOW,
        LPCTSTR lpCurrentDirectory = NULL, BOOL bUseStdHandle = FALSE);

    BOOL Destroy();
    BOOL Join(DWORD dwMilliseconds = INFINITE, DWORD *pdwExitCode = NULL);
    HANDLE Detach();

    BOOL Wait(DWORD dwMilliseconds = INFINITE);
    BOOL GetExitCode(DWORD *lpExitCode);

    BOOL Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
    BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
        DWORD dwMilliseconds = INFINITE, BOOL bOnStdErr = FALSE);

    HANDLE GetProcessHandle() const { return process_handle_; }
    HANDLE GetStdErrorHandle() const { return stderr_read_handle_; }
    HANDLE GetStdOutHandle() const { return stdout_read_handle_; }

    BOOL IsRuning();

private:
    HANDLE stdin_write_handle_;
    HANDLE stdout_read_handle_;
    HANDLE stderr_read_handle_;
    HANDLE process_handle_;
};

} //namespace base