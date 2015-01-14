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
		STATUS_RUNNING,//������
		STATUS_FINISH,//���
		STATUS_BLOCK,//����
		STASTUS_DISCONNECT,//����
	};
	struct HttpTask 
	{
		FILE* fp;//�ļ�ָ��
		ULONGLONG begin;//��ʼ��
		ULONGLONG end;//������
		ULONGLONG total;
		ULONGLONG cur;//��ǰָ��
		CURL* http_handle;
		CURLM* multi_handle;
		int status;//��ǰ״̬
		int seconds;//����ʱ��
		XHttpDownload* hd;
	};
public:
	XHttpDownload(const char* url, const char* local_path, int thread_num);
	~XHttpDownload();
	bool Run();
protected:
	bool InitDownload();//��ʼ������
	void UpdateDownload();//��������
	void EndDownload();
public:
	ULONGLONG cur_size;
protected:
	std::vector<HttpTask*> http_tasks;//��������
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
	//��ȡ�ļ���С
	static bool GetFileSize(const char* url, ULONGLONG& file_size, int& response_code);

	bool DownloadFile(const char* url, const char* local_path, int thread_num);
	bool DownloadFileBackgroud(const char* url, const char* local_path, int thread_num);//�첽����
protected:
	bool HttpRequestFile(const char* url);
private:
	XThread* thread_handle;
};

#endif // XPatcherDownload