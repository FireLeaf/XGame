/*******************************************************************************
	FILE:		XUnPacker.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/05

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XUnPacker.h"
#include "qt/qdir.h"
#include "XFilePackage.h"
#include "XSys.h"

XUnPacker::XUnPacker()
{

}

XUnPacker& XUnPacker::Get()
{
	static XUnPacker inst;
	return inst;
}

void CreateFileAndDirectory(const char* path)
{
	char szTempPath[260] = {'\0'};
	if(strlen(path) > 260)
		return ;
	strcpy(szTempPath,path);//本身想用 _tcscpy_s  可是 不知道为何 此处调用这个函数会修改 szFilePath 的值.. 见鬼了

	for(int i=0,j=0;szTempPath[i]!=0;++i)
	{
		if(szTempPath[i] == ('\\') || szTempPath[i] == '/')
		{
			szTempPath[i] = 0;
			XSys::XCreateDirectory(szTempPath);
			szTempPath[i] = ('\\');
		}
	}
	
	XSys::XCreateFile(path);
}

bool XUnPacker::UnPack(const QString& fpk, const QString& dest_dir)
{
// 	if (!QDir::exists(dest_dir))
// 	{
// 		QDir::mkdir(dest_dir);
// 	}
// 
// 	if (!QDir::exists(dest_dir))
// 	{
// 		return false;
// 	}
	XFilePackageEasy fpe;
	std::string fpk_path = fpk.toStdString().c_str();
	if (!fpe.InitPackage(fpk_path.c_str()))
	{
		return false;
	}

	XFilePackageEasy::PackageRecords& records = fpe.GetPackageRecords();
	for (int i = 0; i < records.size(); i++)
	{
		XFile file;
		QString file_path = dest_dir;
		file_path.append(records[i].path.c_str());
		CreateFileAndDirectory(file_path.toStdString().c_str());
		if (file.OpenFile(file_path.toStdString().c_str(), "wb"))
		{
			void* buffer = NULL;
			int len = 0;
			if (fpe.ReadFileContent(&records[i], &buffer, &len))
			{
				if (len != file.SafeWrite(buffer, 1, len, 1024))
				{
					Assert(0);
				}
			}
			file.Flush();
		}
	}
	return true;
}