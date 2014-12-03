#include "stdafx.h"
#include "file_downloader_engine_impl.h"
#include "file_downloader.h"
using namespace base;

CPFileDownloadEngine::CPFileDownloadEngine()
{
    new_download_task_event_handle_ = CreateEvent( NULL, false, false, NULL );
    update_progress_event_handle_ = CreateEvent( NULL, false, false, NULL );
    exit_event_handle_ = CreateEvent( NULL, false, false, NULL );
}

CPFileDownloadEngine::~CPFileDownloadEngine()
{
    CloseHandle( exit_event_handle_ );
    CloseHandle( update_progress_event_handle_ );
    CloseHandle( new_download_task_event_handle_ );
}

bool CPFileDownloadEngine::InitDownloadEngine()
{
    //Try to stop xiaomi_curl.exe
    //FileDownloaderThread::InitCurlExePath();
    FileDownloaderThread::KillCurlProcessesIfRunning();
    Start();//Start Listen
    return true;
}

bool CPFileDownloadEngine::UninitDownloadEngine()
{
    if ( !IsRuning() )
    {
        return true;
    }
    SetEvent( exit_event_handle_ );
    Join();
    return true;
}

int CPFileDownloadEngine::GenTaskID()
{
    static long taskIdBase = 0;
    return taskIdBase++;
}

bool CPFileDownloadEngine::BeginDownload( const FileDownloadRequestInfo& requsetInfo, long& taskId,  IFileDownloadListener* listener )
{
    CurlFileDownloadPendingTask* pNewTask = new CurlFileDownloadPendingTask();
    pNewTask->taskId = taskId = GenTaskID();
    pNewTask->bNewDownload = true;
    pNewTask->pRequsetInfo = new FileDownloadRequestInfo();
    *(pNewTask->pRequsetInfo) = requsetInfo;
    pNewTask->pListener = listener;
    SmartLockT<CriticalSection> lockQueue( cs_4_queue_ );
    pending_tasks_queue_.push_back( pNewTask );
    SetEvent( new_download_task_event_handle_ );
    return true;
}

bool CPFileDownloadEngine::IsExistUnCompleteTask( const tstring& expectedFile )
{
    return FileDownloaderThread::IsExistUnCompleteTask( expectedFile );
}

bool CPFileDownloadEngine::DeleteUnCompleteTask( const tstring& expectedFile )
{
    return FileDownloaderThread::DeleteUnCompleteTask( expectedFile );
}

bool CPFileDownloadEngine::ContinueFromUnCompleteTask( const tstring& expectedFile, long& taskId ,IFileDownloadListener* listener )
{
    FileDownloadCfg* pTaskCfg = new FileDownloadCfg( expectedFile );
    if ( !pTaskCfg->ReadFromFile() )
    {
        delete pTaskCfg;
        return false;
    }

    CurlFileDownloadPendingTask* pNewTask = new CurlFileDownloadPendingTask();
    pNewTask->taskId = taskId = GenTaskID();
    pNewTask->bNewDownload = false;
    pNewTask->pTaskCfgFile = pTaskCfg;
    pNewTask->pListener = listener;
    SmartLockT<CriticalSection> lockQueue( cs_4_queue_ );
    pending_tasks_queue_.push_back( pNewTask );
    SetEvent( new_download_task_event_handle_ );
    return true;
}

bool CPFileDownloadEngine::EndDownload( long taskId )
{
    SmartLockT<CriticalSection> lockQueue( cs_4_queue_ );
    //if the task id still in the pending queue just remove it
    for ( std::list<CurlFileDownloadPendingTask*>::iterator itPendingTask = pending_tasks_queue_.begin();
        itPendingTask != pending_tasks_queue_.end(); ++itPendingTask )
    {
        CurlFileDownloadPendingTask* pNewTask = *itPendingTask;
        if ( NULL != pNewTask && pNewTask->taskId == taskId )
        {
            delete pNewTask;
            pending_tasks_queue_.erase( itPendingTask );
            return true;
        }
    }
    //if the task id in running queue, set end flag and quit
    std::map<long , CurlFileDownloadTaskContext*>::iterator itRunningTask = running_tasks_map_.find( taskId );
    if ( itRunningTask == running_tasks_map_.end() )
    {
        return true;
    }
     CurlFileDownloadTaskContext*  pTaskCxt = itRunningTask->second;
     running_tasks_map_.erase( itRunningTask );
     if ( NULL == pTaskCxt )
     {
         return true;
     }
    if ( NULL != pTaskCxt->pHttpClient )
        pTaskCxt->pHttpClient->StopIfRunning();//cancel and the client will not send any message here

    delete pTaskCxt->pHttpClient;
    LOG_V_D( _T("task %d was stopped"), taskId );
    delete pTaskCxt;

    return true;
}

void CPFileDownloadEngine::OnDownloadFinished( void* pClientData, bool success, int errorCode )
{
    CurlFileDownloadTaskContext *pTaskCxt = (CurlFileDownloadTaskContext *)pClientData;
    SmartLockT<CriticalSection> lockTask( pTaskCxt->mutex );
    pTaskCxt->bSuccessful = success;
    pTaskCxt->bEndFlag = true;
    pTaskCxt->nErrorCode = errorCode;
    SetEvent( update_progress_event_handle_ );
}

void CPFileDownloadEngine::OnRecvData( void* pcbData, unsigned long long recvSize, unsigned long long allSize )
{
    CurlFileDownloadTaskContext *pTaskCxt = (CurlFileDownloadTaskContext *)pcbData;
    SmartLockT<CriticalSection> lock( pTaskCxt->mutex );
    pTaskCxt->nRecvSize = recvSize;
    pTaskCxt->nTotalSize = allSize;
    SetEvent( update_progress_event_handle_ );
}

UINT CPFileDownloadEngine::Run()
{
    HANDLE hWaitEvents[] =
    {
        new_download_task_event_handle_,
        update_progress_event_handle_,
        exit_event_handle_
    };
    bool running = true;
    while ( running )
    {
        DWORD dwWaitIdx = WaitForMultipleObjects( sizeof( hWaitEvents )/ sizeof( hWaitEvents[0]), hWaitEvents, false, INFINITE );

        switch ( dwWaitIdx )
        {
        case WAIT_OBJECT_0 + 0://New Task
            ProcessTaskCreate();
            break;
        case WAIT_OBJECT_0 + 1://Write data
            ProcessTaskDownloading();
            break;
        case WAIT_OBJECT_0 + 2://exit
            ProcessEngineStop();
            running = false;
            break;
        default:
            LOG_V_E( _T("system error:%d"), GetLastError() );
        }
    }
    LOG_V_T( _T("CPFileDownloadEngine Monitor thread exit") );
    return 0;
}

void CPFileDownloadEngine::ProcessTaskCreate()
{
    SmartLockT<CriticalSection> lockQueue( cs_4_queue_ );
    if ( pending_tasks_queue_.empty() )
        return;

    while ( !pending_tasks_queue_.empty() )
    {
        CurlFileDownloadTaskContext* pTaskCxt = new CurlFileDownloadTaskContext();
        CurlFileDownloadPendingTask* pNewTask = pending_tasks_queue_.front();
        pending_tasks_queue_.pop_front();
        FileDownloaderThread* pHttpClient = new FileDownloaderThread();

        pTaskCxt->taskId = pNewTask->taskId;
        pTaskCxt->pListener = pNewTask->pListener;
        pTaskCxt->pHttpClient = pHttpClient;

        bool isStarted = false;
        if ( pNewTask->bNewDownload )
        {
            isStarted = pHttpClient->BeginDownloadAsync( pNewTask->pRequsetInfo->Url, pNewTask->pRequsetInfo->SaveAs, this, pTaskCxt );
        }
        else
        {
            isStarted = pHttpClient->BeginDownloadAsync( *(pNewTask->pTaskCfgFile), this, pTaskCxt );
        }
        delete pNewTask;
        pTaskCxt->bEndFlag = !isStarted;
        if ( !isStarted )//failed
        {
            pTaskCxt->nErrorCode = pHttpClient->GetLastDownloadError();
        }
        running_tasks_map_[ pTaskCxt->taskId ] = pTaskCxt;
    }// while ( !pending_tasks_queue_.empty() )
    SetEvent( update_progress_event_handle_ );
}

void CPFileDownloadEngine::ProcessTaskDownloading()
{
    SmartLockT<CriticalSection> lockQueue( cs_4_queue_ );

    for ( std::map<long, CurlFileDownloadTaskContext*>::iterator itTask = running_tasks_map_.begin();
        itTask != running_tasks_map_.end();)
    {
        long taskId = itTask->first;
        CurlFileDownloadTaskContext* pTaskCxt = itTask->second;
        if ( NULL == pTaskCxt )
        {
            running_tasks_map_.erase( itTask++ );
            continue;
        }
        bool bDeleteTask = false;
       {
           SmartLockT<CriticalSection> lock( pTaskCxt->mutex );
           unsigned long long totalRecvSize = pTaskCxt->nRecvSize;
           unsigned long long totalFileSize = pTaskCxt->nTotalSize;
           IFileDownloadListener* pListener = pTaskCxt->pListener;
           int nErrorCode = pTaskCxt->nErrorCode;
           if ( totalFileSize > 0 && NULL != pListener )
               pListener->OnDownloadProgressChanged( taskId, totalRecvSize, totalFileSize );
           if ( pTaskCxt->bEndFlag )
           {
               bool bOK = pTaskCxt->bSuccessful;
               if ( NULL != pListener )
               {
                   pListener->OnDownloadFinished( taskId , bOK, nErrorCode );
               }
               bDeleteTask = true;
           }
       }
       if ( bDeleteTask )
       {
           delete pTaskCxt;
           itTask->second = NULL;
           running_tasks_map_.erase( itTask++ );
       }
       else
       {
           ++itTask;
       }
    }
}

void CPFileDownloadEngine::ProcessEngineStop()
{
    SmartLockT<CriticalSection> lockQueue( cs_4_queue_ );
    if ( pending_tasks_queue_.size() > 0 )
    {
        LOG_V_W( _T("Suggest to call EndDownload before quit. There is/are still %d task(s) that pending to run"),
            (int) pending_tasks_queue_.size() );
    }
   //Clear pending task queue
    while ( !pending_tasks_queue_.empty() )
    {
        CurlFileDownloadPendingTask* pNewTask = pending_tasks_queue_.front();
        if ( NULL != pNewTask )
        {
            LOG_V_D( _T("task %d removed."), pNewTask->taskId );
            delete pNewTask;
        }
        pending_tasks_queue_.pop_front();
    }

    if ( running_tasks_map_.size() > 0 )
    {
        LOG_V_W( _T("Suggest to call EndDownload before quit. There is/are still %d task(s) that running"),
            (int) running_tasks_map_.size() );
    }
    //stop all running threads
    std::map<long , CurlFileDownloadTaskContext*>::iterator it = running_tasks_map_.begin();
    for ( ; it != running_tasks_map_.end(); ++it )
    {
        CurlFileDownloadTaskContext* pTaskCxt = it->second;
        if ( NULL != pTaskCxt )
        {
            pTaskCxt->pHttpClient->StopIfRunning();
            delete pTaskCxt->pHttpClient;
            pTaskCxt->pHttpClient = NULL;
            LOG_V_D( _T("task %d removed."), pTaskCxt->taskId );
            delete pTaskCxt;
        }
    }
    running_tasks_map_.clear();
}