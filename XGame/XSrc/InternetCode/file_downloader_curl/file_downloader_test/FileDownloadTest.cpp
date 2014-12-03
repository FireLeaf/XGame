// Logtest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "gtest/gtest.h"
#include "SystemUtility/SystemUtility.h"

#if TOTEST_HTTPDOWNLOAD

#include "FileTransfer/FileDownloader.h"
#include "FileTransfer/HttpWebPage.h"
#include <iostream>
using namespace SystemUtility;

class FileDownloadListener : public IFileDownloadListener
{
public:
    FileDownloadListener( const stdString& tag ):
    tag_( tag )
    {
        quitEvent_ = CreateEvent( NULL, FALSE, FALSE, NULL );
    }

    ~FileDownloadListener()
    {
        CloseHandle( quitEvent_ );
    }

    void Wait4Finish()
    {
        WaitForSingleObject( quitEvent_, INFINITE );
    }
protected:
    virtual void OnDownloadProgressChanged( long taskId, ULONGLONG recvSize, ULONGLONG totalFileSize )
    {
        LOG_TRACE( "OnDownloadProgressChanged(task:%d, recvSize:%d, totalFileSize:%d)", taskId, (long)recvSize, (long)totalFileSize );
        if (totalFileSize > 0)
        {
            double  uProgress = (double)(100.0*double(recvSize)/double(totalFileSize));
            TCHAR  szCurProgMsg[MAX_PATH] = {'\0'};
            _stprintf_s(szCurProgMsg, MAX_PATH, _T("%s : %0.2fMB Recv %.2fMB(%.2f%%)"), tag_.c_str(),
                totalFileSize / (double)0x100000, recvSize / (double)0x100000,
                uProgress);
            SetConsoleTitle( szCurProgMsg );
        }
    }

    virtual void OnDownloadFinished( long taskId , bool result,int errorCode )
    {
        LOG_TRACE("OnDownloadFinished(task:%d) %s", taskId , result ? "OK" : "Failed");
        SetEvent( quitEvent_ );
    }
private:
    HANDLE quitEvent_;
    stdString tag_;
};

TEST( HttpDownloadTEST, DownloadNewFile )
{
    FileDownloader& downloader = FileDownloader::GetInstance();
    downloader.Init();

    //HttpWebPage webPage(  _T("http://localhost:8080/test.txt") );
    //stdString content;
    //webPage.DownloadString( content );
    //stdString content2;
    //webPage.DownloadString( content2, 1, 10 );

    FileDownloadRequestInfo fdl_req_info;
#define URL _T("http://www.python.org/ftp/python/3.2.3/python-3.2.3.amd64.msi")
    fdl_req_info.Url =  URL;//_T("http://p.www.xiaomi.com/User_manual.pdf");
   // fdl_req_info.Url =  _T("http://localhost:8080/test.txt");
    fdl_req_info.SaveAs = _T("test.data");
    //FileUti::DeleteFileIfExist( fdl_req_info.SaveAs );

    FileDownloadListener fdl_listener(fdl_req_info.Url);
    long fdl_task_info;
    long taskId = 0;
    if ( downloader.IsExistUnCompleteTask( fdl_req_info.SaveAs ) )
    {
        EXPECT_TRUE(downloader.ContinueFromUnCompleteTask( fdl_req_info.SaveAs, fdl_task_info, &fdl_listener ));
    }
    else
    {
        EXPECT_TRUE ( downloader.BeginDownload( fdl_req_info, fdl_task_info, &fdl_listener ) );
    }

    fdl_listener.Wait4Finish();
    downloader.Release();
}

#endif //TOTEST_HTTPDOWNLOAD