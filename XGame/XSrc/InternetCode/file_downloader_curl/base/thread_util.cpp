
#include "thread_util.h"
#include <process.h>    // for _beginthreadex
using namespace base;

#define WAIT_THREAD_EXIT_TIMEOUT 5000 * 60

Thread::Thread()
{
    thread_handle_ = NULL;
    thread_id_ = 0;
}

Thread::~Thread()
{
    Stop();
    thread_id_ = 0;
}

bool Thread::Start(size_t nStackSize /* = 0 */)
{
    if(thread_handle_ != NULL)
    {
        return false;
    }
    thread_handle_ = (HANDLE)_beginthreadex(NULL, nStackSize, SThreadProc,
        this, 0, &thread_id_);
    return thread_handle_ != NULL;
}

bool Thread::Stop(DWORD dwMilliseconds /*= 5000*/)
{
    if(!IsRuning())
    {
        if(thread_handle_ != NULL)
        {
            CloseHandle(thread_handle_);
            thread_handle_ = NULL;
        }
        thread_id_ = 0;
        return TRUE;
    }

    if(StopRequest())
    {
        if(Join(WAIT_THREAD_EXIT_TIMEOUT))
        {
            return TRUE;
        }
    }

    Sleep(dwMilliseconds);

    TerminateThread(thread_handle_, ~0);

    CloseHandle(thread_handle_);
    thread_handle_ = NULL;

    thread_id_ = 0;

    return TRUE;
}

bool Thread::Join(DWORD dwMilliseconds /* = INFINITE */, DWORD *pdwExitCode /*= NULL*/)
{
    if(thread_handle_ == NULL)
    {
        return false;
    }

    DWORD dwRet = WaitForSingleObject(thread_handle_, dwMilliseconds);

    if(dwRet != WAIT_OBJECT_0)
        return false;

    DWORD dwExitCode = 0;
    if(!GetExitCodeThread(thread_handle_, &dwExitCode))
    {
        dwExitCode = ~0;
    }
    if(pdwExitCode != NULL)
        *pdwExitCode = dwExitCode;

    CloseHandle(thread_handle_);
    thread_handle_ = NULL;

    thread_id_ = 0;

    return TRUE;
}
HANDLE Thread::Detach()
{
    HANDLE hThread = thread_handle_;
    thread_handle_ = NULL;
    thread_id_ = 0;
    return hThread;
}


bool Thread::Suspend()
{
    if(!IsRuning())
        return false;
    SuspendThread(thread_handle_);
    return TRUE;
}

bool Thread::Resume()
{
    if(!IsRuning())
        return false;
    ResumeThread(thread_handle_);
    return TRUE;
}

bool Thread::GetExitCode(DWORD *lpExitCode)
{
    if(thread_handle_ == NULL || lpExitCode == NULL)
        return false;
    return GetExitCodeThread(thread_handle_, lpExitCode) == TRUE;
}


UINT Thread::Run()
{
    return 0;
}

bool Thread::StopRequest()
{
    return false;
}

bool Thread::IsRuning()
{
    return WaitForSingleObject(thread_handle_, 0) == WAIT_TIMEOUT;
}

unsigned int __stdcall Thread::SThreadProc(void* pParam)
{
    Thread *pThis = (Thread*)pParam;
    return pThis->Run();
}


CriticalSection::CriticalSection()
{
    InitializeCriticalSectionAndSpinCount(&cs_, 5000);
}

CriticalSection::~CriticalSection()
{
    DeleteCriticalSection(&cs_);
}

void CriticalSection::Lock()
{
    EnterCriticalSection(&cs_);
}

void CriticalSection::Unlock()
{
    LeaveCriticalSection(&cs_);
}

bool CriticalSection::TryLock()
{
    return TryEnterCriticalSection(&cs_) == TRUE;
}
