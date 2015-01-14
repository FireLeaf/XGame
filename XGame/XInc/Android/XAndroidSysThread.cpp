/**************************************************************************
/*		FileName: XWinSysThread.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/15 21:06:46
/*		CopyRight: yc 
/*************************************************************************/

#ifdef _ANDROID_PLATFORM
#include "XSys.h"
#include <pthread.h>

void* AndroidThreadProc(void* ptr)
{
	XThread* thread = (XThread*)(ptr);
	if (thread)
	{
		thread->Run();
		return 0;
	}

	return -1;
}

class XAndroidThread : public XThread
{
public:
	XAndroidThread() : XThread()
	{
		job_mutex = XSys::XCreateMutex();
		thread = 0;
	}
	virtual ~XAndroidThread()
	{
		ReleaseThread();
		if(job_mutex) XSys::XDeleteMutex(job_mutex);
	}
	bool CreateThread(pfnJobProc func, XJobDesc* desc)
	{
		job.job_proc = func;
		//		job.thread = this;
		job.desc = desc;

		return (0 == ::pthread_create(&thread, 0, AndroidThreadProc, (void*)this, 0, NULL));
	}

	bool ReleaseThread()
	{
		if (thread)
		{
			//WaitForSingleObject(m_hThread, INFINITE);
			//TerminateThread(m_hThread, 0);
		}
		return true;
	}

	virtual void Suppend(){}
	virtual void Stop(){}
	virtual void Resume(){}

	virtual void Run()
	{
		if (job.job_proc)
		{
			job.job_proc(job.desc);
		}
		//ReleaseThread();
	}

	pthread_t get_thread(){return thread;}
protected:
	pthread_t thread;
};

XThread* XSys::XCreateThread(pfnJobProc func, XJobDesc* desc)
{
	XAndroidThread* thread = new XAndroidThread();
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
	((XAndroidThread*)thread)->ReleaseThread();
	delete thread;
	return true;
}

class XAndroidThreadPool : public XThreadPool
{
public:
	XAndroidThreadPool();
	virtual ~XAndroidThreadPool(){}
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
	HANDLE sync_events[EVENT_NUM];
	XMutex* cond_mutex;
	XMutex* job_mutex;//
};

struct XTPJobDesc : public XJobDesc
{
	XAndroidThreadPool* pool;
};

xint32 TPJobProc(XJobDesc* parm)
{
	if (!parm)
	{
		return -1;
	}
	XAndroidThreadPool* pool = ((XTPJobDesc*)parm)->pool;
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

XAndroidThreadPool::XAndroidThreadPool()
{
	for (int i = 0; i < EVENT_NUM; i++)
	{
		sync_events[i] = CreateEvent(NULL, i != RUN_EVENT, FALSE, NULL)/*XSys::*/;
	}
	job_mutex = XSys::XCreateMutex();
}



bool XAndroidThreadPool::CreateThreadPool(int thread_count)
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

void XAndroidThreadPool::ReleaseThreadPool()
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
	XAndroidThreadPool* thread = new XAndroidThreadPool();
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
#endif
