#pragma once
#include "base/string_util.h"
#include "libcurl/curl.h"
#include "file_downloader_error.h"

class FileDownloaderErrorUtil
{
public:
static bool IsNetworkError( int nDLErrorCode, int& nNetError )
{
    if ( nDLErrorCode >= FDL_ERROR_NETWORK_BASE && nDLErrorCode <= FDL_ERROR_NETWORK_MAX )
    {
        nNetError = nDLErrorCode - FDL_ERROR_NETWORK_BASE;
        return true;
    }

    return false;
}

static bool IsSystemError( int nDLErrorCode, int& nSysError )
{
    if ( IsNetworkError( nDLErrorCode, nSysError ) )
    {
        return false;
    }
    nSysError = nDLErrorCode - FDL_ERROR_SYSTEM_BASE;
    return true;
}

static tstring ConvertError2String( int nDLErrorCode )
{
    int nSysErr = 0;
    if ( IsSystemError( nDLErrorCode, nSysErr ) )
    {
        //return SystemUtility::WinSysInfo::FormatSystemErrorMessage( nSysErr );
    }

    int nNetErr = 0;
    if ( IsNetworkError( nDLErrorCode, nNetErr ) )
    {
        return base::StringUtil::ToString( curl_easy_strerror( (CURLcode)nDLErrorCode ) );
    }
    tostringstream oss;
    oss << _T("Unknow error:") << nDLErrorCode;
    return oss.str();
}


//As for there are some errors that curl exposed are converted from window standard error.
//here, pick up them and convert to window error code.
static int ConvertCURLcode2FDLError( int curlCode )
{
    if ( CURLE_WRITE_ERROR == (CURLcode) curlCode )
    {
        return MAKE_SYSTEM_ERROR( ERROR_DISK_FULL );
    }
    else if ( CURLE_OUT_OF_MEMORY == ( CURLcode)curlCode )
    {
        return MAKE_SYSTEM_ERROR( ERROR_OUTOFMEMORY );
    }
    else
    {
        return MAKE_NETWORK_ERROR( curlCode );
    }
}

};
