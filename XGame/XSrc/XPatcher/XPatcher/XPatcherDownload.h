/**************************************************************************
/*		FileName: XPatcherDownload.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/12/01 20:40:05
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XPATCHERDOWNLOAD__H
#define __XPATCHERDOWNLOAD__H

typedef void (*pfnCallBack)(const void* buffer, int length);

struct XDownloadTaskData
{
public:
	void 
public:
	std::string url;
	FILE* fp;
	int range_begin;
	int range_end;
	int cur_size;
	CURL* curl_handle;
	pfnCallBack call_back;
};

class XPatcherDownload
{
public:
	//获取文件大小
	bool GetFileSize(const char* url, ULONGLONG& file_size, int& response_code);

	bool DownloadFile(const char* url);
protected:
	bool HttpRequestFile(const char* url);
};

#endif // XPatcherDownload