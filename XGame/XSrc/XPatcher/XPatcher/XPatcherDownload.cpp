/**************************************************************************
/*		FileName: XPatcherDownload.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/12/01 20:42:14
/*		CopyRight: yc 
/*************************************************************************/
#include "XPatcherDownload.h"

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

		res = curl_easy_getinfo(http_handle, CURLINFO_RESPONSE_CODE, &nResponseCode);
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

bool XPatcherDownload::DownloadFile(const char* url)
{
	
}