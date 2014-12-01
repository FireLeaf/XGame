#pragma once
#include <Windows.h>

namespace base {

class Thread
{
public:
    Thread();
    // Before Call Thread, You Must cleanup the Previous State first.
    virtual ~Thread();

public:
    // Before Call Start, you must first call Stop() ro Join() or Detach() to Cleanup the prev state.
    bool Start(size_t nStackSize = 0);

    bool Stop(DWORD dwMilliseconds = 5000);
    bool Join(DWORD dwMilliseconds = INFINITE, DWORD *pdwExitCode = NULL);
    HANDLE Detach();

    bool Suspend();
    bool Resume();

    bool GetExitCode(DWORD *lpExitCode);

    bool IsRuning();

protected:
    virtual UINT Run();
    virtual bool StopRequest();

private:
    static unsigned int __stdcall SThreadProc(void* pParam);

protected:
    HANDLE thread_handle_;
    UINT thread_id_;
};


class CriticalSection
{
public:
    CriticalSection();
    virtual ~CriticalSection();

public:
    void Lock();
    void Unlock();
    bool TryLock();

private:
    CRITICAL_SECTION cs_;
};


template <typename T>
class SmartLockT
{
public:
    SmartLockT(T &locker)
        : ref_locker_(locker)
    {
        ref_locker_.Lock();
    }

    ~SmartLockT()
    {
        ref_locker_.Unlock();
    }
private:
    T &ref_locker_;
};

} //// namespace base