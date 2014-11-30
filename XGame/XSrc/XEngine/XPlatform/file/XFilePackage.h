/**************************************************************************
/*		FileName: XFilePackage.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/16 13:42:29
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XFILEPACKAGE__H
#define __XFILEPACKAGE__H

#include "XFile.h"

#define 

class XFileImageBase : public XFile
{

};

struct XFilePackageHeader 
{
	xint64 magic;//魔法数字
	xint32 version;//版本号

// 	xint32 file_count;//文件数量
// 	xint32 sub_package_count;//子包数量
// 	struct  
// 	{
// 		xint32 file_count;//子包文件数量
// 
// 	}sub_package_desc[1];
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

class XFilePackage : public XFile
{
public:
	xbool InitPackage(const char* path);

	bool AppendFile(const char* path);
	bool AppendFile(const char* path, const unsigned char* buffer, int length);
	bool ReplaceFile(const char* src_path, const char* dest_path);
	bool ReplaceFile(const char* src_path, const char* dest_path, const unsigned char* buffer, int length);
	bool RemoveFile(const char* path);
protected:
	XStl::vector<XPackageRecord> packages;
};

class XFilePackageEasy : public XFile//简版
{
public:
	enum
	{
		XFILE_PACKAGE_MAGIC1 = 0xabcdef98,
		XFILE_PACKAGE_MAGIC2 = 0xfedcba11,
		XFILE_PACKAGE_SAFE_SIZE = 1024,
	};
	enum
	{
		NONE_COMPRESS,//无压缩
		Z_LIB_COMPRESS,//zlib压缩
		7Z_COMPRESS,//7z压缩
	};
	struct XEasyFPKHeader 
	{
		int magic1;
		int magic2;
		int version;
	};
	struct XEasyPackageRecord 
	{
		std::string path;
		int compress_type;
		int offset;//单包长度不超过2^31 - 1
	};
	/*
		header//文件头
		all file_content//所有文件内容
		records//记录
		records offset//记录在文件中偏移
	*/
	typedef std::vector<XEasyPackageRecord> PackageRecords;
public:
	bool InitPackage(const char* fpk_file);
	
	bool CreatePackage(const char* fpk_file);
	bool SavePackageRecords();
	bool SavePackageHeader();

	bool AppendFile(const char* path, const unsigned char* buffer, int length);
	bool ReplaceFile(const char* old_path, const char* cur_path, const unsigned char* buffer, int length);
	bool RewriteFile(const char* path, const unsigned char* buffer, int length);
	bool RemoveFile(const char* path);

	XEasyPackageRecord* FindRecord(const char* path);
protected:
	bool LoadPackage(int version);
	XEasyPackageRecord* AddRecord(const char* path);
	bool AddBufferZlib(XEasyPackageRecord* record, const unsigned char* buffer, int length);
	//bool ReplaceRecord(const char* path, )
protected:
	PackageRecords package_records;
	int cur_offset;
};

#endif // XFilePackage