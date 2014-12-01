#pragma once
#include <WinSock2.h>
#include <Windows.h>

#include "base/thread_util.h"
#include "file_downloader_engine.h"
#include <map>

struct FLTaskInfo
{
    int nDownloadStatus;//0(pending) 1 (downloading) 2 (finished)
    int nFinalResult;
    unsigned long long ullTotalSize;
    unsigned long long ullRecvSize;
};

class IFileDownloadListener
{
public:
    virtual ~IFileDownloadListener(){}
    virtual void OnDownloadProgressChanged( long taskId, unsigned long long recvSize, unsigned long long totalFileSize ) = 0;

    virtual void OnDownloadFinished( long taskId, bool result, int errorCode ) = 0;
};

class FileDownloader : private IFileDownloadListener
{
 public:
    static FileDownloader& GetInstance();
    bool Init();
    void Release();

    ~FileDownloader();

    bool BeginDownload( const FileDownloadRequestInfo& requsetInfo, long& taskId);

    bool IsExistUnCompleteTask( const tstring& expectedFile );

    bool DeleteUnCompleteTask( const tstring& expectedFile );

    bool ContinueFromUnCompleteTask( const tstring& expectedFile, long& taskId);

    bool EndDownload( long taskId );

    bool QueryTaskInfo( int nTaskId, int &Status, int &ErrorCode, unsigned long long &nTotalSize, unsigned long long &nRecvSize );

protected:
    virtual void OnDownloadProgressChanged( long taskId, unsigned long long recvSize, unsigned long long totalFileSize );

    virtual void OnDownloadFinished( long taskId, bool result, int errorCode );
private:
    FileDownloader();
private:
    FileDownloadEngine* download_engine_ptr_;
    base::CriticalSection cs_4_tasks_;
    std::map<long, FLTaskInfo> all_task_infos_;
};