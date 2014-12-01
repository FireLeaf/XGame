#include "stdafx.h"
#include "http_webclient.h"
#include "libcurl/curl.h"
using namespace base;

HttpWebFileClient::HttpWebFileClient( const tstring& url ):
url_( url ),
listener_ptr_( NULL ),
exit_flag_is_set_( false ),
last_http_status_code_( 0 ),
error_reason_( HDErrorNone )
{

}

HttpWebFileClient::~HttpWebFileClient()
{
    StopIfRunning();
}

void HttpWebFileClient::StopIfRunning()
{
    exit_flag_is_set_ = true;
    if ( IsRuning() )
    {
        Join();
        error_reason_ = HDErrorInterrupt;
    }
}

bool HttpWebFileClient::QueryFileSize( const tstring& url, ULONGLONG& totalSize, int& nResponseCode )
{
    CURL* http_handle = curl_easy_init();
    totalSize = 0;
    CURLcode res = CURLE_OK;
    bool isOk = false;
    do{
        std::string url_a = StringUtil::ToString( url );
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

        if ( 200 != nResponseCode )
        {
            LOG_V_W( _T("HttpCode:%d"), nResponseCode );
            break;
        }

        double sz = 0.0f;
        res = curl_easy_getinfo( http_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &sz );
        if ( CURLE_OK != res ) break;

        totalSize = static_cast<ULONGLONG>(sz);

        isOk = true;
    }while ( false );

    if ( CURLE_OK !=  res )
    {
        nResponseCode = res;
    }
    return isOk;
}

bool HttpWebFileClient::QueryFileSize( const tstring& url, ULONGLONG& totalSize )
{
    int nTempResponseCode = 0;
    return QueryFileSize( url, totalSize, nTempResponseCode );
}

bool HttpWebFileClient::BeginDownloadAsync( ULONGLONG rangeFrom, ULONGLONG rangeTo, IHttpWebFileDownloadListener* listener, void* pClientData )
{
    thread_data_.rangeFrom = rangeFrom;
    thread_data_.rangeTo = rangeTo;
    thread_data_.pThis = this;
    thread_data_.pClientData = pClientData;
    listener_ptr_ = listener;
    Start();
    return true;
}

bool HttpWebFileClient::ReDownloadAsync()
{
    exit_flag_is_set_ =false;
    last_http_status_code_ = 0;
    error_reason_ = HDErrorNone;
    Start();
    return true;
}

size_t HttpWebFileClient::WriteData( void* data, size_t size, size_t nmemb, void* pClientData )
{
    size_t dwWriten = size * nmemb;
    InnerThreadPara* pThreadData = (InnerThreadPara*)pClientData;
    int statusCode = 0;
    CURLcode res = curl_easy_getinfo( pThreadData->http_handle, CURLINFO_RESPONSE_CODE, &statusCode );
    if ( statusCode/100  != 2 ) //not range in 2**
    {
        return 0;
    }
    if ( pThreadData->rangeFrom + dwWriten > pThreadData->rangeTo )
    {
        LOG_V_W( _T("recv more data [status code %d]"), statusCode );
        return 0;
    }
    pThreadData->rangeFrom += dwWriten;
    HttpWebFileClient* pThis = pThreadData->pThis;
    if ( NULL != pThis->listener_ptr_ )
    {
        if ( !pThis->listener_ptr_->OnRecvData( pThis, pThreadData->pClientData, data, dwWriten ) )
        {
            LOG_V_W( _T("cancel recv data") );
            return 0;
        }
    }
    return dwWriten;
}

UINT HttpWebFileClient::Run()
{
RUN:
    error_reason_ = HDErrorInternal;
    ULONGLONG totalBytes2Download = thread_data_.rangeTo - thread_data_.rangeFrom;
    CURLM* multi_handle = curl_multi_init();
    std::string url_a = StringUtil::ToString( url_ );
    char* url = const_cast<char*> ( url_a.c_str() );
    CURL* http_handle = curl_easy_init();
    thread_data_.http_handle = http_handle;
    curl_easy_setopt( http_handle, CURLOPT_URL, url );
    char opt_range[100] = { 0 };
    sprintf_s( opt_range, 100, "%lld-%lld", thread_data_.rangeFrom, thread_data_.rangeTo - 1 );
    curl_easy_setopt( http_handle, CURLOPT_RANGE, opt_range );
    curl_easy_setopt( http_handle, CURLOPT_WRITEFUNCTION, WriteData );
    curl_easy_setopt( http_handle, CURLOPT_WRITEDATA, &thread_data_ );
    curl_easy_setopt( http_handle, CURLOPT_MAXREDIRS, -1 );
    curl_easy_setopt( http_handle, CURLOPT_FOLLOWLOCATION, 1);

    curl_multi_add_handle( multi_handle, http_handle );
    const int nPeekIntervalInMs = 20;//ms
    const int nTimeout4NoDataInSec = 60;//s
    const int nMaxFailedCount = nTimeout4NoDataInSec * 1000 / nPeekIntervalInMs;
    int nSuccessiveFailedCount = 0;
    int still_running = 0;
    while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &still_running) );
    while( still_running && !exit_flag_is_set_ )
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
            LOG_V_E( _T("socket error:[%d] returned by select func"), WSAGetLastError() );
            exit_flag_is_set_ = true;
            break;
        case 0://timeout
            ++nSuccessiveFailedCount;
            if ( nSuccessiveFailedCount > nMaxFailedCount )
            {
                LOG_V_T( _T("HttpWebFileClient.timeout to download in HTTP") );
                exit_flag_is_set_ = true;
                error_reason_ = HDErrorTimeout;
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

    CURLcode res = curl_easy_getinfo(http_handle, CURLINFO_RESPONSE_CODE, &last_http_status_code_);
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
    bool bOk = totalBytes2Download == acutalDownloadBytes && (last_http_status_code_ /100 == 2 ) &&
        (thread_data_.rangeTo == thread_data_.rangeFrom );
    if ( bOk )
    {
        error_reason_ = HDErrorNone;
    }
    if ( NULL != listener_ptr_ )
    {
        bool retry = false;
        listener_ptr_->OnDownloadFinished( this, thread_data_.pClientData, bOk , retry );
        if ( retry )
        {
            exit_flag_is_set_ = false;
            Sleep(2000);
            goto RUN;
        }
    }

    return 0;
}

size_t WriteDatatNone( void* data, size_t size, size_t nmemb, void* pClientData )
{
    size_t dwWriten = size * nmemb;
    return dwWriten;
}

bool HttpWebFileClient::GetMaxConnectionNumber( const tstring& url, int& connCount )
{
    ULONGLONG totalSize;
    if ( !QueryFileSize( url, totalSize ) )
        return false;
    if ( totalSize == 0)
    {
        connCount = 0;
        return true;
    }

    if ( totalSize < 1024*1024) //<=1M than use 1 thread
    {
        connCount = 1;
        return true;
    }

    CURL* http_handle = curl_easy_init();
    CURLcode res = CURLE_OK;
    std::string url_a = StringUtil::ToString( url );
    char* url_c = const_cast<char*> ( url_a.c_str() );
    curl_easy_setopt( http_handle, CURLOPT_URL, url_c );
    char opt_range[100] = { 0 };
    sprintf_s( opt_range, 100, "%lld-%lld",totalSize/2, totalSize/2 );
    curl_easy_setopt( http_handle, CURLOPT_RANGE, opt_range );
    curl_easy_setopt( http_handle, CURLOPT_WRITEFUNCTION, WriteDatatNone );
    curl_easy_setopt( http_handle, CURLOPT_WRITEDATA, &connCount );
    curl_easy_setopt( http_handle, CURLOPT_MAXREDIRS, -1);
    curl_easy_setopt( http_handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_perform( http_handle );
    int status_code = 0;
    curl_easy_getinfo(http_handle, CURLINFO_RESPONSE_CODE, &status_code);
    if ( 206 != status_code )
    {
        curl_easy_cleanup( http_handle );
        connCount = 1;
        return true;
    }

    //Try to query the max connection number
    curl_easy_reset( http_handle );
    return true;

}