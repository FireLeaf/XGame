#ifndef _FILE_DOWNLOADER_API_H
#define _FILE_DOWNLOADER_API_H
//#include "MiPhoneManagerShareLib/MiPhoneManagerErrorCode.h"
#include <Windows.h>
#include <tchar.h>
#ifndef FILEDOWNLOADER_STATIC_LIB
# ifdef FILEDOWNLOADER_EXPORTS
#  define FILEDOWNLOADER_API __declspec(dllexport)
# else
#  define FILEDOWNLOADER_API __declspec(dllimport)
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
* Init the download engine
* @Paras:
* @Returns:
*   FDL_ERROR_SUCCESS
*   ErrFdInitFailed
**/
FILEDOWNLOADER_API int
InitDownloadEngine(const TCHAR* lpszCurlPath);

/**
* UnInit the download engine
* @Paras:
* @Returns:
*   FDL_ERROR_SUCCESS
**/
FILEDOWNLOADER_API int
UninitDownloadEngine();

/**
* Begin a new url file download
* @Paras:
*  [IN] lpszUrl :   the url of a remote file
*  [IN] lpszSaveAs: local file name to be saved for url
*  [OUT] pnTaskId:  task id that related to this download
* @Returns:
*   FDL_ERROR_SUCCESS
*   FDL_ERROR_INVALID_PARAS,
*   FDL_ERROR_INIT
*   FDL_ERROR_FAILED
**/
FILEDOWNLOADER_API int
BeginDownload( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs, int *pnTaskId );

/**
* Check the download(lpszUrl,lpszSaveAs) was uncompleted or not.
* @Paras:
*  [IN] lpszUrl :   the url of a remote file
*  [IN] lpszSaveAs: local file name to be saved for url
*  [OUT] pResult:   the result of existed or not
* @Returns:
*   FDL_ERROR_SUCCESS
*   FDL_ERROR_INVALID_PARAS,
*   FDL_ERROR_FAILED
**/
FILEDOWNLOADER_API int
IsExistUnCompleteTask( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs, bool *pResult );

/**
* Delete the download task.
* @Paras:
*  [IN] lpszUrl :   the url of a remote file
*  [IN] lpszSaveAs: local file name to be saved for url
* @Returns:
*   FDL_ERROR_SUCCESS
*   FDL_ERROR_INVALID_PARAS,
*   FDL_ERROR_FAILED
**/
FILEDOWNLOADER_API int
DeleteUnCompleteTask( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs );

/**
* Continue the download(lpszUrl,lpszSaveAs) from the history ( differrent from BeginDownload)
* @Paras:
*  [IN] lpszUrl :   the url of a remote file
*  [IN] lpszSaveAs: local file name to be saved for url
*  [OUT] pnTaskId:  task id that related to this download
* @Returns:
*   FDL_ERROR_SUCCESS
*   FDL_ERROR_INVALID_PARAS,
*   FDL_ERROR_INIT
*   FDL_ERROR_FAILED
**/
FILEDOWNLOADER_API int
ContinueFromUnCompleteTask( const TCHAR * lpszUrl, const TCHAR *lpszSaveAs, int *nTaskId );

/**
* End(abort) a download
* @Paras:
*  [IN] nTaskId:   the task id of a download (maybe downloading or downloaded or invalid)
* @Returns:
*   FDL_ERROR_SUCCESS
*   FDL_ERROR_INVALID_PARAS,
*   FDL_ERROR_INIT
*   FDL_ERROR_FAILED
**/
FILEDOWNLOADER_API int
EndDownload( int nTaskId );

#define MIDLStatusPending       0
#define MIDLStatusDownloading   1
#define MIDLStatusFinished      2
/**
* Query the downloading task status[ final download result | error code(if failed) | progress (if downloading)]
* @Paras:
*  [IN] nTaskId:
*  [OUT] pStatus: final status 0 : pending, 1: downloading 2 : finished
*  [OUT] pErrorCode: filled with error code if *pStatus == 2 
*  [OUT] pTotalSize: the total file size in bytes
*  [OUT] pRecvSize: the toal downloaded size in bytes
* @Returns:
*   FDL_ERROR_SUCCESS
*   FDL_ERROR_INVALID_PARAS,
*   FDL_ERROR_FAILED
**/
FILEDOWNLOADER_API int
QueryTaskInfo( int nTaskId, int *pStatus, int *pErrorCode, unsigned long long *pTotalSize, unsigned long long *pRecvSize );

#ifdef __cplusplus
}
#endif

#endif //_FILE_DOWNLOADER_API_H