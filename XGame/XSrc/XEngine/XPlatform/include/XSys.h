/**************************************************************************
/*		FileName: XSys.h
/*		FileDesc: 
/*		FileAuthor: YangCaoϵͳ
/*		CreateTime: 2014/10/14 20:28:23
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XSYS__H
#define __XSYS__H

#include "XType.h"

class XMutex
{
public:
	virtual ~XMutex(){}
public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
};

class XEvent
{

};

struct XJobDesc 
{

};

typedef xint32 (*pfnJobProc)(XJobDesc* parm);

class XThread;

struct XJob
{
	XJobDesc* desc;
	pfnJobProc job_proc;
//	XThread* thread;
	XJob():desc(NULL), job_proc(NULL)/*, thread(NULL)*/{}
};

class XThread
{
public:
	enum
	{
		LOWER_PRIORITY,
		NORMAL_PRIORITY,
		HIGH_PRIORITY,
		PRIORITY_NUM,
	};
public:
	XThread():job(), job_mutex(NULL){}
	virtual ~XThread(){}
public:
	virtual void Suppend() = 0;
	virtual void Stop() = 0;
	virtual void Resume() = 0;
	virtual void Run() = 0;
	virtual void Join() = 0;
protected:
	//std::vector<XJob> jobs[PRIORITY_NUM];
	XJob job;
	XMutex* job_mutex;//
};

class XThreadPool
{
public:
	enum
	{
		RUN_EVENT,
		PAUSE_EVENT,
		EXIT_EVENT,
		EVENT_NUM,
	};
public:
	virtual ~XThreadPool(){}

	virtual bool CreateThreadPool(int thread_count) = 0;//
	virtual void ReleaseThreadPool() = 0;
	virtual void PauseThreadPool() = 0;
	virtual void ResumeThreadPool() = 0;

	virtual void DoJob(const XJob& job) = 0;
protected:
};
	
namespace XSys
{
	//日志打印调试器或控制台上
	void XLogOutput(const char* log);

	XMutex* XCreateMutex();
	void XDeleteMutex(XMutex*);
	XThread* XCreateThread(pfnJobProc func, XJobDesc* desc);
	bool XReleaseThread(XThread*);
	XThreadPool* XCreateThreadPool(int thread_count);
	void XReleaseThreadPool(XThreadPool*);

//文件操作
	bool XCreateFile(const char* file_path);
	bool XIsFileExist(const char* file_path);
	bool XIsDirectory(const char* path);
	bool XCreateDirectory(const char* path);
	bool XDeleteFile(const char* path);
	bool XDeleteDirectory(const char* path, bool bIsSubDelAll);
	bool XSetFileSize(const char* path, long size);
	bool XSetFileSize(FILE* fp, long size);
	int XFileNo(FILE* fp);
};

#endif // XSys