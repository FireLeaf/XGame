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

class XFileImageBase : public XFile
{

};

struct XFilePackageHeader 
{
	xint64 magic;//ħ������
	xint32 version;//�汾��

	xint32 file_count;//�ļ�����
	xint32 sub_package_count;//�Ӱ�����
	struct  
	{
		xint32 file_count;//�Ӱ��ļ�����

	}sub_package_desc[1];
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

#endif // XFilePackage