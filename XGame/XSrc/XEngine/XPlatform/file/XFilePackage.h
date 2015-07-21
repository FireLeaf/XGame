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
#include<set>

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

class XFileMemory//���ڴ��е��ļ�
{
public:
	bool OpenFile(const char* path);
protected:
	void* buf;
	int file_length;
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
	//ÿ���ļ�д��ĸ�ʽ
	/*
		file_name//�䳤
		file_size
		file_content
		md5
	*/
	enum
	{
		XFILE_PACKAGE_MAGIC1 = 0xabcdef98,
		XFILE_PACKAGE_MAGIC2 = 0xfedcba11,
		XFILE_PACKAGE_SAFE_SIZE = 1024,
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
		int buf_len;//���ݳ���
		int org_len;//ԭ�����ݵĳ���
		int offset;//�������Ȳ�����2^31 - 1
		XEasyPackageRecord():compress_type(NONE_COMPRESS), buf_len(-1), offset(-1){}
	};

	struct ChipRecord//��Ƭ 
	{
		int reserve_len;
		int offset;
		ChipRecord(int res_len, int off):reserve_len(res_len), offset(off){}
		const bool operator < (const ChipRecord& right) const
		{
			return reserve_len < right.reserve_len;
		}
	};
	/*
		header//�ļ�ͷ
		all file_content//�����ļ�����
		records//��¼
		records offset//��¼���ļ���ƫ��
	*/
	typedef std::vector<XEasyPackageRecord> PackageRecords;
	typedef std::set<ChipRecord> ChipRecords;
	struct PackageTail 
	{
		PackageRecords package_records;
		ChipRecords chip_records;
	};
public:
	bool InitPackage(const char* fpk_file);
	
	bool CreatePackage(const char* fpk_file);
	bool SavePackageRecords();
	bool SavePackageHeader();

	bool AppendFileFromData(const char* path, const unsigned char* buffer, int length);
//	bool ReplaceFileFromData(const char* old_path, const char* cur_path, const unsigned char* buffer, int length);
	bool RewriteFileFromData(const char* path, const unsigned char* buffer, int length);
	bool RemoveFile(const char* path);
	bool AddFile(const char* full_path, const char* path);

	XEasyPackageRecord* FindRecord(const char* path);
	PackageRecords& GetPackageRecords(){return package_tail.package_records;}
public:
	bool ReadFileContent(const XEasyPackageRecord* record, void** buff, int* len);
	bool ReadFileContent(const char* path, void** buff, int* len);
protected:
	virtual bool LoadPackage(int version);
	bool FindMatchRecord(XFilePackageEasy::ChipRecord& chip_record, int length);
	XEasyPackageRecord* AddRecord(const char* path);
	bool AddBufferZlib(XEasyPackageRecord* record, const unsigned char* buffer, int length);
	//bool ReplaceRecord(const char* path, )
protected:
	PackageTail package_tail;
	//int cur_offset;
	int file_tail;
};

#endif // XFilePackage