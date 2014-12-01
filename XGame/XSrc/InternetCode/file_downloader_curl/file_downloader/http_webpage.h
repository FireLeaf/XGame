#pragma once
#include "base/string_util.h"
#include "base/thread_util.h"

class IHttpWebPageDownloadListener
{
public:
    virtual void OnDownloadFinished( bool success ) = 0;
};

class HttpStringDownloadThread;
class http_webpage
{
    friend class HttpStringDownloadThread;
    enum HttpQueryStatus
    {
        HQS_Unstarted,
        HQS_Downloading,
        RDS_FinishedWithError,
        RDS_FinishedWithOK
    };
    static const long DL_LOW_SPPED_LIMIT = 1000;//1K/sec
    static const long DL_LOW_SPEED_TIME = 15; //sec
public:
    http_webpage( const tstring& url );
    ~http_webpage();

    bool DownloadString( tstring& content );
    //ªÒ»°UTF8◊÷∑˚¥Æ
    bool DownloadUTF8String(std::string & strContent);
    bool DownloadStringAsync( IHttpWebPageDownloadListener* pListener );
    bool GetResponse( tstring& content );
    int  GetLastStatusCode() const { return error_code_; }
    void Abort();
    void SetCookie(const tstring &strCookie);
private:
    static size_t WriteDataToString( void* data, size_t size, size_t nmemb, void* p );
    bool DownloadStringImpl();
    void Reset();
    void FinishDownloadStringAsync();
private:
    HttpQueryStatus query_status_;
    tstring url_;
    std::string response_;
    IHttpWebPageDownloadListener* listener_ptr_;
    HttpStringDownloadThread* download_thread_ptr_;
    bool  exit_flag_is_set_;
    int   error_code_;
    base::CriticalSection cs_4_data_;

    //cookie…Ë÷√
    bool  cookie_enabled_;
    tstring cookie_str_;
};

