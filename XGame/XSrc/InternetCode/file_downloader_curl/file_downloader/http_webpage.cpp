#include "stdafx.h"
#include "http_webpage.h"
#include "curl/curl.h"
#include "file_downloader_error_util.h"
#include "base/thread_util.h"

using namespace base;

class HttpStringDownloadThread : public Thread
{
public:
    HttpStringDownloadThread( http_webpage& webPage ):
    webpage_ref_( webPage )
    {
    }
    virtual UINT Run()
    {
        webpage_ref_.FinishDownloadStringAsync();
        return 0;
    }
private:
    http_webpage& webpage_ref_;
};


http_webpage::http_webpage( const tstring& url ):
url_( url ),
listener_ptr_( NULL ),
query_status_( HQS_Unstarted ),
download_thread_ptr_( NULL ),
exit_flag_is_set_( false ),
error_code_( 0 ),
cookie_enabled_( false )
{

}

http_webpage::~http_webpage()
{
    Abort();
}

void http_webpage::Reset()
{
    query_status_ = HQS_Unstarted;
    response_.clear();
    listener_ptr_ = NULL;
    error_code_ = 0;
    delete download_thread_ptr_;
    download_thread_ptr_ = NULL;
    exit_flag_is_set_ = false;
}

bool http_webpage::GetResponse( tstring& response )
{
    if (RDS_FinishedWithOK != query_status_ )
        return false;

   response = StringUtil::ToString( response_ );
   return true;
}

size_t http_webpage::WriteDataToString( void* data, size_t size, size_t nmemb, void* p )
{
    std::string* pString =  (std::string*)p;
    std::ostringstream oss;
    oss.write( (const char*) data, size * nmemb);
    *pString += oss.str();
    return size * nmemb;
}

bool http_webpage::DownloadStringImpl()
{
    CURLM* multi_handle = curl_multi_init();
    if ( NULL == multi_handle ){
        LOG_V_E( _T("curl_multi_init init failed.") );
        return false;
    }
    CURL* http_handle= curl_easy_init();
    if ( NULL == http_handle ){
        curl_multi_cleanup( multi_handle );
        LOG_V_E( _T("curl_easy_init init failed.") );
        return false;
    }

    std::string content_a;
    std::string url_a = StringUtil::ToString( url_ );
    curl_easy_setopt( http_handle, CURLOPT_URL,  const_cast<char*> ( url_a.c_str() ) );
    curl_easy_setopt( http_handle, CURLOPT_LOW_SPEED_LIMIT, DL_LOW_SPPED_LIMIT );
    curl_easy_setopt( http_handle, CURLOPT_LOW_SPEED_TIME, DL_LOW_SPEED_TIME );
    curl_easy_setopt( http_handle, CURLOPT_MAXREDIRS, -1 );
    curl_easy_setopt( http_handle, CURLOPT_FOLLOWLOCATION, 1 );
    curl_easy_setopt( http_handle, CURLOPT_WRITEFUNCTION, WriteDataToString );
    curl_easy_setopt( http_handle, CURLOPT_WRITEDATA, &content_a );
    std::string cookie_a = StringUtil::ToString( cookie_str_ );
    if (cookie_enabled_)
    {
        //ÉèÖÃtoken
        int rc = curl_easy_setopt( http_handle, CURLOPT_COOKIE, cookie_a.c_str() );
    }

    curl_multi_add_handle(multi_handle, http_handle );
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
         timeout.tv_usec = 20000;
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
            default:
            /* timeout or readable/writable sockets */
            while( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( multi_handle, &still_running ) );
            break;
        }
    }

    CURLcode complete_status = CURL_LAST;//default set with error
    //see how the transfer went
    int msgs_left = 0;
    CURLMsg *msg = NULL;
    while (  msg = curl_multi_info_read( multi_handle, &msgs_left ) )
    {
        if ( CURLMSG_DONE ==  msg->msg && msg->easy_handle == http_handle )
        {
            complete_status = msg->data.result;
            break;
        }
    }
    int nResponseCode = 0;
    if ( CURLE_OK == complete_status )
    {
        CURLcode res = curl_easy_getinfo(http_handle, CURLINFO_RESPONSE_CODE, &nResponseCode);
        if ( 200 == nResponseCode || 206 == nResponseCode )
        {
            response_ = content_a;
            error_code_ = FDL_ERROR_SUCCESS;
        }
        else
        {
            error_code_ = MAKE_NETWORK_ERROR( nResponseCode );
        }
    }
    else
    {
        error_code_ = FileDownloaderErrorUtil::ConvertCURLcode2FDLError( complete_status );
    }
    curl_multi_remove_handle( multi_handle, http_handle );
    curl_multi_cleanup( multi_handle );
    curl_easy_cleanup( http_handle );

    return FDL_ERROR_SUCCESS == error_code_;
}

bool http_webpage::DownloadString( tstring& content )
{
    Reset();
    query_status_ = HQS_Downloading;
    bool ret = DownloadStringImpl();
    if ( ret )
    {
        query_status_ = RDS_FinishedWithOK;
        GetResponse( content );
    }
    else
    {
        query_status_ = RDS_FinishedWithError;
    }
    return ret;
}

bool http_webpage::DownloadStringAsync( IHttpWebPageDownloadListener* pListener )
{
    {
        SmartLockT<CriticalSection> lockData( cs_4_data_ );
        Reset();
        listener_ptr_ = pListener;
    }
    query_status_ = HQS_Downloading;
    assert ( NULL == download_thread_ptr_ );
    download_thread_ptr_ = new HttpStringDownloadThread( *this );
    download_thread_ptr_->Start();
    return true;
}

void http_webpage::FinishDownloadStringAsync()
{
    bool ret = DownloadStringImpl();
    query_status_ = ret ? RDS_FinishedWithOK: RDS_FinishedWithError;
    download_thread_ptr_ = NULL; //The thread it will auto delete itself
    {
        SmartLockT<CriticalSection> lockData( cs_4_data_ );
        if ( NULL != listener_ptr_ )
        {
            listener_ptr_->OnDownloadFinished( ret );
        }
    }
}

void http_webpage::Abort()
{
    {
        SmartLockT<CriticalSection> lockData( cs_4_data_ );
        listener_ptr_ = NULL;
    }
    if ( NULL != download_thread_ptr_ && download_thread_ptr_->IsRuning() )
    {
        exit_flag_is_set_ = true;
        download_thread_ptr_->Join();
        Reset();
    }
}

bool http_webpage::DownloadUTF8String(std::string & strContent)
{
    Reset();
    query_status_ = HQS_Downloading;
    bool ret = DownloadStringImpl();
    if ( ret )
    {
        strContent = response_;
        query_status_ = RDS_FinishedWithOK;
    }
    else
    {
        query_status_ = RDS_FinishedWithError;
    }
    return ret;
}

void http_webpage::SetCookie(const tstring &strCookie)
{
    cookie_enabled_ = true;
    cookie_str_     = strCookie;
}
