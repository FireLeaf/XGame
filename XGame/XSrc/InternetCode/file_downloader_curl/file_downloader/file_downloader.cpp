#include "file_downloader.h"
#include "file_downloader_engine_impl.h"
#include "libcurl/curl.h"

using namespace base;

FileDownloader* g_FileDownloader = NULL;
FileDownloader& FileDownloader::GetInstance()
{
    if ( NULL == g_FileDownloader )
    {
        g_FileDownloader = new FileDownloader();
    }
    return *g_FileDownloader;
}

bool FileDownloader::Init()
{
    curl_global_init( CURL_GLOBAL_WIN32 );
    return true;
}

void FileDownloader::Release()
{
    g_FileDownloader = NULL;
    delete this;
    curl_global_cleanup();
}

FileDownloader::FileDownloader()
{
    download_engine_ptr_ = new CPFileDownloadEngine();
    download_engine_ptr_->InitDownloadEngine();
}

FileDownloader::~FileDownloader()
{
    download_engine_ptr_->UninitDownloadEngine();
    delete download_engine_ptr_;
}

bool FileDownloader::BeginDownload( const FileDownloadRequestInfo& requsetInfo, long& taskId )
{
    bool ret = download_engine_ptr_->BeginDownload( requsetInfo, taskId, this );
    if ( ret )
    {
        LOG_V_D( _T("start download task(%d)"), taskId );
        SmartLockT<CriticalSection> lockTasks( cs_4_tasks_ );
        FLTaskInfo& taskInfo = all_task_infos_[taskId];
        taskInfo.nDownloadStatus = 0;
    }
    else
        LOG_V_E( _T("fail to start download task %s"), requsetInfo.Url.c_str() );

    return ret;
}

bool FileDownloader::IsExistUnCompleteTask( const tstring& expectedFile )
{
    return download_engine_ptr_->IsExistUnCompleteTask( expectedFile );
}

bool FileDownloader::DeleteUnCompleteTask( const tstring& expectedFile )
{
    return download_engine_ptr_->DeleteUnCompleteTask( expectedFile );
}

bool FileDownloader::ContinueFromUnCompleteTask( const tstring& expectedFile, long& taskId )
{
    bool ret = download_engine_ptr_->ContinueFromUnCompleteTask( expectedFile, taskId, this );
    if (ret)
    {
        LOG_V_D( _T("start download task(%d) from last point"), taskId );
        SmartLockT<CriticalSection> lockTasks( cs_4_tasks_ );
        FLTaskInfo& taskInfo = all_task_infos_[taskId];
        taskInfo.nDownloadStatus = 0;
    }

    return ret;
}

bool FileDownloader::EndDownload( long taskId )
{
    if ( taskId < 0 )
    {
        LOG_V_T( _T("nothing to do for invalid task (%d)"), taskId );
        return true;
    }
    
    SmartLockT<CriticalSection> lockTasks( cs_4_tasks_ );
    LOG_V_D( _T("end download task(%d)"), taskId );
    bool ret = download_engine_ptr_->EndDownload( taskId );
    LOG_V_D( _T("ended") );
    all_task_infos_.erase( taskId );
    return ret;
}

void FileDownloader::OnDownloadProgressChanged( long taskId, unsigned long long recvSize, unsigned long long totalFileSize )
{
    SmartLockT<CriticalSection> lockTasks( cs_4_tasks_ );
    FLTaskInfo& taskInfo = all_task_infos_[taskId];
    taskInfo.nDownloadStatus = 1;
    taskInfo.ullTotalSize = totalFileSize;
    taskInfo.ullRecvSize = recvSize;
}

void FileDownloader::OnDownloadFinished( long taskId, bool result, int errorCode )
{
    SmartLockT<CriticalSection> lockTasks( cs_4_tasks_ );
    FLTaskInfo& taskInfo = all_task_infos_[taskId];
    taskInfo.nDownloadStatus = 2;
    taskInfo.nFinalResult = result ? 0 : errorCode;
}

bool FileDownloader::QueryTaskInfo( int nTaskId, int &nStatus, int &nErrorCode, unsigned long long &nTotalSize, unsigned long long &nRecvSize )
{
    SmartLockT<CriticalSection> lockTasks( cs_4_tasks_ );
    if (all_task_infos_.find( nTaskId ) == all_task_infos_.end() )
        return false;

    FLTaskInfo& taskInfo = all_task_infos_[nTaskId];
    nStatus = taskInfo.nDownloadStatus;
    nErrorCode = taskInfo.nFinalResult;
    nTotalSize = taskInfo.ullTotalSize;
    nRecvSize = taskInfo.ullRecvSize;
    return true;
}