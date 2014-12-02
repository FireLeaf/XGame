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
	size_t Read( void *buffer, size_t size, size_t count)
	{
		return fread(buffer, size, count, m_fp);
	}
	size_t Write( const void *buffer, size_t size, size_t count)
	{
		return fwrite(buffer, size, count, m_fp);
	}

	size_t SafeWrite( const void *buffer, size_t size, size_t count, int safe_size)
	{
		size_t write_size = 0;
		size_t all_size = size * count;
		while(write_size < all_size)
		{
			size_t delta_size = all_size - write_size;
			size_t cur_write = delta_size > safe_size ? safe_size : delta_size;
			size_t writed = Write( (const void*)((unsigned char*)buffer + cur_write), 1, cur_write);
			write_size += writed;
			if (!writed)
			{
				break;
			}
		}
		return write_size;
	}

	xchar *Gets( xchar *buffer, int n)
	{
		return fgets(buffer, n, m_fp);
	}

	size_t Puts(xchar *buffer)
	{
		return fputs(buffer, m_fp);
	}

	size_t Flush()
	{
		return fflush(m_fp);
	}

	long Tell()
	{
		return ftell(m_fp);
	}

	long Seek( xulong offset, xint32 origin )
	{
		return fseek(m_fp, offset, origin);
	}

	inline long SeekSet(xlong offset)
	{
		return Seek(offset, SEEK_SET);
	}


	xulong Length()
	{
		xulong cur = Tell();
		Seek(0, SEEK_SET);
		xulong start = Tell();
		Seek(0, SEEK_END);
		xulong end = Tell();
		Seek(cur, SEEK_SET);
		return end - start;
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

class XFileMapBase : public XFile
{

};

class XFileMap : public XFileMapBase
{

};


#endif