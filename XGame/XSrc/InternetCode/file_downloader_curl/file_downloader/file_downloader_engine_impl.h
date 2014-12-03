#pragma once
#include "file_downloader_engine.h"
#include <map>
#include <queue>
#include <list>
#include "FileDownloaderThread.h"
#include "file_downloader_cfg.h"

struct CurlFileDownloadPendingTask
{
    long taskId;
    IFileDownloadListener* pListener;
    bool bNewDownload;
    FileDownloadRequestInfo* pRequsetInfo;
    FileDownloadCfg* pTaskCfgFile;

    CurlFileDownloadPendingTask():
    pListener ( NULL ),
    bNewDownload( true ),
    pRequsetInfo( NULL ),
    pTaskCfgFile( NULL )
    {

    }

    ~CurlFileDownloadPendingTask()
    {
        delete pRequsetInfo;
        delete pTaskCfgFile;
    }
};

struct CurlFileDownloadTaskContext
{
    base::CriticalSection mutex;
    long taskId;
    IFileDownloadListener* pListener;
    FileDownloaderThread* pHttpClient;
    bool bEndFlag; // true for end boundary ,false is just data
    bool bSuccessful;
    int  nErrorCode;
    unsigned long long nRecvSize;
    unsigned long long nTotalSize;
    CurlFileDownloadTaskContext():
    pListener ( NULL ),
    pHttpClient( NULL ),
    bEndFlag( false ),
    bSuccessful( false ),
    nRecvSize( 0 ),
    nTotalSize( 0 ),
    nErrorCode( 0 )
    {

    }

    ~CurlFileDownloadTaskContext()
    {
    }
};

class CPFileDownloadEngine : public FileDownloadEngine, private ICPFileDownloaderListener, private base::Thread
{
public:
    CPFileDownloadEngine();

    ~CPFileDownloadEngine();

    virtual bool InitDownloadEngine();

    virtual bool UninitDownloadEngine();

    virtual bool BeginDownload( const FileDownloadRequestInfo& requsetInfo, long& taskId,  IFileDownloadListener* listener );

    virtual bool IsExistUnCompleteTask( const tstring& expectedFile );

    virtual bool DeleteUnCompleteTask( const tstring& expectedFile );

    virtual bool ContinueFromUnCompleteTask( const tstring& expectedFile, long& taskId ,IFileDownloadListener* listener );

    virtual bool EndDownload( long taskId );

private:
    virtual UINT Run();
    void ProcessTaskCreate();
    void ProcessEngineStop();
    void ProcessTaskDownloading();

    static tstring GetTaskFileOfDownload( const tstring& file2download );
    static void DeleteTaskFileOf( const FileDownloadRequestInfo& requsetInfo );
    static int GenTaskID();
    //interface for ICPFileDownloaderListener
    virtual void OnDownloadFinished( void* pClientData, bool success, int errorCode );
    virtual void OnRecvData( void* pClientData, unsigned long long recvSize, unsigned long long allSize );
private:
    HANDLE exit_event_handle_;

    HANDLE new_download_task_event_handle_;
    HANDLE update_progress_event_handle_;

    base::CriticalSection cs_4_queue_;
    std::list< CurlFileDownloadPendingTask* > pending_tasks_queue_;
    std::map<long, CurlFileDownloadTaskContext*> running_tasks_map_;
};



