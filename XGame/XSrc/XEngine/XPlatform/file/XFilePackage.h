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
	xint64 magic;//ħ������
	xint32 version;//�汾��

// 	xint32 file_count;//�ļ�����
// 	xint32 sub_package_count;//�Ӱ�����
// 	struct  
// 	{
// 		xint32 file_count;//�Ӱ��ļ�����
// 
// 	}sub_package_desc[1];
};

struct XFileRecord
{
	XStl::string name;//�ļ���
	xint64 offset;//��ʼƫ��
	xint64 length;//���ļ�����
};

struct XDirRecord 
{
	XStl::string name;//Ŀ¼��
	XStl::vector<XFileRecord> file_records;
	XStl::vector<XDirRecord> dir_records;
};

struct XSubPackageRecord : public XDirRecord
{
	//nameΪ�Ӱ���
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

class XFilePackageEasy : public XFile//���
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
		NONE_COMPRESS,//��ѹ��
		Z_LIB_COMPRESS,//zlibѹ��
		7Z_COMPRESS,//7zѹ��
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
		int offset;//�������Ȳ�����2^31 - 1
	};
	/*
		header//�ļ�ͷ
		all file_content//�����ļ�����
		records//��¼
		records offset//��¼���ļ���ƫ��
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