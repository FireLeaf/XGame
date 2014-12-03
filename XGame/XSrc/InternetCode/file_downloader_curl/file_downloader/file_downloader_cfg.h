#pragma once
#include "stdafx.h"
#include "base/string_util.h"
#include <vector>

#define CURLPROC_TEMP_FILE_POSIX        _T(".dl")
#define CURLPROC_TASK_FILE_POSIX        _T(".dl.cfg")
#define CURLPROC_TEMP_FILE_RANGES_POSIX _T(".dl.ranges")

struct FileRange
{
    FileRange( const unsigned long long& from_, const unsigned long long& to_ )
    {
        this->from_ = from_;
        this->to_ = to_;
    }
    unsigned long long from_;
    unsigned long long to_;
};

class FileDownloadCfg
{
public:
    FileDownloadCfg( const tstring& filename );
    ~FileDownloadCfg();

    bool ReadFromFile( );
    bool WriteToFile( );

    void SetUrl( const tstring& url );
    tstring GetUrl() const;

    tstring GetCfgFileName() const;
    tstring GetFileName() const;

    void SetFileSize( const unsigned long long& fileSize );
    unsigned long long GetFileSize() const;

    unsigned long long GetRecvSize() const;

    void RemoveRange( const unsigned long long& from, const unsigned long long& to );
    tstring GetFileNameOfRange( const unsigned long long& from, const unsigned long long& to ) const;

    void GetAllRanges( std::vector< FileRange >& ranges );

    static tstring UInt64ToString( unsigned long long uill );
private:
    tstring url_;
    tstring file_name_;
    unsigned long long file_size_;
    //key is RangeFrom
    std::vector< FileRange > ranges_2_download_;
};