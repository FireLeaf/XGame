#include "stdafx.h"
#include "FileDownloaderThread.h"
#include "http_webclient.h"
#include "libcurl/curl.h"
#include <shlwapi.h>
#include <Tlhelp32.h>
#include "base/file_util.h"
#include "file_downloader_error_util.h"
#include "file_downloader_api.h"

using namespace base;

tstring FileDownloaderThread::s_strExePath;


class HandleAutoCloser
{
public:
    HandleAutoCloser()
    {

    }
    ~HandleAutoCloser()
    {
        CloseAll();
    }
    void Add( HANDLE handle )
    {
        handles_.push_back( handle );
    }

    void CloseAll()
    {
        for ( std::vector< HANDLE >::const_iterator itHandle = handles_.begin();
            itHandle != handles_.end(); ++itHandle )
        {
            ::CloseHandle( *itHandle );
        }
        handles_.clear();
    }

private:
    std::vector< HANDLE > handles_;
};

bool FileDownloaderThread::InitCurlExePath( const tstring& exePath )
{
    s_strExePath = exePath;
    return true;
}

void FileDownloaderThread::KillCurlProcessesIfRunning()
{
    TCHAR *szCurlName = PathFindFileName( s_strExePath.c_str() );
    //Kill AllProcess
    int nKilledCount = 0;
    PROCESSENTRY32* pInfo = new PROCESSENTRY32;
    pInfo->dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    ::Process32First( hSnapshot, pInfo );
    while ( ::Process32Next( hSnapshot, pInfo) )
    {
        if ( 0 == _tcsicmp( szCurlName, pInfo->szExeFile) )
        {
            HANDLE handleTerminate = ::OpenProcess( PROCESS_TERMINATE, false, pInfo->th32ProcessID);
            if ( NULL == handleTerminate )
            {
                LOG_V_E( _T("fail to OpenProcess( name:%s: id:%d) with error:%d"), szCurlName, pInfo->th32ProcessID, GetLastError() );
                continue;
            }
            if ( !::TerminateProcess( handleTerminate, 0 ) )
            {
                LOG_V_E( _T("fail to TerminateProcess( name:%s: id:%d) with error:%d"),
                    szCurlName, pInfo->th32ProcessID, GetLastError() );
            }
            else
            {
                ++nKilledCount;
            }
            ::CloseHandle( handleTerminate );
        }
    }
    ::CloseHandle( hSnapshot);
    delete pInfo;
}

FileDownloaderThread::FileDownloaderThread():
download_cfg_( _T("") )
{
    exit_flag_is_set_ = false;
    last_recv_data_size_ = 0;
    listener_ptr_ = NULL;
    client_data_ptr_ = NULL;
    error_code_ = 0;
}

FileDownloaderThread::~FileDownloaderThread()
{
    StopIfRunning();
}

void FileDownloaderThread::StopIfRunning()
{
    if ( IsRuning() )
    {
        LOG_V_D( _T("[FileDownloaderThread] Force stop downloading ") );
        exit_flag_is_set_ = true;
        {
            SmartLockT<CriticalSection> lockListener( cs_4_listener_ );
            listener_ptr_ = NULL;
        }
        Join();
        exit_flag_is_set_ = false;
    }
}

bool FileDownloaderThread::IsExistUnCompleteTask( const tstring& targetFile )
{
    FileDownloadCfg taskCfg( targetFile );
    if ( !taskCfg.ReadFromFile() )
    {
        return false;
    }
    tstring strCfgFile = taskCfg.GetCfgFileName();
    if ( !FileUtil::IsFileExists( targetFile.c_str() ) )
    {
        if ( FileUtil::IsFileExists( strCfgFile.c_str() ) )
            FileUtil::DeleteFile( strCfgFile.c_str() );
        return false;
    }

    LONG dwFileSize = FileUtil::GetFileSize( targetFile.c_str() );
    if ( dwFileSize != taskCfg.GetFileSize() )
    {
        FileUtil::DeleteFile( targetFile.c_str() );
        FileUtil::DeleteFile( strCfgFile.c_str() );
        return false;
    }

    return true;
}

bool FileDownloaderThread::DeleteUnCompleteTask( const tstring& targetFile )
{
    const TCHAR* szFile = targetFile.c_str();
    if ( FileUtil::IsFileExists( szFile ) )
    {
        if ( !FileUtil::DeleteFile( szFile ) )
            return false;
    }

    tstring cfgFile = targetFile + CURLPROC_TASK_FILE_POSIX;
    const TCHAR* szCfgFile = cfgFile.c_str();
    if ( FileUtil::IsFileExists( szCfgFile ) )
    {
        if ( !FileUtil::DeleteFile( szCfgFile ) )
            return false;
    }

     tstring subRangesDir = targetFile + CURLPROC_TEMP_FILE_RANGES_POSIX;
     const TCHAR* szRangeDir = subRangesDir.c_str();
    if ( FileUtil::IsDirectoryExists( szRangeDir ) )
    {
        if ( !FileUtil::DeleteFile( szRangeDir ) )
            return false;
    }
     return true;
}

bool CreateFixedSizeFile( LPCTSTR lpFile, ULONG lowSize, ULONG highSize )
{
    HANDLE hFile = ::CreateFile( lpFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if ( INVALID_HANDLE_VALUE == hFile )
        return false;
    HANDLE hMapFile = CreateFileMapping( hFile, NULL, PAGE_READWRITE, highSize, lowSize, NULL);
    CloseHandle( hFile );
    if ( NULL == hMapFile )
    {
        return false;
    }
    CloseHandle(hMapFile);
    return TRUE;
}

bool FileDownloaderThread::BeginDownloadAsync( const tstring& url,
                                           const tstring& strSaveAs,
                                           ICPFileDownloaderListener* pListener,
                                           void* pClientData )
{
    unsigned long long totalFileSize = 0;
    int nResponseCode = 0;
    if (!HttpWebFileClient::QueryFileSize( url, totalFileSize, nResponseCode ) )
    {
        LOG_V_E( _T("Failed to query file size of %s [error-code:%d]"), url.c_str(), nResponseCode );
        error_code_ = MAKE_NETWORK_ERROR( nResponseCode );
        return false;
    }

    ULARGE_INTEGER liTotalSize;
    liTotalSize.QuadPart = totalFileSize;
    if ( !CreateFixedSizeFile( strSaveAs.c_str(), liTotalSize.LowPart, liTotalSize.HighPart ) )
    {
        DWORD dwErrCode = GetLastError();
        LOG_V_E( _T("Failed to create file:%s with size(L:%ul H:%ul)[Error:%d]"), strSaveAs.c_str(),
                liTotalSize.LowPart, liTotalSize.HighPart, dwErrCode );
        error_code_ = MAKE_SYSTEM_ERROR( dwErrCode );
        return false;
    }

    FileDownloadCfg dlcfg( strSaveAs ) ;
    dlcfg.SetUrl( url );
    dlcfg.SetFileSize( totalFileSize );
    if ( !dlcfg.WriteToFile() )
    {
        DWORD dwErrCode = GetLastError();
        LOG_V_E( _T("Failed to write cfg of %s [Error:%d]"), strSaveAs.c_str(), dwErrCode );
        error_code_ = MAKE_SYSTEM_ERROR( dwErrCode );
        return false;
    }

    return BeginDownloadAsync( dlcfg, pListener, pClientData );
}

bool FileDownloaderThread::BeginDownloadAsync( const FileDownloadCfg& dlcfg,
                                                ICPFileDownloaderListener* pListener,
                                                void* pClientData )
{
    download_cfg_ = dlcfg;
    client_data_ptr_ = pClientData;
    listener_ptr_ = pListener;
    Start();
    return true;
}

bool FileDownloaderThread::GetRangesQueue2Download( std::queue< FileRange >& rangeQueue )
{
    std::vector< FileRange > ranges;
    download_cfg_.GetAllRanges( ranges );
    for ( std::vector< FileRange >::const_iterator itBigRange = ranges.begin();
        itBigRange != ranges.end(); ++itBigRange )
    {
        const FileRange& bigRange = *itBigRange;
        if ( bigRange.to_ <= bigRange.from_ )
        {
            LOG_V_W( _T("Invalid range (%ull-%ull), ignore it!"),bigRange.from_, bigRange.to_ );
            continue;
        }
        int nSubCount = (int) ( ( bigRange.to_ - bigRange.from_ ) / nMaxBytesPerRange );
        unsigned long long nLeftBytes = ( bigRange.to_ - bigRange.from_ ) - nSubCount* nMaxBytesPerRange;
        if ( nLeftBytes > 1.0f* nMaxBytesPerRange / 3.0f || 0 == nSubCount )
        {
            ++nSubCount ;
        }

        for ( int i = 0; i < nSubCount; ++i )
        {
            FileRange smallRange( bigRange.from_ + i * nMaxBytesPerRange,
                bigRange.from_ + (i + 1 ) * nMaxBytesPerRange );
            if ( i == nSubCount- 1 )
                smallRange.to_ = bigRange.to_;
            rangeQueue.push( smallRange );
        }
    }
    LOG_V_D( _T("Ranges :%d X %dBytes"),(int) rangeQueue.size(), nMaxBytesPerRange );
    return rangeQueue.size() > 0;
}

tstring FileDownloaderThread::GetDirectoryOfRanges2Download() const
{
    tstring strSubRangeDir = download_cfg_.GetFileName() + CURLPROC_TEMP_FILE_RANGES_POSIX;
    return strSubRangeDir;
}

bool FileDownloaderThread::LaunchCurl2DownloadRange( const FileRange& range2Download,
                                                 CPDownloadProcContext& runningDownloadInst )
{
    tstring strRangeFileName = FileDownloadCfg::UInt64ToString( range2Download.from_ ) + _T("-")
        + FileDownloadCfg::UInt64ToString( range2Download.to_ );
    tstring strDirOfDownloads = GetDirectoryOfRanges2Download();
    TCHAR szSaveFileAs[ MAX_PATH ] = { 0 };
    _tcscpy_s( szSaveFileAs, MAX_PATH, strDirOfDownloads.c_str() );
    PathAppend( szSaveFileAs, strRangeFileName.c_str() );
    tstring strSaveFileAs = szSaveFileAs;

    tostringstream cmdPara;
    cmdPara << s_strExePath.c_str();
    cmdPara << _T(" --url ") << download_cfg_.GetUrl();
    cmdPara << _T(" --range ") << range2Download.from_ << _T("-") << range2Download.to_ - 1;
    cmdPara << _T(" --speed-time ") << 120;
    cmdPara << _T(" --speed-limit ") << 1;
    cmdPara << _T(" --location ");
    cmdPara << _T(" --max-redirs ") << 50;
    cmdPara << _T(" -#");//just show total percent
    cmdPara << _T(" --output ") << _T("\"") << strSaveFileAs << _T("\"");
    tstring strCmd = cmdPara.str();

    Process* pProcess = new Process();
    if ( !pProcess->Create( strCmd.c_str(), 0, NULL, TRUE ) )
    {
        LOG_V_E( _T("failed to create process:%s"), s_strExePath.c_str() );
        delete pProcess;
        return false;
    }

    LOG_V_D( _T("%s"), strCmd.c_str() );
    runningDownloadInst.pProcess = pProcess;
    runningDownloadInst.fPercent = 0.0f;
    runningDownloadInst.nRangeFrom = range2Download.from_;
    runningDownloadInst.nRangeCur = runningDownloadInst.nRangeFrom;
    runningDownloadInst.nRangeTo = range2Download.to_;
    runningDownloadInst.strFile = strSaveFileAs;
    return true;
}

bool FileDownloaderThread::Wait4CurlExit( const std::vector< CPDownloadProcContext >& runningDownloadInsts,
                                           DWORD dwTimeout, size_t& index )
{
    if ( runningDownloadInsts.size() == 0 )
        return false;

    std::vector<HANDLE> procHandles;
    for ( std::vector< CPDownloadProcContext >::const_iterator itDlInstCxt = runningDownloadInsts.begin();
        itDlInstCxt != runningDownloadInsts.end(); ++itDlInstCxt )
    {
        procHandles.push_back( itDlInstCxt->pProcess->GetProcessHandle() );
    }
    HANDLE* pProcessHandles = &(procHandles.at(0));
    //wait curl exit
    DWORD dwWaitObjIndx = ::WaitForMultipleObjects( runningDownloadInsts.size(), pProcessHandles, false, dwTimeout );
    if ( !(dwWaitObjIndx >= WAIT_OBJECT_0 && dwWaitObjIndx <= WAIT_OBJECT_0 + runningDownloadInsts.size() - 1) )
        return false;
    index = dwWaitObjIndx - WAIT_OBJECT_0;
    return true;
}

unsigned long long FileDownloaderThread::CommitRangeFile( const tstring& strRangeFile,
                                            const unsigned long long& rangeFrom )
{
    if ( !FileUtil::IsFileExists( strRangeFile.c_str() ) )
    {
        LOG_V_W( _T("cannot commit range file: the file %s not exist"), strRangeFile.c_str() );
        return 0;
    }

    tstring strFinalFile = download_cfg_.GetFileName();

    //Save file
    HANDLE hFinalFile =  CreateFile( strFinalFile.c_str(),
                                     GENERIC_WRITE,
                                     FILE_SHARE_WRITE,
                                     NULL,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     NULL );
    if ( INVALID_HANDLE_VALUE == hFinalFile )
    {
        LOG_V_E( _T("FileDownloaderThread CreateFile(%s) with error:%d"), strFinalFile.c_str(), GetLastError() );
        return 0;
    }

    HandleAutoCloser handleAutoCloser;
    handleAutoCloser.Add( hFinalFile );

    LARGE_INTEGER li;
    li.QuadPart = rangeFrom;
    if ( !SetFilePointerEx( hFinalFile, li, NULL, FILE_BEGIN ) )
    {
        LOG_V_E( _T("FileDownloaderThread failed when SetFilePointerEx with error:%d"), GetLastError() );
        return 0;
    }

    HANDLE hRangeFile = CreateFile( strRangeFile.c_str(),
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL );
    if ( INVALID_HANDLE_VALUE == hRangeFile )
    {
        LOG_V_E( _T("FileDownloaderThread CreateFile(%s) with error:%d"), strRangeFile.c_str(), GetLastError() );
        return 0;
    }
    handleAutoCloser.Add( hRangeFile );

    //Merg file
    const int nReadWriteBufLen = 1024;
    BYTE  rwBuf[ nReadWriteBufLen ] = { 0 };
    bool bIOSuccess = true;
    unsigned long long uTotalWrittenSize = 0;
    DWORD dwReaded = 0;
    while ( ReadFile( hRangeFile, rwBuf, nReadWriteBufLen, &dwReaded, NULL ) && dwReaded > 0 )
    {
        DWORD written = 0;
        if ( !WriteFile( hFinalFile, rwBuf, dwReaded, &written, NULL ) || written != dwReaded )
        {
            LOG_V_E( _T("[FileDownloaderThread::CommitRangeFile] WriteFile failed:%d"), GetLastError() );
            bIOSuccess = false;
            break;
        }
        uTotalWrittenSize += written;
    }
    return uTotalWrittenSize;
}

int EnumFiles( const tstring& dir, std::vector<tstring> &files )
{
    tstring dirEndWithSlash = dir;

    if (*dirEndWithSlash.rbegin() != '/' && *dirEndWithSlash.rbegin() != '\\')
    {
        dirEndWithSlash += _T("\\");
    }

    tstring filter = dirEndWithSlash + _T("*.*");

     WIN32_FIND_DATA fd;
     HANDLE  hFind = FindFirstFile( filter.c_str(), &fd );
     if ( hFind == INVALID_HANDLE_VALUE )
     {
         return 0;
     }

     tstring name;
     do 
     {
         if ( !(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY ))
         {
             name = dirEndWithSlash + fd.cFileName;
             files.push_back( name );
         }

     } while ( FindNextFile( hFind, &fd ) != 0 );

     FindClose( hFind );

     return ( int )( files.size() );
}

#define SYSTEM_PATH_SEP _T( "/\\" )
tstring GetShortFileName( const tstring& fullFileName )
{
    size_t len = fullFileName.length();
    if ( 0 == len ) return tstring();
    size_t beginPos = fullFileName.find_last_of( SYSTEM_PATH_SEP );
    size_t endPos = tstring::npos;
    if ( beginPos == len - 1 ){
        //re find
        endPos = beginPos;
        beginPos = fullFileName.find_last_of( SYSTEM_PATH_SEP, endPos - 1 );
    }
    if ( beginPos == tstring::npos ){
        beginPos= 0;
    }
    else
    {
        ++beginPos;
    }

    tstring shortName = fullFileName.substr( beginPos, endPos - beginPos );
    return shortName;
}

void FileDownloaderThread::CommitLastRangeFilesIfExist()
{
    tstring strSubRangeDir = GetDirectoryOfRanges2Download();

    //enum all files under foder of {download file}.dl.ranges/
    std::vector< tstring > unMergedFiles;
    EnumFiles( strSubRangeDir, unMergedFiles );
    if ( unMergedFiles.size() == 0 ) return;

    for ( std::vector< tstring >::const_iterator itFile = unMergedFiles.begin();
        itFile != unMergedFiles.end(); ++itFile )
    {
        unsigned long long dwFileSize = FileUtil::GetFileSize( itFile->c_str() );
        if ( dwFileSize == 0 )
        {
            FileUtil::DeleteFile( itFile->c_str() );
            LOG_V_W(_T("range file %s size with ZERO"), itFile->c_str() );
            continue;
        }
        //extract range[from, to ] from file name such as 0-1024
        tstring strShortFileName = GetShortFileName( *itFile );
        TCHAR rangeFromBufStr[MAX_PATH] = { 0 };
        TCHAR rangeToBufStr[MAX_PATH] = { 0 };
        const TCHAR* tokenString = strShortFileName.c_str();
        if ( 2 != _stscanf( tokenString, _T("%[0-9]-%[0-9]"), rangeFromBufStr, rangeToBufStr ) )
        {
            FileUtil::DeleteFile( itFile->c_str() );
            LOG_V_W( _T("invalid range file %s was deleted"), tokenString );
            continue;
        }

        unsigned long long uRangeFrom = _tcstoui64( rangeFromBufStr, NULL, 10 );
        unsigned long long uRangeTo = _tcstoui64( rangeToBufStr, NULL, 10 );

        if ( uRangeFrom >= uRangeTo || uRangeTo > download_cfg_.GetFileSize() )
        {
            LOG_V_W(_T("%s uRangeFrom >= uRangeTo || uRangeTo > download_cfg_.GetFileSize()"), tokenString );
            FileUtil::DeleteFile( itFile->c_str() );
            continue;
        }
        //commit the range file
        unsigned long long uWrittenSize = CommitRangeFile( *itFile, uRangeFrom );
        if ( uWrittenSize > 0  )
        {
            LOG_V_D( _T("%s was merged"), tokenString );
        }
        FileUtil::DeleteFile( itFile->c_str() );
        //Now, we dont keep this range to download again
        if ( uWrittenSize > 0 )
        {
            download_cfg_.RemoveRange( uRangeFrom, uRangeFrom + uWrittenSize );
            download_cfg_.WriteToFile();
        }
    }
}

//extract the percent value from the output
//The output of curl.exe:
/*
                                                                           0.0%
                                                                           0.7%
#                                                                          1.4%
#####                                                                      7.7%
#######                                                                   10.0%
##################################################################        92.8%
######################################################################### 100.0%
*/
void FileDownloaderThread::PeekDownloadPercent( std::vector< CPDownloadProcContext >& dlInstCxts )
{
    const int nBufLength = 1024;
    char szBuff[nBufLength] = { 0 };
    for ( std::vector< CPDownloadProcContext >::iterator itDlInstCxt = dlInstCxts.begin();
        itDlInstCxt != dlInstCxts.end(); ++itDlInstCxt )
    {
        CPDownloadProcContext& dlInstCxt = *itDlInstCxt;
        HANDLE hOut = dlInstCxt.pProcess->GetStdErrorHandle();//GetStdErrorHandle
        HANDLE hProcess = dlInstCxt.pProcess->GetProcessHandle();
        bool bPercentReaded = false;
        do
        {
            DWORD dwTotalBytesAvail = 0;
            if ( !PeekNamedPipe( hOut, NULL, 0, NULL, &dwTotalBytesAvail, NULL) )
            {
                break;
            }
            if ( dwTotalBytesAvail <=0 )
                break;
            tstring strPercentBars;
            DWORD dwReaded = 0;
            //Here we just read <-1024 bytes to get approximate percent
            if( ReadFile(hOut, szBuff, nBufLength, &dwReaded, NULL) )
            {
                strPercentBars = StringUtil::ToString( szBuff );
            }
            if ( strPercentBars.empty() )
                break;

            //LOG_V_D(_T("Raw output:\n%s"), strPercentBars.c_str() );
            size_t nPercentPos = strPercentBars.find_last_of( _T("%") );
            if ( tstring::npos == nPercentPos )
                break;
            size_t nLastSpacePos = strPercentBars.find_last_of(_T(" "), nPercentPos );
            if ( tstring::npos == nLastSpacePos )
                break;
            if ( nPercentPos - nLastSpacePos - 1 <3 )
                break;
            tstring strPercent = strPercentBars.substr( nLastSpacePos + 1, nPercentPos - nLastSpacePos - 1 );
            float fPercent = static_cast<float>( _ttof( strPercent.c_str() ) );
            if (  fabs(100.0f- fPercent) <= 1e-6 )
            {
                break;
            }
            //LOG_V_D(_T("Fetch percent: process-%p- percent %s%%"), dlInstCxt.pProcess->getProcessHanlde(), strPercent.c_str() );
            dlInstCxt.fPercent = max( fPercent, dlInstCxt.fPercent );
            bPercentReaded = true;
        }
        while (false);

        //Issue the time between process exited and not monitored
        if ( !bPercentReaded )
        {
            if ( !dlInstCxt.pProcess->IsRuning() )
            {
                dlInstCxt.fPercent = FileUtil::GetFileSize( dlInstCxt.strFile.c_str() ) *1.0f/(dlInstCxt.nRangeTo - dlInstCxt.nRangeFrom) *100.0f;
            }
        }
    }
}

void FileDownloaderThread::NotifyDownloadProgress( std::vector< CPDownloadProcContext >& dlInstCxts )
{
    SmartLockT<CriticalSection> lockListener( cs_4_listener_ );
    if ( NULL == listener_ptr_ )
        return;

    unsigned long long bookKeepedDownloadedSize = download_cfg_.GetRecvSize();
    unsigned long long unBookKeepedDownloadedSize = 0;
    for ( std::vector< CPDownloadProcContext >::iterator itDlInstCxt = dlInstCxts.begin();
        itDlInstCxt != dlInstCxts.end(); ++itDlInstCxt )
    {
        CPDownloadProcContext& dlInstCxt = *itDlInstCxt;
        unBookKeepedDownloadedSize += (unsigned long long)(( dlInstCxt.nRangeTo - dlInstCxt.nRangeFrom ) * dlInstCxt.fPercent / 100.0f );
    }

    unsigned long long recvSize = bookKeepedDownloadedSize + unBookKeepedDownloadedSize;
    unsigned long long totalFileSize = download_cfg_.GetFileSize();
    if ( recvSize > last_recv_data_size_ )//avoid from big to small
    {
        last_recv_data_size_ = recvSize;
    }
    listener_ptr_->OnRecvData( client_data_ptr_, last_recv_data_size_, totalFileSize );
}

UINT FileDownloaderThread::Run()
{
    last_recv_data_size_ = 0;
    LOG_V_D( _T("begin download %s"),  download_cfg_.GetUrl().c_str() );
    tstring strSubRangeDir = GetDirectoryOfRanges2Download();
    if ( FileUtil::IsDirectoryExists( strSubRangeDir.c_str() ) )
    {
        CommitLastRangeFilesIfExist();
    }
    else
    {
        if ( !FileUtil::CreateDirectory( strSubRangeDir.c_str() ) )
        {
            DWORD dwErrCode = GetLastError();
            LOG_V_E( _T("fail to create dir: %s [error code:%d]"), strSubRangeDir.c_str(), dwErrCode );
             error_code_ = MAKE_SYSTEM_ERROR( dwErrCode );
            return 0;
        }
    }
    std::queue< FileRange > rangeQueue;
    GetRangesQueue2Download( rangeQueue );
    //There are at most nMaxInstanceOfCurl curl.exe running at the same time.
    std::vector< CPDownloadProcContext > runningDownloadInsts;
    while( !rangeQueue.empty() && !exit_flag_is_set_ ) //dequeue
    {
        for ( size_t i = runningDownloadInsts.size(); i < nMaxInstanceOfCurl && !rangeQueue.empty(); ++i )
        {
            FileRange range2download = rangeQueue.front();
            rangeQueue.pop();

            CPDownloadProcContext dlInstCxt;
            if ( !LaunchCurl2DownloadRange( range2download, dlInstCxt ) )
            {
                DWORD dwErrCode = GetLastError();
                LOG_V_E( _T("Failed to start curl.exe [error:%d]"), dwErrCode );
                error_code_ = MAKE_SYSTEM_ERROR( dwErrCode );
                return 0;
            }
            runningDownloadInsts.push_back( dlInstCxt );
        }

        if ( runningDownloadInsts.size() == 0 ) break; //finish

        //loop to monitor the state (download percent & exit ) of curl.exe processes
        //IF one downloaded sucessfully then launch a new process to download
        //ELSE IF downloaded failed
        //        IF   it is the last one that running then quit with error
        //        ELSE continue monitor current running queue
        const bool bContinueMonitorThisDownloadGroup = true;
        while ( bContinueMonitorThisDownloadGroup && !exit_flag_is_set_ )
        {
            if ( runningDownloadInsts.size() == 0 )
                break;
            PeekDownloadPercent( runningDownloadInsts );
            NotifyDownloadProgress( runningDownloadInsts );
            size_t exitProcIndex = 0;
            if ( !Wait4CurlExit( runningDownloadInsts, nReportIntervalsInSeconds * 1000, exitProcIndex ) )
                continue;//no process exited

            //dequeue process from running list
            CPDownloadProcContext dlInst = runningDownloadInsts.at( exitProcIndex );
            runningDownloadInsts.erase( runningDownloadInsts.begin() + exitProcIndex );
            DWORD dwExitCode = -1;//normally the exit code >=0
            if ( !GetExitCodeProcess( dlInst.pProcess->GetProcessHandle(), &dwExitCode ) )
            {
                LOG_V_E( _T("failed to get exit code [Error:%d]"), GetLastError() );
            }
            delete dlInst.pProcess;
            dlInst.pProcess = NULL;

            //Try to commit data if any bytes were downloaded 
            unsigned long long uActualDownloadedBytes = CommitRangeFile( dlInst.strFile, dlInst.nRangeFrom );
            if ( !FileUtil::DeleteFile( dlInst.strFile.c_str() ) )
            {
                LOG_V_W( _T("DeleteFile(%s) failed [%d]"), dlInst.strFile.c_str(), GetLastError() );
            }

            if ( uActualDownloadedBytes > 0 )
            {
                dlInst.nRangeCur = dlInst.nRangeFrom + uActualDownloadedBytes;
                download_cfg_.RemoveRange( dlInst.nRangeFrom, dlInst.nRangeCur );
                if (!download_cfg_.WriteToFile() )
                {
                    DWORD dwErrCode = GetLastError();
                    LOG_V_E( _T("Failed to update cfg file[error:%d]"), dwErrCode );
                    error_code_ = MAKE_SYSTEM_ERROR( dwErrCode );
                    exit_flag_is_set_ = true;
                    break;
                }
            }
            //Enqueue undownload range
            if ( dlInst.nRangeCur != dlInst.nRangeTo )
            {
                FileRange rangeRetry( dlInst.nRangeCur, dlInst.nRangeTo );
                rangeQueue.push( rangeRetry );
            }

            bool isDLInstSuccessful = ( 0 == dwExitCode );
            if ( !isDLInstSuccessful )// some error occurs with this process
            {
                //LOG_V_E( "%s", curl_easy_strerror( (CURLcode)dwExitCode ) );
                //:( As for the last one is failed, may be the following ones that newly launched will be failed again.
                //So EXIT
                if ( runningDownloadInsts.size() == 0 )
                {
                    LOG_V_E( _T("cancel download for error occurs") );
                    error_code_ = FileDownloaderErrorUtil::ConvertCURLcode2FDLError( dwExitCode );
                    exit_flag_is_set_ = true;
                }
                else
                {
                    //continue monitor current download queue
                }
            }
            else //downloaded ok
            {
                if ( !rangeQueue.empty() )
                {
                    //we can launch a new curl process
                    break;
                }
            }
        }
    }

    //stop curl process if any active
    for ( std::vector< CPDownloadProcContext >::iterator itInst = runningDownloadInsts.begin();
        itInst != runningDownloadInsts.end(); ++itInst )
    {
        itInst->pProcess->Destroy();
        itInst->pProcess->Join();
        delete itInst->pProcess;
    }

    bool bAllDownloaded = download_cfg_.GetRecvSize() == download_cfg_.GetFileSize();
    if ( bAllDownloaded )
    {
        FileUtil::DeletePath( strSubRangeDir.c_str() );
        //delete cfg
        FileUtil::DeleteFile( download_cfg_.GetCfgFileName().c_str());
    }
    {
        SmartLockT<CriticalSection> lockListener( cs_4_listener_ );
        if ( NULL != listener_ptr_ )
        {
            listener_ptr_->OnRecvData( client_data_ptr_, download_cfg_.GetRecvSize(), download_cfg_.GetFileSize() );

            listener_ptr_->OnDownloadFinished( client_data_ptr_, bAllDownloaded, error_code_ );
        }
    }
    LOG_V_D( _T("end download") );
    return 0;
}

