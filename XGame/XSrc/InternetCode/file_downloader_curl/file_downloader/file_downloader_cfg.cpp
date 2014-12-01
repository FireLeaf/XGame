#include "stdafx.h"
#include "file_downloader_cfg.h"
#include <algorithm>
#include "base/file_util.h"

using namespace std;
using namespace base;

bool CompareRange( const FileRange& first, const FileRange& second )
{
    return first.from_ < second.to_;
}

//FileDownloadCfg
FileDownloadCfg::FileDownloadCfg( const tstring& filename ):
file_size_( 0 ),
file_name_( filename )
{

}

FileDownloadCfg::~FileDownloadCfg()
{

}

tstring FileDownloadCfg::GetCfgFileName() const
{
    assert( !file_name_.empty() );
    return file_name_ + CURLPROC_TASK_FILE_POSIX;
}

bool FileDownloadCfg::ReadFromFile()
{
    tstring cfgFile = GetCfgFileName();
    tifiletream infile( cfgFile );
    if ( !infile.is_open() )
    {
        return false;
    }
    vector< tstring> lines;
    tstring line;
    while ( getline( infile, line ) )
    {
        lines.push_back( line );
    }

    if ( lines.size() < 4 ) return false;

    int ln = 0;
    url_ = lines.at( ln++ );
    StringUtil::Trim( url_ );
    if ( url_.empty() ) return false;

    file_name_ = lines.at( ln++ );
    StringUtil::Trim( file_name_ );
    if ( file_name_.empty() ) return false;

    file_size_ = _tcstoui64( lines.at( ln++ ).c_str(), NULL, 10 );

    int nRanges = _ttoi( lines.at( ln++ ).c_str() );
    if ( nRanges < 0 )  return false;

    if ( (int)lines.size() - 4 < nRanges )
        return false;

    for ( int i = 0; i < nRanges; ++i )
    {
        tstring strRanges = lines.at( ln++ );
        StringUtil::Trim( strRanges );
        vector<tstring> vecStrs;
        if ( !StringUtil::StringSplit( strRanges, _T("-"), vecStrs ) )
            continue;
        if ( vecStrs.size() != 2 )
            continue;
        FileRange range( _tcstoui64( vecStrs.at( 0 ).c_str(), NULL, 10 ),
                           _tcstoui64( vecStrs.at( 1 ).c_str(), NULL, 10 ) );
        if ( range.from_ >= range.to_ )
            continue;
        ranges_2_download_.push_back( range );
    }

    if ( ranges_2_download_.size() == 0 )
        return false;
    sort( ranges_2_download_.begin(), ranges_2_download_.end(), CompareRange );
    return true;
}

tstring FileDownloadCfg::UInt64ToString( unsigned long long uill )
{
    TCHAR uInt64StrBuf[65] = { 0 };
    _ui64tot_s( uill, uInt64StrBuf, 65, 10 );
    return uInt64StrBuf;
}

bool FileDownloadCfg::WriteToFile()
{
    tstring file = GetCfgFileName();
    vector< tstring> lines;
    lines.push_back( url_ );
    lines.push_back( file_name_ );
    lines.push_back( UInt64ToString( file_size_ ) );

    TCHAR intStr[ 10 ] = { 0 };
    _itot_s( ranges_2_download_.size() , intStr, 10, 10 );
    lines.push_back( intStr );

    for ( size_t i = 0;i < ranges_2_download_.size(); ++i )
    {
        tstring strRangeFrom = UInt64ToString( ranges_2_download_[i].from_ );
        tstring strRangeTo = UInt64ToString( ranges_2_download_[i].to_ );
        lines.push_back( strRangeFrom + _T("-") + strRangeTo );
    }

    tofiletream outfile;
    outfile.open( file.c_str(), ofstream::trunc );
    if ( !outfile.is_open() )
    {
        return false;
    }

    for ( std::vector<tstring>::const_iterator itLine = lines.begin(); itLine != lines.end(); ++itLine )
    {
        outfile << *itLine << std::endl;
    }
    outfile.close();
    return true;
}

unsigned long long FileDownloadCfg::GetRecvSize() const
{
    unsigned long long uRecvSize = 0;
    unsigned long long uPreVal = 0;
    for ( size_t i = 0; i < ranges_2_download_.size(); ++i )
    {
        const FileRange& range = ranges_2_download_[i];
        uRecvSize += ( range.from_ - uPreVal );
        uPreVal = range.to_;
    }

    uRecvSize += (file_size_ - uPreVal );
    return uRecvSize;
}

void FileDownloadCfg::SetUrl( const tstring& url )
{
    url_ = url;
}

tstring FileDownloadCfg::GetUrl() const
{
    return url_;
}

tstring FileDownloadCfg::GetFileName() const
{
    return file_name_;
}

void FileDownloadCfg::SetFileSize( const unsigned long long& fileSize )
{
    assert( ranges_2_download_.empty() );
    FileRange range( 0, fileSize );
    ranges_2_download_.push_back( range );
    file_size_ = fileSize;
}

unsigned long long FileDownloadCfg::GetFileSize() const
{
    return file_size_;
}

void FileDownloadCfg::RemoveRange( const unsigned long long& from, const unsigned long long& to )
{
    if ( from >= to ) return;
    int rangeIndex = -1;
    for ( size_t i = 0; i < ranges_2_download_.size(); ++i )
    {
        FileRange& range = ranges_2_download_[i];
        if ( range.from_ <= from )
            rangeIndex = i;
        else
            break;
    }
    if ( -1 == rangeIndex ) return;

    FileRange& range = ranges_2_download_[rangeIndex];
    if ( range.to_ < to ) return ;

    if ( range.from_ == from && range.to_ == to ) //the range removed
    {
        ranges_2_download_.erase( ranges_2_download_.begin() + rangeIndex );
    }
    else
    {
        if ( range.from_ == from ) //merge on left
            ranges_2_download_[rangeIndex].from_ = to;
        else if ( range.to_ == to ) //merge on right
            ranges_2_download_[rangeIndex].to_ = from;
        else //merge in mid
        {
            //1 divided to 2
            FileRange newLeftSubRange( ranges_2_download_[rangeIndex].from_, from );
            FileRange newRightSubRange( to, ranges_2_download_[rangeIndex].to_ );
            ranges_2_download_[rangeIndex] = newLeftSubRange;
            ranges_2_download_.insert( ranges_2_download_.begin() + rangeIndex + 1, newRightSubRange );
        }
    }
}

void FileDownloadCfg::GetAllRanges( vector< FileRange >& ranges )
{
    ranges = ranges_2_download_;
}

