#ifndef _NETWORK_COMM_FILEDOWNLOAD_ENGINE_H
#define _NETWORK_COMM_FILEDOWNLOAD_ENGINE_H

#include "stdafx.h"
#include "base/string_util.h"


class IFileDownloadListener;
struct FileDownloadRequestInfo
{
    tstring Url;
    tstring SaveAs;
};

class FileDownloadEngine
{
public:
    virtual ~ FileDownloadEngine(){}
    virtual bool InitDownloadEngine() = 0;

    virtual bool UninitDownloadEngine() = 0;

    virtual bool BeginDownload( const FileDownloadRequestInfo& requsetInfo, long& taskId,  IFileDownloadListener* listener ) = 0;

    virtual bool EndDownload( long taskId ) = 0;

    virtual bool IsExistUnCompleteTask( const tstring& expectedFile ) = 0;

    virtual bool DeleteUnCompleteTask( const tstring& expectedFile ) = 0;

    virtual bool ContinueFromUnCompleteTask( const tstring& expectedFile, long& taskId ,IFileDownloadListener* listener ) = 0;
};

#endif //!_NETWORK_COMM_FILEDOWNLOAD_ENGINE_H