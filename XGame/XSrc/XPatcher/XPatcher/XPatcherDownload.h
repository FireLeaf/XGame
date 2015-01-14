/**************************************************************************
/*		FileName: XPatcherDownload.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/12/01 20:40:05
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XPATCHERDOWNLOAD__H
#define __XPATCHERDOWNLOAD__H

#include <string>
#include "curl/curl.h"
#include "XType.h"
#include "XSys.h"

typedef void (*pfnCallBack)(const void* buffer, int length);

enum 
{
	MIN_FILE_SIZE = 2 * 1024 * 1024,
	MAX_THREAD_NUM = 4,
};

struct DownloadFileTask : public XJobDesc
{
	std::string url;
	std::string local_path;
	int thread_num;

	DownloadFileTask(const char* url, const char* dest, int thread_num) 
	{
		this->url = url;
		this->local_path = dest;
		this->thread_num = thread_num;
	}
};

class XHttpDownload
{
public:
	enum DOWNLOAD_STATUS
	{
		STATUS_RUNNING,//运行中
		STATUS_FINISH,//完成
		STATUS_BLOCK,//阻塞
		STASTUS_DISCONNECT,//断连
	};
	struct HttpTask 
	{
		FILE* fp;//文件指针
		ULONGLONG begin;//起始点
		ULONGLONG end;//结束点
		ULONGLONG total;
		ULONGLONG cur;//当前指针
		CURL* http_handle;
		CURLM* multi_handle;
		int status;//当前状态
		int seconds;//花费时间
		XHttpDownload* hd;
	};
public:
	XHttpDownload(const char* url, const char* local_path, int thread_num);
	~XHttpDownload();
	bool Run();
protected:
	bool InitDownload();//初始化下载
	void UpdateDownload();//更新下载
	void EndDownload();
public:
	ULONGLONG cur_size;
protected:
	std::vector<HttpTask*> http_tasks;//下载任务
	std::string url;
	std::string local_path;
	int thread_num;
	bool running;
	ULONGLONG file_size;
	int failed_count;
};

class XPatcherDownload
{
public:
	XPatcherDownload();
	~XPatcherDownload();
public:
	//获取文件大小
	static bool GetFileSize(const char* url, ULONGLONG& file_size, int& response_code);

	bool DownloadFile(const char* url, const char* local_path, int thread_num);
	bool DownloadFileBackgroud(const char* url, const char* local_path, int thread_num);//异步下载
protected:
	bool HttpRequestFile(const char* url);
private:
	XThread* thread_handle;
};

#endif // XPatcherDownload