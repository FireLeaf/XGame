#include "stdafx.h"
#include "file_downloader_api.h"
#include "file_downloader_error.h"
#include "file_downloader.h"
#include "FileDownloaderThread.h"

int InitDownloadEngine(const TCHAR* lpszCurlPath)
{
    FileDownloaderThread::InitCurlExePath(lpszCurlPath);

    if ( !FileDownloader::GetInstance().Init() )
    {
        return FDL_ERROR_INIT;
    }
    return FDL_ERROR_SUCCESS;
}

int UninitDownloadEngine()
{
    FileDownloader::GetInstance().Release();
    return FDL_ERROR_SUCCESS;
}

int BeginDownload( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs, int *pnTaskId )
{
    if ( NULL == lpszUrl || NULL == lpszSaveAs )
        return FDL_ERROR_INVALID_PARAS;
    FileDownloadRequestInfo reqInfo;
    reqInfo.Url = lpszUrl;
    reqInfo.SaveAs = lpszSaveAs;
    long lTaskId = -1;
    if ( !FileDownloader::GetInstance().BeginDownload( reqInfo, lTaskId) )
    {
        return FDL_ERROR_FAILED;
    }
    if ( NULL != pnTaskId )
        *pnTaskId = lTaskId;

    return FDL_ERROR_SUCCESS;
}

int IsExistUnCompleteTask( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs, bool *pResult )
{
    bool bExist = FileDownloader::GetInstance().IsExistUnCompleteTask( lpszSaveAs );

    if (pResult)
        *pResult = bExist ? TRUE : false;

    return FDL_ERROR_SUCCESS;
}

int DeleteUnCompleteTask( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs )
{
    if ( NULL == lpszSaveAs )
        return FDL_ERROR_INVALID_PARAS;

    if (!FileDownloader::GetInstance().DeleteUnCompleteTask( lpszSaveAs ))
    {
        return FDL_ERROR_FAILED;
    }
    return FDL_ERROR_SUCCESS;
}

int ContinueFromUnCompleteTask( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs, int *pnTaskId )
{
    if ( NULL == lpszSaveAs )
        return FDL_ERROR_INVALID_PARAS;

    long lTaskId = -1;
    if (!FileDownloader::GetInstance().ContinueFromUnCompleteTask( lpszSaveAs, lTaskId ))
    {
        return FDL_ERROR_FAILED;
    }

    if ( NULL != pnTaskId )
        *pnTaskId = lTaskId;
    return FDL_ERROR_SUCCESS;
}

int EndDownload( int nTaskId )
{
    FileDownloader::GetInstance().EndDownload( nTaskId );
    return FDL_ERROR_SUCCESS;
}

int QueryTaskInfo( int nTaskId, int *pStatus, int *pErrorCode, unsigned long long *pTotalSize, unsigned long long *pRecvSize )
{
    int nStatus;
    int nErrorCode;
    unsigned long long ullTotalSize;
    unsigned long long ullRecvSize;
    if (!FileDownloader::GetInstance().QueryTaskInfo(nTaskId, nStatus, nErrorCode, ullTotalSize, ullRecvSize) )
        return FDL_ERROR_FAILED;

    if (pStatus) *pStatus = nStatus;
    if (pErrorCode) *pErrorCode = nErrorCode;
    if (pTotalSize) *pTotalSize = ullTotalSize;
    if (pRecvSize) *pRecvSize = ullRecvSize;

    return FDL_ERROR_SUCCESS;
}
