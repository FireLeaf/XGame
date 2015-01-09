/*******************************************************************************
	FILE:		XAndroidSys.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/08

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XSys.h"
#include <io.h>

class XAndroidMutex : public XMutex
{
public:
	XAndroidMutex(){pthread_mutex_init(&mutex, NULL);}
	virtual ~XAndroidMutex(){pthread_mutex_destroy(&mutex);}
	virtual void Lock(){pthread_mutex_lock(&mutex);}
	virtual void Unlock(){pthread_mutex_unlock(&mutex);}
protected:
	pthread_mutex_t  mutex;
};

namespace XSys
{
	XMutex* XCreateMutex()
	{
		return (new XAndroidMutex());
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

	bool XSetFileSize(FILE* fp, long size)
	{
		return (0 == _chsize(XFileNo(fp), size));
	}

	bool XSetFileSize(const char* path, long size)
	{
		if (!XIsFileExist(path))
		{
			return false;
		}
		FILE* fp = fopen(path, "wb");
		if (!fp)
		{
			return false;
		}
		bool bRet = (0 == _chsize(XFileNo(fp), size));
		fclose(fp);
		return bRet;
	}

	int XFileNo(FILE* fp)
	{
		return _fileno(fp);
	}
}