// curl_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
// extern "C"
// {
	#include "curl/curl.h"
//}
#pragma comment(lib, "libcurl_d.lib")

UINT DownloadFile(const char* url, const char* path);
bool GetFileSize(const char* url, long long& file_size, int& response_code)
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

int _tmain(int argc, _TCHAR* argv[])
{
	long long szFile = 0;
	int responseCode = 0;
	const char* url = "http://127.0.0.1/fileres/[迅雷下载www.xunbo.cc]行尸走肉.第二季.1024高清EP04.mkv";
	GetFileSize(url, szFile, responseCode);
	DownloadFile(url, "hello.mkv");
	return 0;
// 	 CURL *curl;
//  
//   CURLM *multi_handle;
//   int still_running;
//  
//   struct curl_httppost *formpost=NULL;
//   struct curl_httppost *lastptr=NULL;
//   struct curl_slist *headerlist=NULL;
//   static const char buf[] = "Expect:";
//  
//   /* Fill in the file upload field. This makes libcurl load data from
//      the given file name when curl_easy_perform() is called. */ 
//   curl_formadd(&formpost,
//                &lastptr,
//                CURLFORM_COPYNAME, "sendfile",
//                CURLFORM_FILE, "postit2.c",
//                CURLFORM_END);
//  
//   /* Fill in the filename field */ 
//   curl_formadd(&formpost,
//                &lastptr,
//                CURLFORM_COPYNAME, "filename",
//                CURLFORM_COPYCONTENTS, "postit2.c",
//                CURLFORM_END);
//  
//   /* Fill in the submit field too, even if this is rarely needed */ 
//   curl_formadd(&formpost,
//                &lastptr,
//                CURLFORM_COPYNAME, "submit",
//                CURLFORM_COPYCONTENTS, "send",
//                CURLFORM_END);
//  
//   curl = curl_easy_init();
//   multi_handle = curl_multi_init();
//  
//   /* initalize custom header list (stating that Expect: 100-continue is not
//      wanted */ 
//   headerlist = curl_slist_append(headerlist, buf);
//   if(curl && multi_handle) {
//  
//     /* what URL that receives this POST */ 
//     curl_easy_setopt(curl, CURLOPT_URL, "http://www.gameres.com/data/attachment/forum/201411/17/184119uzkzk6emmwounnuw.jpg");
//     curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//  
//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
//     curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
//  
//     curl_multi_add_handle(multi_handle, curl);
//  
//     curl_multi_perform(multi_handle, &still_running);
//  
//     do {
//       struct timeval timeout;
//       int rc; /* select() return code */ 
//       CURLMcode mc; /* curl_multi_fdset() return code */ 
//  
//       fd_set fdread;
//       fd_set fdwrite;
//       fd_set fdexcep;
//       int maxfd = -1;
//  
//       long curl_timeo = -1;
//  
//       FD_ZERO(&fdread);
//       FD_ZERO(&fdwrite);
//       FD_ZERO(&fdexcep);
//  
//       /* set a suitable timeout to play around with */ 
//       timeout.tv_sec = 1;
//       timeout.tv_usec = 0;
//  
//       curl_multi_timeout(multi_handle, &curl_timeo);
//       if(curl_timeo >= 0) {
//         timeout.tv_sec = curl_timeo / 1000;
//         if(timeout.tv_sec > 1)
//           timeout.tv_sec = 1;
//         else
//           timeout.tv_usec = (curl_timeo % 1000) * 1000;
//       }
//  
//       /* get file descriptors from the transfers */ 
//       mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
//  
//       if(mc != CURLM_OK)
//       {
//         fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
//         break;
//       }
//  
//       /* On success the value of maxfd is guaranteed to be >= -1. We call
//          select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
//          no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
//          to sleep 100ms, which is the minimum suggested value in the
//          curl_multi_fdset() doc. */ 
//  
//       if(maxfd == -1) {
// #ifdef _WIN32
//         Sleep(100);
//         rc = 0;
// #else
//         /* Portable sleep for platforms other than Windows. */ 
//         struct timeval wait = { 0, 100 * 1000 }; /* 100ms */ 
//         rc = select(0, NULL, NULL, NULL, &wait);
// #endif
//       }
//       else {
//         /* Note that on some platforms 'timeout' may be modified by select().
//            If you need access to the original value save a copy beforehand. */ 
//         rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
//       }
//  
//       switch(rc) {
//       case -1:
//         /* select error */ 
//         break;
//       case 0:
//       default:
//         /* timeout or readable/writable sockets */ 
//         printf("perform!\n");
//         curl_multi_perform(multi_handle, &still_running);
//         printf("running: %d!\n", still_running);
//         break;
//       }
//     } while(still_running);
//  
//     curl_multi_cleanup(multi_handle);
//  
//     /* always cleanup */ 
//     curl_easy_cleanup(curl);
//  
//     /* then cleanup the formpost chain */ 
//     curl_formfree(formpost);
//  
//     /* free slist */ 
//     curl_slist_free_all (headerlist);
//   }
// 	return 0;
}

struct DownloadData 
{
	CURL* http_handle;
	FILE* fp;
};

size_t WriteData( void* data, size_t size, size_t nmemb, void* pClientData )
{
	size_t dwWriten = size * nmemb;
	int statusCode = 0;
	DownloadData* pDownloadData = (DownloadData*)pClientData;
	CURLcode res = curl_easy_getinfo( pDownloadData->http_handle, CURLINFO_RESPONSE_CODE, &statusCode );
	if ( statusCode/100  != 2 ) //not range in 2**
	{
		return 0;
	}
// 	if ( pThreadData->rangeFrom + dwWriten > pThreadData->rangeTo )
// 	{
// 		LOG_V_W( _T("recv more data [status code %d]"), statusCode );
// 		return 0;
// 	
	int writed = fwrite(data, 1, dwWriten, pDownloadData->fp);
	return dwWriten;
}

UINT DownloadFile(const char* url, const char* path)
{
	long long size;
	int respone_code;
	if(!GetFileSize(url, size, respone_code))
	{
		return 0;
	}

	if (size <= 0)
	{
		return 0;
	}
	DownloadData dd;
	dd.fp = fopen(path, "wb");
RUN:
	//error_reason_ = HDErrorInternal;
	ULONGLONG totalBytes2Download = size;
	CURLM* multi_handle = curl_multi_init();
	//std::string url_a = url;
	//char* url = const_cast<char*> ( url_a.c_str() );
	CURL* http_handle = curl_easy_init();
	dd.http_handle = http_handle;
	curl_easy_setopt( http_handle, CURLOPT_URL, url );
	char opt_range[100] = { 0 };
	long long begin = 0;
	long long end = size;
	sprintf_s( opt_range, 100, "%lld-%lld", begin, end );
	curl_easy_setopt( http_handle, CURLOPT_RANGE, opt_range );
	curl_easy_setopt( http_handle, CURLOPT_WRITEFUNCTION, WriteData );
	curl_easy_setopt( http_handle, CURLOPT_WRITEDATA, &dd );
	curl_easy_setopt( http_handle, CURLOPT_MAXREDIRS, -1 );
	curl_easy_setopt( http_handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt( http_handle, CURLOPT_)

	curl_multi_add_handle( multi_handle, http_handle );
	const int nPeekIntervalInMs = 20;//ms
	const int nTimeout4NoDataInSec = 60;//s
	const int nMaxFailedCount = nTimeout4NoDataInSec * 1000 / nPeekIntervalInMs;
	int nSuccessiveFailedCount = 0;
	int still_running = 0;
	while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &still_running) );
	while( still_running/* && !exit_flag_is_set_ */)
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
		//	LOG_V_E( _T("socket error:[%d] returned by select func"), WSAGetLastError() );
		//	exit_flag_is_set_ = true;
			break;
		case 0://timeout
			++nSuccessiveFailedCount;
			if ( nSuccessiveFailedCount > nMaxFailedCount )
			{
			//	LOG_V_T( _T("HttpWebFileClient.timeout to download in HTTP") );
			//	exit_flag_is_set_ = true;
			//	error_reason_ = HDErrorTimeout;
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
	int last_http_status_code = 0;
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
	fclose(dd.fp);
// 	bool bOk = totalBytes2Download == acutalDownloadBytes && (last_http_status_code_ /100 == 2 ) &&
// 		(thread_data_.rangeTo == thread_data_.rangeFrom );
// 	if ( bOk )
// 	{
// 		error_reason_ = HDErrorNone;
// 	}
// 	if ( NULL != listener_ptr_ )
// 	{
// 		bool retry = false;
// 		listener_ptr_->OnDownloadFinished( this, thread_data_.pClientData, bOk , retry );
// 		if ( retry )
// 		{
// 			exit_flag_is_set_ = false;
// 			Sleep(2000);
// 			goto RUN;
// 		}
// 	}

	return 0;
}