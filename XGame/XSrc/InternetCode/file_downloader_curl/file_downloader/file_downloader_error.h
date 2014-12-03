#ifndef _NETWORK_COMM_FILEDOWNLOAD_ERROR_H
#define _NETWORK_COMM_FILEDOWNLOAD_ERROR_H
//NOTICE
//We use int type to carry error code in the download moudle
//As for there are two kinds of error type
//  1. Windows standard error code machine ( refer to: GetLastError())
//  2. Network error, such as timeout, cannot connect host, internet etc.
//So we reserver [0-600] for networ error (including  HTTP response code[100-599] )
//and do offset for windows error

#define   FDL_ERROR_FAILED                         -1

#define   FDL_ERROR_INVALID_PARAS                   -2
#define   FDL_ERROR_INIT                            -3
#define   FDL_ERROR_SUCCESS                         0
//Network error
#define   FDL_ERROR_NETWORK_BASE                    FDL_ERROR_SUCCESS
#define   FDL_ERROR_NETWORK_CANNOT_RESOLVE_PROXY    ( FDL_ERROR_NETWORK_BASE + 5L )
#define   FDL_ERROR_NETWORK_CANNOT_RESOLVE_HOST     ( FDL_ERROR_NETWORK_BASE + 6L )
#define   FDL_ERROR_NETWORK_CANNOT_CONNECT_HOST     ( FDL_ERROR_NETWORK_BASE + 7L )
#define   FDL_ERROR_NETWORK_TIMEOUT                 ( FDL_ERROR_NETWORK_BASE + 28L )
//56: "Failure in receiving network data."
#define   FDL_ERROR_NETWORK_CANNOT_CONECT_INTERNET  ( FDL_ERROR_NETWORK_BASE + 56L )
#define   FDL_ERROR_NETWORK_MAX                     ( FDL_ERROR_NETWORK_BASE + 600L )

//System Error error code
#define   FDL_ERROR_SYSTEM_BASE                     ( FDL_ERROR_NETWORK_MAX + NO_ERROR )
//112L There is not enough space on the disk
#define   FDL_ERROR_DISK_FULL                       ( FDL_ERROR_SYSTEM_BASE + ERROR_DISK_FULL )
//14L  Not enough storage is available to complete this operation.
#define   FDL_ERROR_OUTOFMEMORY                     ( FDL_ERROR_SYSTEM_BASE + ERROR_OUTOFMEMORY )

#define   MAKE_NETWORK_ERROR( HttpCode )            ( FDL_ERROR_NETWORK_BASE + ( HttpCode ) )
#define   MAKE_SYSTEM_ERROR( LastErrorCode )        ( FDL_ERROR_SYSTEM_BASE + ( LastErrorCode ) )

#endif //!_NETWORK_COMM_FILEDOWNLOAD_ERROR_H