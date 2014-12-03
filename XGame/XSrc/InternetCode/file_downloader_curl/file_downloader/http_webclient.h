#pragma once
#include "base/thread_util.h"
#include "base/string_util.h"

class HttpWebFileClient;
class IHttpWebFileDownloadListener
{
public:
    virtual void OnDownloadFinished( HttpWebFileClient* pClient, void* pClientData, bool success, bool& retry ) = 0;
    virtual bool OnRecvData( HttpWebFileClient* pClient, void* pClientData, void* data, size_t size ) = 0;
};

enum HttpDownloadErrorReason
{
    HDErrorNone = 0,
    HDErrorTimeout = 1,
    HDErrorInterrupt = 2,
    HDErrorInternal = 3,
};

class HttpWebFileClient : public base::Thread
{
public:
    HttpWebFileClient( const tstring& url );

    ~HttpWebFileClient();

    static bool QueryFileSize( const tstring& url, unsigned long long& totalSize );
    static bool QueryFileSize( const tstring& url, unsigned long long& totalSize, int& nResponseCode );

    bool BeginDownloadAsync( unsigned long long rangeFrom, unsigned long long rangeTo, IHttpWebFileDownloadListener* listener, void* pClientData );

    bool ReDownloadAsync();

    void StopIfRunning();

    static bool GetMaxConnectionNumber( const tstring& url, int& connCount );

    int GetLastStatusCode() const { return last_http_status_code_ ;}
    HttpDownloadErrorReason GetErrorReason() const { return error_reason_; }
private:
    //Thread entry point
    virtual UINT Run();
    static size_t WriteData( void* data, size_t size, size_t nmemb, void* pClientData );
private:
    struct InnerThreadPara
    {
        HttpWebFileClient* pThis;
        void* pClientData;
        unsigned long long rangeFrom;
        unsigned long long rangeTo;
        void* http_handle;
    };
    tstring url_;
    InnerThreadPara thread_data_;
    IHttpWebFileDownloadListener* listener_ptr_;
    bool exit_flag_is_set_;
    int last_http_status_code_;
    HttpDownloadErrorReason error_reason_;
};
