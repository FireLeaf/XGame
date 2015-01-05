/**************************************************************************
/*		FileName: XWinSys.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/14 20:30:22
/*		CopyRight: yc 
/*************************************************************************/

#include "XSys.h"
#include <windows.h>
#include <io.h>

class XWinMutex : public XMutex
{
public:
	XWinMutex(){InitializeCriticalSection(&cs);}
	virtual ~XWinMutex(){DeleteCriticalSection(&cs);}
	virtual void Lock(){EnterCriticalSection(&cs);}
	virtual void Unlock(){LeaveCriticalSection(&cs);}
protected:
	CRITICAL_SECTION cs;
};

namespace XSys
{
	XMutex* XCreateMutex()
	{
		return (new XWinMutex());
	}

	bool XCreateFile(const char* file_path)
	{
		HANDLE hFile = CreateFileA(file_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL);
		if ( INVALID_HANDLE_VALUE != hFile)
		{
			CloseHandle(hFile);
		}
		return true;
	}
	
	bool XCreateDirectory(const char* path)
	{
		return (TRUE == CreateDirectoryA(path, NULL));
	}

	bool XIsFileExist(const char* file_path)
	{
		return ( (_access( file_path , 0 )) != -1 );
	}

	bool XIsDirectory(const char* path)
	{
		return false;
	}
	bool XSetFileSize(const char* path, long size)
	{
		if (!XIsFileExist(path))
		{
			return false;
		}
// 		FILE* fp = fopen(path, "wb");
// 		if (!fp)
// 		{
// 			return false;
// 		}
// 		return (0 == _chsize(fp, size));
	}
}