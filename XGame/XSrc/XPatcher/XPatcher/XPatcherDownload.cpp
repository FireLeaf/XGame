/**************************************************************************
/*		FileName: XPatcherDownload.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/12/01 20:42:14
/*		CopyRight: yc 
/*************************************************************************/
#include "XPatcherDownload.h"
#include "XPatcher.h"

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

size_t WriteData( void* data, size_t size, size_t nmemb, void* pClientData )
{
	size_t dwWriten = size * nmemb;
	int statusCode = 0;
	DownloadFileTask* pDownloadData = (DownloadFileTask*)pClientData;
	if (!pDownloadData)
	{
		Assert(0);
		return 0;
	}
	CURLcode res = curl_easy_getinfo( pDownloadData->handle, CURLINFO_RESPONSE_CODE, &statusCode );
	if ( statusCode/100 != 2 ) //not range in 2**
	{
		return 0;
	}
	if ( pDownloadData->begin + dwWriten > pDownloadData->end )
	{
		Assert(0);
		return 0;
	}

	if (pDownloadData->fp)
	{
		int writed = fwrite(data, 1, dwWriten, pDownloadData->fp);
		return writed;
	}
	return 0;
}

xint32 DownloadProc(XJobDesc* desc)
{
	DownloadFileTask* task_desc= (DownloadFileTask*)desc;
	int last_http_status_code = 0;
	int  error_reason = HDErrorInternal;
	ULONGLONG totalBytes2Download = task_desc->end - task_desc->begin;
	CURLM* multi_handle = curl_multi_init();
	std::string url_a = task_desc->url;
	char* url = const_cast<char*> ( url_a.c_str() );
	CURL* http_handle = curl_easy_init();
	task_desc->handle = http_handle;
	curl_easy_setopt( http_handle, CURLOPT_URL, url );
	char opt_range[100] = { 0 };
	sprintf_s( opt_range, 100, "%lld-%lld", task_desc->begin, task_desc->end - 1 );
	curl_easy_setopt( http_handle, CURLOPT_RANGE, opt_range );
	curl_easy_setopt( http_handle, CURLOPT_WRITEFUNCTION, WriteData );
	curl_easy_setopt( http_handle, CURLOPT_WRITEDATA, &task_desc );
	curl_easy_setopt( http_handle, CURLOPT_MAXREDIRS, -1 );
	curl_easy_setopt( http_handle, CURLOPT_FOLLOWLOCATION, 1);

	curl_multi_add_handle( multi_handle, http_handle );
	const int nPeekIntervalInMs = 20;//ms
	const int nTimeout4NoDataInSec = 60;//s
	const int nMaxFailedCount = nTimeout4NoDataInSec * 1000 / nPeekIntervalInMs;
	int nSuccessiveFailedCount = 0;
	int still_running = 0;
	while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &still_running) );
	while( still_running && !task_desc->is_exit )
	{
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
		curl_multi_fdset( multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd );
		int rc = select( maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout );
		switch(rc)
		{
		case SOCKET_ERROR:
			//LOG_V_E( _T("socket error:[%d] returned by select func"), WSAGetLastError() );
			task_desc->is_exit = true;
			break;
		case 0://timeout
			++nSuccessiveFailedCount;
			if ( nSuccessiveFailedCount > nMaxFailedCount )
			{
				//LOG_V_T( _T("HttpWebFileClient.timeout to download in HTTP") );
				//exit_flag_is_set_ = true;
				error_reason = HDErrorTimeout;
			}
			else
			{
				while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( multi_handle, &still_running ) );
			}
			break;
		default:
			nSuccessiveFailedCount = 0;
			/* timeout or readable/writable sockets */
			while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( multi_handle, &still_running ) );
			break;
		}
	}

	CURLcode res = curl_easy_getinfo(http_handle, CURLINFO_RESPONSE_CODE, &last_http_status_code);
	ULONGLONG acutalDownloadBytes = 0;
	double dlsz = 0.0f;
	res = curl_easy_getinfo( http_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dlsz );
	if ( CURLE_OK == res )
	{
		acutalDownloadBytes = (ULONGLONG)dlsz;
	}
	curl_multi_remove_handle( multi_handle, http_handle );
	curl_multi_cleanup( multi_handle );
	curl_easy_cleanup( http_handle );
	bool bOk = (totalBytes2Download == acutalDownloadBytes) && 
		(last_http_status_code /100 == 2 ) &&
		(task_desc->begin == task_desc->end );
	if ( bOk )
	{
		error_reason = HDErrorNone;
	}
	return 0;
}

bool XPatcherDownload::DownloadFile(const char* url, const char* local_path, int thread_num)
{
	ULONGLONG file_size;
	int res_code = 0;
	if (!GetFileSize(url, file_size, res_code))
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
	int each_size = file_size / thread_num;
	DownloadFileTask** tasks = new DownloadFileTask*[thread_num];
	if(!XSys::XCreateFile(local_path))
	{
		return false;
	}
	XSys::XSetFileSize(local_path, file_size);
	for (int i = 0; i < thread_num; i++)
	{
		FILE* fp = fopen(local_path, "r+b");
		if (!fp)
		{
			goto End_Label;
		}
		tasks[i] = new DownloadFileTask(url, local_path, i * each_size, xMin((i + 1) * each_size, file_size), fp);
		if (!tasks[i])
		{
			goto End_Label;
		}
	}


End_Label:
	for (int i = 0; i < thread_num; i++)
	{

	}
	return true;
}