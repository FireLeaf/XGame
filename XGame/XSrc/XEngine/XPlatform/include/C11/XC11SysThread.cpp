/**************************************************************************
/*		FileName: XWinSysThread.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/15 21:06:46
/*		CopyRight: yc 
/*************************************************************************/

#ifdef _USE_C11_

#include "XSys.h"
#include <thread>
#include <condition_variable>

unsigned int ThreadProc(void* ptr)
{
	XThread* thread = (XThread*)(ptr);
	if (thread)
	{
		thread->Run();
		return 0;
	}

	return -1;
}

class XC11Thread : public XThread
{
public:
	XC11Thread() : XThread()
	{
		job_mutex = XSys::XCreateMutex();
		stdthread = NULL;
	}
	virtual ~XC11Thread()
	{
		ReleaseThread();
		if(job_mutex) XSys::XDeleteMutex(job_mutex);
	}
	bool CreateThread(pfnJobProc func, XJobDesc* desc)
	{
		job.job_proc = func;
//		job.thread = this;
		job.desc = desc;
		stdthread = new std::thread(ThreadProc, (void*)this);
		stdthread->detach();
		return (stdthread->native_handle() != NULL);
	}

	bool ReleaseThread()
	{
		Join();
		return true;
	}

	virtual void Suppend(){}
	virtual void Stop(){}
	virtual void Resume(){}
	virtual void Join()
	{
		if (stdthread)
		{
			stdthread->join();
		}
	}

	virtual void Run()
	{
		if (job.job_proc)
		{
			job.job_proc(job.desc);
		}
		//ReleaseThread();
	}

protected:
	std::thread* stdthread;
};

XThread* XSys::XCreateThread(pfnJobProc func, XJobDesc* desc)
{
	XC11Thread* thread = new XC11Thread();
	if (thread)
	{
		if (thread->CreateThread(func, desc))
		{
			return thread;
		}
		delete thread;
	}
	return NULL;
}

bool XSys::XReleaseThread(XThread* thread)
{
	if (!thread)
	{
		return true;
	}
	((XC11Thread*)thread)->ReleaseThread();
	delete thread;
	return true;
}

class XC11ThreadPool : public XThreadPool
{
public:
	XC11ThreadPool();
	virtual ~XC11ThreadPool(){}
	virtual bool CreateThreadPool(int thread_count);//
	virtual void ReleaseThreadPool();
	virtual void PauseThreadPool(){}
	virtual void ResumeThreadPool(){}

	HANDLE* GetWaitEvents(){return sync_events;}
	
	virtual void DoJob(const XJob& job)
	{
		job_mutex->Lock();
		XJob* ptr_job = new XJob;
		ptr_job->job_proc = job.job_proc;
		ptr_job->desc = job.desc;
		job_queue.push_back(ptr_job);
		SetEvent(sync_events[RUN_EVENT]);
		job_mutex->Unlock();
	}

	XJob* GetJob()
	{
		job_mutex->Lock();
		if (job_queue.size())
		{
			XJob* ret = (XJob*)(*(job_queue.begin()));
			job_queue.pop_front();
			
			if (job_queue.size())
			{
				SetEvent(sync_events[RUN_EVENT]);
			}
			else
			{
				ResetEvent(sync_events[RUN_EVENT]);
			}
			job_mutex->Unlock();
			return ret;
		}
		ResetEvent(sync_events[RUN_EVENT]);
		job_mutex->Unlock();
	
		return NULL;
	}
protected:
	std::vector<XThread*> thread_pool;
	std::list<XJob*> job_queue;
	//HANDLE sync_events[EVENT_NUM];
	std::condition_variable sync_condition[EVENT_NUM];
	std::mutex sync_mutex[EVENT_NUM];
	XMutex* job_mutex;//
};

struct XTPJobDesc : public XJobDesc
{
	XC11ThreadPool* pool;
};

xint32 TPJobProc(XJobDesc* parm)
{
	if (!parm)
	{
		return -1;
	}
	XC11ThreadPool* pool = ((XTPJobDesc*)parm)->pool;
	if (!pool)
	{
		return -1;
	}
	while(1)

	{
		int ret = WaitForMultipleObjects(XThreadPool::EVENT_NUM, (const HANDLE*)pool->GetWaitEvents(), FALSE, INFINITE);
		switch (ret - WAIT_OBJECT_0)
		{
		case XThreadPool::RUN_EVENT:
			break;
		case XThreadPool::PAUSE_EVENT:
			break;
		case XThreadPool::EXIT_EVENT:
			{
				return 0;
			}
			break;
		}
		XJob* job = pool->GetJob();
		if (!job)
		{
			continue;
		}

		if (job->job_proc)
		{
			job->job_proc(job->desc);
		}
	}
}

XC11ThreadPool::XC11ThreadPool()
{
	for (int i = 0; i < EVENT_NUM; i++)
	{
		sync_events[i] = CreateEvent(NULL, i != RUN_EVENT, FALSE, NULL)/*XSys::*/;
	}
	job_mutex = XSys::XCreateMutex();
}



bool XC11ThreadPool::CreateThreadPool(int thread_count)
{
	for (int i = 0; i < thread_count; i++)
	{
		XTPJobDesc* desc = new XTPJobDesc;
		desc->pool = this;
		XThread* ptr_thread = XSys::XCreateThread(TPJobProc, desc);
		if (ptr_thread)
		{
			thread_pool.push_back(ptr_thread);
		}
		else
		{
			return false;
		}
	}

	return true;
}

void XC11ThreadPool::ReleaseThreadPool()
{
	SetEvent(sync_events[EXIT_EVENT]);
	for (int i = 0; i < (int)thread_pool.size(); i++)
	{
		HANDLE hThread = ((XWinThread*)thread_pool[i])->GetHandle();
		WaitForSingleObject(hThread, INFINITE);
	}
}

XThreadPool* XSys::XCreateThreadPool(int thread_count)
{
	XC11ThreadPool* thread = new XC11ThreadPool();
	if (thread)
	{
		if (thread->CreateThreadPool(thread_count))
		{
			return thread;
		}
		delete thread;
	}
	return NULL;
}

#endif // _USE_C11_