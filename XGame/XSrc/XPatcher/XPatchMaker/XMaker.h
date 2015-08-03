/**************************************************************************
/*		FileName: XMaker.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2015/08/01 13:55:13
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XMAKER__H
#define __XMAKER__H

#include <hash_map>
#include <string>
#include <Qt/qstring.h>
#include "Qt/qhash.h"
#include "qt/qdir.h"
#include "XPatcherFile.h"
#include "qt/qlistview.h"
class XMaker
{
public:
	struct Channel 
	{
		QString channel_name;
		QString local_asset_dir;
		QString upload_url;
	};
	
	struct AssetVersion 
	{
		int main_version;
		int sub_version;
		int asset_version;
	};

	struct MD5_Value 
	{
		unsigned char digist[16];
		bool operator == (const MD5_Value& rh)
		{
			return (memcmp(digist, rh.digist, sizeof(digist)) == 0);
		}
	};

	typedef /*stdext::hash_map*/QHash<QString, MD5_Value> PathToMd5Map;

	static XMaker& GetInstance();
public:
	XMaker();
	bool Init(QListView* logList, QListView* uploadList);
	bool Save();
public:
	const char* GetVersion()const{return version;}
	const std::vector<Channel>& GetChannels() const{return channel_list;}
	bool SelectChannel(int iIndex);
	bool GenerateLocalMD5();//生成本地资源的md5
	bool GenerateUploadFile();//生成上传更新资源
protected:
	bool LoadConfig();
	bool LoadMD5();
	bool SaveMD5(QString& path, PathToMd5Map& md5map);
	void QuickLog(QString& log_info);
	void QuickUploadInfo(QString& upload_info);

	bool LoadPreUploadFile();//加载预上传资源
	void CompareFile(QDir& file_dir, QString parent_path, XPathcherFile& patchfile);
	void GenerateMD5(QDir& file_dir, QString parent_path, XMaker::PathToMd5Map& mapPathToMd5);
protected:
	QListView* log_list;
	QListView* upload_list;

	PathToMd5Map map_path_to_md5;
	PathToMd5Map cache_path_to_md5;//这个是最终的md5值
	std::vector<Channel> channel_list;
	int select_index;
	char version[64];

	/*XPatcher::*/AssetVersion av;
};

#endif // XMaker