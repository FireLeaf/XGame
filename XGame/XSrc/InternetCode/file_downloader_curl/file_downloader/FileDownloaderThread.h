#pragma once
#include "stdafx.h"
#include "file_downloader_cfg.h"
#include <queue>
#include <vector>
#include "file_downloader_error.h"
#include "base/process_util.h"
#include "base/thread_util.h"

class ICPFileDownloaderListener
{
public:
    virtual void OnDownloadFinished( void* pClientData, bool success , int errorCode ) = 0;
    virtual void OnRecvData( void* pClientData, unsigned long long recvSize, unsigned long long allSize ) = 0;
};

struct CPDownloadProcContext
{
    base::Process* pProcess;
    unsigned long long nRangeFrom;
    unsigned long long nRangeTo;
    unsigned long long nRangeCur;
    float fPercent;
    tstring strFile;
    CPDownloadProcContext():
    pProcess( NULL ),
    nRangeFrom( 0 ),
    nRangeTo( 0 ),
    nRangeCur( 0 ),
    fPercent( 0.0f )
    {

    }
};

class FileDownloaderThread : public base::Thread
{
    static const int nMaxBytesPerRange = 10 * 1024 * 1024;//bytes
    static const int nMaxInstanceOfCurl = 8;
    static const int nReportIntervalsInSeconds = 1;
public:
    static bool InitCurlExePath( const tstring& exePath );
    static void KillCurlProcessesIfRunning();

    FileDownloaderThread();

    ~FileDownloaderThread();

    bool BeginDownloadAsync( const FileDownloadCfg& taskFile,
        ICPFileDownloaderListener* pListener, void* pClientData );

    bool BeginDownloadAsync( const tstring& url,
        const tstring& strSaveAs, ICPFileDownloaderListener* pListener, void* pClientData );

    void StopIfRunning();

    static bool IsExistUnCompleteTask( const tstring& targetFile );
    static bool DeleteUnCompleteTask( const tstring& targetFile );

    int GetLastDownloadError() const { return error_code_; }
private:
    //Thread entry point
    virtual UINT Run();

    void CommitLastRangeFilesIfExist();

    bool GetRangesQueue2Download( std::queue< FileRange >& smallRanges );

    bool LaunchCurl2DownloadRange( const FileRange& range2Download, CPDownloadProcContext& runningDownloadInst );

    bool Wait4CurlExit( const std::vector< CPDownloadProcContext >& runningDownloadInsts, DWORD dwTimeout, size_t& index );

    unsigned long long CommitRangeFile( const tstring& strRangeFile, const unsigned long long& rangeFrom );

    static void PeekDownloadPercent( std::vector< CPDownloadProcContext >& dlInstCxts );
    void NotifyDownloadProgress( std::vector< CPDownloadProcContext >& dlInstCxts );

    tstring GetDirectoryOfRanges2Download() const;

private:
    static tstring s_strExePath;
    bool exit_flag_is_set_;
    base::CriticalSection cs_4_listener_;
    ICPFileDownloaderListener* listener_ptr_;
    void* client_data_ptr_;
    FileDownloadCfg download_cfg_;
    unsigned long long last_recv_data_size_;
    int error_code_;
};
