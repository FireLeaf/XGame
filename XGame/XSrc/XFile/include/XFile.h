/**************************************************************************
/*  FileName 	: XFile.h								 
/*	FileDesc 	: 														 
/*	FileAuthor	: YangCao											 
/*	FileTime	: 2014/5/7 23:49	
/*  CopyRight	: 				 
**************************************************************************/

#ifndef __XFILE_H_
#define __XFILE_H_
#include <stdio.h>
#include "XType.h"
#include <algorithm>
class XFile
{
public:
	XFile(){m_fp = NULL;}
	virtual ~XFile()
	{
		CloseFile();
	}
	//
	FILE* GetFileHandle()
	{
		return m_fp;
	}
	//
	virtual bool OpenFile(const xchar* szFileName, const xchar* szOpenMode);
	virtual void CloseFile()
	{
		if (m_fp)
		{
			fclose(m_fp);
		}
	}
	xuint32 Read( void *buffer, size_t size, size_t count)
	{
		return fread(buffer, size, count, m_fp);
	}
	xuint32 Write( const void *buffer, size_t size, size_t count)
	{
		return fwrite(buffer, size, count, m_fp);
	}

	xchar *Gets( xchar *buffer, int n)
	{
		return fgets(buffer, n, m_fp);
	}

	xint32 Puts(xchar *buffer)
	{
		return fputs(buffer, m_fp);
	}

	xint32 Flush()
	{
		return fflush(m_fp);
	}

	xulong Tell()
	{
		return ftell(m_fp);
	}

	xint32 Seek( xlong offset, xint32 origin )
	{
		return fseek(m_fp, offset, origin);
	}

	void Rewind()
	{
		rewind(m_fp);
	}
	template<typename T>
	xint32 QuickReadValue(T& t)
	{
		return Read(&t, 1, sizeof(T));
	}
	template<typename T>
	xint32 QuickWriteValue(const T& t)
	{
		return Write(&t, 1, sizeof(T));
	}
protected:
	FILE* m_fp;
};

#endif