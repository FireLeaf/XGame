// MiFileDownloader_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../file_downloader/file_downloader_api.h"
#include "../file_downloader/file_downloader_error.h"
int _tmain(int argc, _TCHAR* argv[])
{
#define URL         _T("http://www.python.org/ftp/python/3.2.3/python-3.2.3.amd64.msi")
#define SAVE_AS     _T("python-3.2.3.amd64.msi")

    int miErrCode = 0;
    miErrCode = InitDownloadEngine(_T("curl.exe"));
    if ( miErrCode != FDL_ERROR_SUCCESS )
    {
        return 0;
    }

    int nTaskID = -1;
    miErrCode = BeginDownload(URL, SAVE_AS, &nTaskID);

    if ( miErrCode != FDL_ERROR_SUCCESS )
    {
        return 0;
    }
    printf("Begin download as task:%d\n", nTaskID);
    //query
    int nStatus;
    int nErrorCode;
    unsigned long long ullTotalSize;
    unsigned long long ullRecvSize;
    bool bFinished = false;
    while ( !bFinished )
    {
        Sleep(1000);
        nErrorCode = QueryTaskInfo(nTaskID, &nStatus, &nErrorCode, &ullTotalSize, &ullRecvSize);
        if (miErrCode != FDL_ERROR_SUCCESS )
            break;

        switch (nStatus)
        {
        case 2://finished
            if (nErrorCode == 0 )
            {
                //OK
                printf("Downloaded\n");
            }
            else
            {
                //Failed
                printf("Downloaded failed(%d)\n", nErrorCode );
            }
            bFinished = true;
            break;
        case 1:
            {
                //percent
                printf("%lld/%lld Bytes\n", ullRecvSize, ullTotalSize);
            }
            break;
        }
    }
    EndDownload(nTaskID);

    UninitDownloadEngine();
    return 0;
}

