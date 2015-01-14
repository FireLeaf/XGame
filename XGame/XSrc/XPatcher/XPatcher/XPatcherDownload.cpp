/**************************************************************************
/*		FileName: XPatcherDownload.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/12/01 20:42:14
/*		CopyRight: yc 
/*************************************************************************/
#include "XPatcherDownload.h"
#include "XPatcher.h"
#include "XTemplate.h"

XPatcherDownload::XPatcherDownload()
{
	thread_handle = NULL;
}

XPatcherDownload::~XPatcherDownload()
{
	if (thread_handle)
	{
		XSys::XReleaseThread(thread_handle);
	}
}

bool XPatcherDownload::GetFileSize(const char* url, ULONGLONG& file_size, int& response_code)
{
	CURL* http_handle = curl_easy_init();
	file_size = 0;
	CURLcode res = CURLE_OK;
	bool isOk = false;
	do{
		std::string url_a = url;
		char* url_c = const_cast<char*> ( url_a.c_str() );
		res = curl_easy_setopt( http_handle, CURLOPT_URL, url_c );
		if ( CURLE_OK != res ) break;

		res = curl_easy_setopt( http_handle, CURLOPT_HEADER, 1 );
		if ( CURLE_OK != res ) break;

		res = curl_easy_setopt( http_handle, CURLOPT_NOBODY, 1 );
		if ( CURLE_OK != res ) break;

		res = curl_easy_setopt( http_handle, CURLOPT_MAXREDIRS, -1);
		if ( CURLE_OK != res ) break;

		res = curl_easy_setopt( http_handle, CURLOPT_FOLLOWLOCATION, 1);
		if ( CURLE_OK != res ) break;

		res = curl_easy_perform( http_handle );
		if ( CURLE_OK != res ) break;

		res = curl_easy_getinfo(http_handle, CURLINFO_RESPONSE_CODE, &response_code);
		if ( CURLE_OK != res ) break;

		if ( 200 != response_code )
		{
			//LOG_V_W( _T("HttpCode:%d"), nResponseCode );
			break;
		}

		double sz = 0.0f;
		res = curl_easy_getinfo( http_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &sz );
		if ( CURLE_OK != res ) break;

		file_size = static_cast<ULONGLONG>(sz);

		isOk = true;
	}while ( false );

	if ( CURLE_OK !=  res )
	{
		response_code = res;
	}
	return isOk;
}

xint32 DownloadProc(XJobDesc* desc)
{
// 	DownloadFileTask* task_desc= (DownloadFileTask*)desc;
// 	int last_http_status_code = 0;
// 	int  error_reason = HDErrorInternal;
// 	ULONGLONG totalBytes2Download = task_desc->end - task_desc->begin;
// 	CURLM* multi_handle = curl_multi_init();
// 	std::string url_a = task_desc->url;
// 	char* url = const_cast<char*> ( url_a.c_str() );
// 	CURL* http_handle = curl_easy_init();
// 	task_desc->handle = http_handle;
// 	curl_easy_setopt( http_handle, CURLOPT_URL, url );
// 	char opt_range[100] = { 0 };
// 	sprintf_s( opt_range, 100, "%lld-%lld", task_desc->begin, task_desc->end - 1 );
// 	curl_easy_setopt( http_handle, CURLOPT_RANGE, opt_range );
// 	curl_easy_setopt( http_handle, CURLOPT_WRITEFUNCTION, WriteData );
// 	curl_easy_setopt( http_handle, CURLOPT_WRITEDATA, &task_desc );
// 	curl_easy_setopt( http_handle, CURLOPT_MAXREDIRS, -1 );
// 	curl_easy_setopt( http_handle, CURLOPT_FOLLOWLOCATION, 1);
// 
// 	curl_multi_add_handle( multi_handle, http_handle );
// 	const int nPeekIntervalInMs = 20;//ms
// 	const int nTimeout4NoDataInSec = 60;//s
// 	const int nMaxFailedCount = nTimeout4NoDataInSec * 1000 / nPeekIntervalInMs;
// 	int nSuccessiveFailedCount = 0;
// 	int still_running = 0;
// 	while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &still_running) );
// 	while( still_running && !task_desc->is_exit )
// 	{
// 		fd_set fdread;
// 		fd_set fdwrite;
// 		fd_set fdexcep;
// 
// 		FD_ZERO(&fdread);
// 		FD_ZERO(&fdwrite);
// 		FD_ZERO(&fdexcep);
// 
// 		struct timeval timeout;
// 		timeout.tv_sec = 0;
// 		timeout.tv_usec = nPeekIntervalInMs * 1000;
// 		/* get file descriptors from the transfers */
// 		int maxfd;
// 		curl_multi_fdset( multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd );
// 		int rc = select( maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout );
// 		switch(rc)
// 		{
// 		case SOCKET_ERROR:
// 			//LOG_V_E( _T("socket error:[%d] returned by select func"), WSAGetLastError() );
// 			task_desc->is_exit = true;
// 			break;
// 		case 0://timeout
// 			++nSuccessiveFailedCount;
// 			if ( nSuccessiveFailedCount > nMaxFailedCount )
// 			{
// 				//LOG_V_T( _T("HttpWebFileClient.timeout to download in HTTP") );
// 				//exit_flag_is_set_ = true;
// 				error_reason = HDErrorTimeout;
// 			}
// 			else
// 			{
// 				while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( multi_handle, &still_running ) );
// 			}
// 			break;
// 		default:
// 			nSuccessiveFailedCount = 0;
// 			/* timeout or readable/writable sockets */
// 			while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( multi_handle, &still_running ) );
// 			break;
// 		}
// 	}
// 
// 	CURLcode res = curl_easy_getinfo(http_handle, CURLINFO_RESPONSE_CODE, &last_http_status_code);
// 	ULONGLONG acutalDownloadBytes = 0;
// 	double dlsz = 0.0f;
// 	res = curl_easy_getinfo( http_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dlsz );
// 	if ( CURLE_OK == res )
// 	{
// 		acutalDownloadBytes = (ULONGLONG)dlsz;
// 	}
// 	curl_multi_remove_handle( multi_handle, http_handle );
// 	curl_multi_cleanup( multi_handle );
// 	curl_easy_cleanup( http_handle );
// 	bool bOk = (totalBytes2Download == acutalDownloadBytes) && 
// 		(last_http_status_code /100 == 2 ) &&
// 		(task_desc->begin == task_desc->end );
// 	if ( bOk )
// 	{
// 		error_reason = HDErrorNone;
// 	}
	return 0;
}

xint32 _HttpDownloadProc(XJobDesc* parm)
{
	DownloadFileTask* task = (DownloadFileTask*)parm;
	if (task)
	{
		XHttpDownload hd(task->url.c_str(), task->local_path.c_str(), task->thread_num);
		delete task;
		hd.Run();
		return 1;
	}

	return 0;
}

bool XPatcherDownload::DownloadFileBackgroud(const char* url, const char* local_path, int thread_num)
{
	DownloadFileTask* dft = new DownloadFileTask(url, local_path, thread_num);
	thread_handle = XSys::XCreateThread(_HttpDownloadProc, dft);
	return (thread_handle != NULL);
}

bool XPatcherDownload::DownloadFile(const char* url, const char* local_path, int thread_num)
{
	XHttpDownload hd(url, local_path, thread_num);
	hd.Run();
	return true;
}

XHttpDownload::XHttpDownload(const char* url, const char* local_path, int thread_num)
{
	this->url = url;
	this->local_path = local_path;
	this->thread_num = thread_num;
	running = true;
}

XHttpDownload::~XHttpDownload()
{

}

size_t WriteData( void* data, size_t size, size_t nmemb, void* pClientData )
{
	size_t dwWriten = size * nmemb;
	int statusCode = 0;
	XHttpDownload::HttpTask* http_task = (XHttpDownload::HttpTask*)pClientData;
	if (!http_task)
	{
		Assert(0);
		return 0;
	}
	CURLcode res = curl_easy_getinfo( http_task->http_handle, CURLINFO_RESPONSE_CODE, &statusCode );
	if ( statusCode/100 != 2 ) //not range in 2**
	{
		return 0;
	}

	http_task->cur += dwWriten;
	http_task->hd->cur_size += dwWriten;

	if ( http_task->begin + dwWriten > http_task->end )
	{
		Assert(0);
		return 0;
	}

	if (http_task->fp)
	{
		int writed = fwrite(data, 1, dwWriten, http_task->fp);
		return writed;
	}
	return 0;
}

bool XHttpDownload::InitDownload()
{
	int res_code = 0;
	if (!XPatcherDownload::GetFileSize(url.c_str(), file_size, res_code))
	{
		return false;
	}

	if (file_size <= 0)
	{
		return true;
	}

	if (file_size < MIN_FILE_SIZE || thread_num < 0 || thread_num > MAX_THREAD_NUM)
	{
		thread_num = 1;
	}
	ULONGLONG each_size = file_size / thread_num;
	if(!XSys::XCreateFile(local_path.c_str()))
	{
		return false;
	}
	XSys::XSetFileSize(local_path.c_str(), file_size);
	for (int i = 0; i < thread_num; i++)
	{
		FILE* fp = fopen(local_path.c_str(), "r+b");
		if (!fp)
		{
			return false;
		}
		fseek(fp, i * each_size, SEEK_SET);
		HttpTask* http_task = new HttpTask;
		http_task->fp = fp;
		http_task->begin = i * each_size;
		http_task->end = xMin((int)(i + each_size), (int)file_size);
		http_task->cur = 0;
		http_task->seconds = 0;
		http_task->hd = this;

		http_task->total = http_task->end - http_task->begin;
		http_task->multi_handle = curl_multi_init();
		http_task->http_handle = curl_easy_init();
		CURLcode code = curl_easy_setopt( http_task->http_handle, CURLOPT_URL, url.c_str() );
		char opt_range[100] = { 0 };
		sprintf_s( opt_range, 100, "%lld-%lld", http_task->begin, http_task->end - 1 );
		code = curl_easy_setopt( http_task->http_handle, CURLOPT_RANGE, opt_range );
		code = curl_easy_setopt( http_task->http_handle, CURLOPT_WRITEFUNCTION, WriteData );
		code = curl_easy_setopt( http_task->http_handle, CURLOPT_WRITEDATA, http_task );
		code = curl_easy_setopt( http_task->http_handle, CURLOPT_MAXREDIRS, -1 );
		code = curl_easy_setopt( http_task->http_handle, CURLOPT_FOLLOWLOCATION, 1);

		CURLMcode mcode = curl_multi_add_handle( http_task->multi_handle, http_task->http_handle );
		int running_handles = 0;
		while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform(http_task->multi_handle, &running_handles) );
		if (!running_handles)
		{
			return false;
		}
		http_tasks.push_back(http_task);
	}
	return true;
}

void XHttpDownload::UpdateDownload()
{
	for (int i = 0; i < http_tasks.size(); i++)
	{
		if (!http_tasks[i] )
		{
			running = false;
			return;
		}
		if (http_tasks[i]->status == STATUS_FINISH)
		{
			continue;
		}
		else if (http_tasks[i]->status == STASTUS_DISCONNECT)
		{
			running = false;
			return;
		}
		
		const int nPeekIntervalInMs = 20;//ms
		const int nTimeout4NoDataInSec = 60;//s
		const int nMaxFailedCount = nTimeout4NoDataInSec * 1000 / nPeekIntervalInMs;
		int nSuccessiveFailedCount = 0;
		int running_handles = 0;

		fd_set fdread;
		fd_set fdwrite;
		fd_set fdexcep;

		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcep);

		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = nPeekIntervalInMs * 1000;
		/* get file descriptors from the transfers */
		int maxfd;
		curl_multi_fdset( http_tasks[i]->multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd );
		int rc = select( maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout );
		switch(rc)
		{
		case SOCKET_ERROR:
			http_tasks[i]->status = STASTUS_DISCONNECT;
			break;
		case 0://timeout
			{
				++failed_count;
				if ( failed_count > nMaxFailedCount )
				{
					running = false;
					return;
				}
				else
				{
					while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( http_tasks[i]->multi_handle, &running_handles ) );
					running = running_handles > 0;
				}
			}
			break;
		default:
			{
				failed_count = 0;
				/* timeout or readable/writable sockets */
				while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( http_tasks[i]->multi_handle, &running_handles ) );
				running = running_handles > 0;
			}
			break;
		}
	}
}

void XHttpDownload::EndDownload()
{
	
}

bool XHttpDownload::Run()
{
	
	if (!InitDownload())
	{
		return false;
	}
	
	while(running)
	{
		UpdateDownload();
	}

	EndDownload();

	return true;
}