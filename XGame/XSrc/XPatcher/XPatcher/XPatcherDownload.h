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

struct DownloadFileTask : public XJobDesc
{
	std::string url;
	std::string dest;
	int begin;
	int end;
	int cur;
	CURL* handle;
	FILE* fp;
	bool is_exit;

	DownloadFileTask(const char* url, const char* dest, int begin, int end, FILE* fp) : this->url(url), 
		this->dest(dest), this->begin(begin), this->end(end), handle(NULL), fp(NULL), is_exit(false){}
};

class XPatcherDownload
{
public:
	enum 
	{
		MIN_FILE_SIZE = 2 * 1024 * 1024,
		MAX_THREAD_NUM = 4,
	};
public:
	//获取文件大小
	bool GetFileSize(const char* url, ULONGLONG& file_size, int& response_code);

	bool DownloadFile(const char* url, const char* local_path, int thread_num);
protected:
	bool HttpRequestFile(const char* url);
};

#endif // XPatcherDownload