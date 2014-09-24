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

class XFileImageBase : public XFile
{

};

struct XFilePackageHeader 
{
	xint64 magic;//魔法数字
	xint32 version;//版本号

	xint32 file_count;//文件数量
	xint32 sub_package_count;//子包数量
	struct  
	{
		xint32 file_count;//子包文件数量

	}sub_package_desc[1];
};

struct XFileRecord
{
	XStl::string name;//文件名
	xint64 offset;//起始偏移
	xint64 length;//该文件长度
};

struct XDirRecord 
{
	XStl::string name;//目录名
	XStl::vector<XFileRecord> file_records;
	XStl::vector<XDirRecord> dir_records;
};

struct XSubPackageRecord : public XDirRecord
{
	//name为子包名
};

struct XPackageRecord
{
	XStl::string name;
	XStl::vector<XSubPackageRecord> sub_packages;
};

struct PackageCmp
{
public:
	PackageCmp(const char* name){this->name = name;}
	bool operator ()(const XPackageRecord* pr)
	{
		if (0 == strcmp(pr->name.c_str(), name.c_str()))
		{
			return true;
		}
		return false;
	}
protected:
	XStl::string name;
};

class XFilePackageBase : public XFile
{
public:
	xbool InitPackage(const char* path)
	{
// 		if (packages.end() != std:find_if(packages.begin(), packages.end(), PackageCmp(path)))
// 		{
// 			return true;
// 		}
// 
// 		CloseFile();
// 
// 		if(!OpenFile(path, "rb"))
// 		{
// 
// 		}
	}
protected:
	XStl::vector<XPackageRecord> packages;
};


#endif