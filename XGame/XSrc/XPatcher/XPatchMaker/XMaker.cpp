/**************************************************************************
/*		FileName: XMaker.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2015/08/01 14:15:33
/*		CopyRight: yc 
/*************************************************************************/

#include "XMaker.h"
#include "XFile.h"
#include "XFilePackManager.h"
#include "XMD5.h"
#include "Qt/qsettings.h"
#include "qt/qstringlist.h"
#include "qt/qfile.h"
#include "qiodevice.h"
#include "qt/qdir.h"
#include "qt/qfileinfo.h"
#include "XPatcherFile.h"
#include "QStandardItem"
#include "qt/QStandardItemModel.h"

extern const char* g_FpkName[64];

XMaker& XMaker::GetInstance()
{
	static XMaker maker;
	return maker;
}

XMaker::XMaker()
{
	select_index = -1;
	log_list = NULL;
	upload_list = NULL;
}

bool XMaker::Init(QListView* logList, QListView* uploadList)
{
	if (!LoadConfig())
	{
		return false;
	}
	log_list = logList;
	upload_list = uploadList;
	return true;
}

bool XMaker::SaveMD5(QString& path, PathToMd5Map& md5map)
{
	//¼ÇÂ¼md5
	QString md5_dir = path + QString("/md5.txt");
	std::string std_md5_dir = md5_dir.toLocal8Bit();
	XFile file;
	if (file.OpenFile(std_md5_dir.c_str(), "wb"))
	{
		int iCount = map_path_to_md5.size();
		file.QuickWriteValue(iCount);
		for (PathToMd5Map::iterator iter = md5map.begin(); iter != md5map.end(); iter++)
		{
			std::string file_key = iter.key().toLocal8Bit();
			MD5_Value file_value;
			file.QuickWriteString(file_key);
			file.Write(&(iter.value().digist), 1, sizeof(file_value));
		}
	}
	return true;
}

bool XMaker::Save()
{
	QString asset_root_dir = channel_list[select_index].local_asset_dir;
	QString asset_dir = asset_root_dir + QString("/asset/");
	
	SaveMD5(asset_root_dir, map_path_to_md5);
	return true;
}

bool XMaker::LoadConfig()
{
	QSettings channelConf("AssetChannel.ini", QSettings::IniFormat);
	QStringList channels = channelConf.childGroups();
	for (int i = 0; i < channels.size(); i++)
	{
		Channel channel;
		//toLocal8Bit()
		channel.channel_name = channels[i];
		channelConf.beginGroup(channels[i]);
		channel.local_asset_dir = channelConf.value("asset_dir").toString();
		channel.upload_url = channelConf.value("upload_url").toString();
		channelConf.endGroup();
		channel_list.push_back(channel);
	}
	return true;
}

bool XMaker::SelectChannel(int iIndex)
{
	if (iIndex < 0 || iIndex >= channel_list.size())
	{
		return false;
	}

	select_index = iIndex;

	if(!LoadMD5())
	{
		return false;
	}
	
	QString asset_root_dir = channel_list[iIndex].local_asset_dir;
	QString asset_dir = asset_root_dir + QString("/asset");
	QString asset_version_path = asset_dir + QString("/asset_version.txt");
	QFile file(asset_version_path);
	if ( !file.open( QIODevice::ReadOnly ) ) 
	{
		return false;
	}

	file.readLine(version, sizeof(version) - 1);
	int iret = sscanf(version, "%d.%d.%d", &av.main_version, &av.sub_version, &av.asset_version);
	if (iret != 3)
	{
		return false;
	}
	return true;
}

void XMaker::QuickLog(QString& log_info)
{
	((QStandardItemModel*)(log_list->model()))->appendRow(new QStandardItem(log_info));
}

void XMaker::QuickUploadInfo(QString& upload_info)
{
	((QStandardItemModel*)(upload_list->model()))->appendRow(new QStandardItem(upload_info));
}

bool XMaker::LoadMD5()
{
	map_path_to_md5.clear();
	QString asset_root_dir = channel_list[select_index].local_asset_dir;
	QString asset_dir = asset_root_dir + QString("/asset");
// 	QSettings Md5Conf(asset_root_dir + QString("md5.txt"), QSettings::IniFormat);
// 	Md5Conf.beginGroup("md5");
// 	QStringList md5keys = Md5Conf.allKeys();
// 	for (int i = 0; i < md5keys.size(); i++)
// 	{
// 		QString md5 = Md5Conf.value(md5keys[i]).toString();
// 		map_path_to_md5[md5keys[i]] = md5;
// 	}
// 	Md5Conf.endGroup();
	QString md5_dir = asset_root_dir + QString("\\md5.txt");
	std::string std_md5_dir = md5_dir.toLocal8Bit();
	XFile file;
	if (file.OpenFile(std_md5_dir.c_str(), "rb"))
	{
		int iCount = 0;
		file.QuickReadValue(iCount);
		for (int i = 0; i < iCount; i++)
		{
			std::string file_key;
			MD5_Value file_value;
			file.QuickReadString(file_key);
			file.Read(&file_value, 1, sizeof(file_value));
			map_path_to_md5[QString(file_key.c_str())] = file_value;
		}
	}
	return true;
}

void XMaker::GenerateMD5(QDir& file_dir, QString parent_path, XMaker::PathToMd5Map& mapPathToMd5)
{
	QFileInfoList file_list = file_dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	//file_dir.setSorting(QDir::Name | QDir::Reversed);
	for (int file_index = 0; file_index < file_list.size(); file_index++)
	{
		const QFileInfo& file_info = file_list.at(file_index);
		std::string file_path = file_info.fileName().toLocal8Bit();
		if (file_path == "." || file_path == "..")
		{
			continue;
		}
		else if (file_info.isDir())
		{
			QString tmp_parent = parent_path;
			tmp_parent.append("/");
			tmp_parent.append(file_info.fileName());
			QString abs_path = file_dir.absolutePath();
			abs_path.append("/");
			abs_path.append(file_info.fileName());
			QDir dir(abs_path);
			if (!dir.exists())
			{
				continue;
			}
			GenerateMD5(dir, tmp_parent, mapPathToMd5);
		}
		else if (file_info.isFile())
		{
			QString cur_file_path = parent_path;
			//cur_file_path.append("/");
			cur_file_path.append(file_info.fileName());
			std::string strCurFile = cur_file_path.toLocal8Bit();
			bool is_fpk = false;
			for (int i = 0; i < FPK_COUNT; i++)
			{
				if(file_info.fileName() == (g_FpkName[i] + QString(".fpk")) )
				{
					is_fpk = true;
					XFilePackageEasy fpe;
					std::string absulote_path = file_info.absoluteFilePath().toLocal8Bit();
					if (!fpe.InitPackage(absulote_path.c_str()))
					{
						continue;
						//return;
					}
					XFilePackageEasy::PackageRecords& records = fpe.GetPackageRecords();
					for (int j = 0; j < records.size(); j++)
					{
						void* buffer = NULL;
						int len = 0;
						unsigned char digist[16];
						if (fpe.ReadFileContent(&records[j], &buffer, &len))
						{
							XMaker::MD5_Value value;
							MD5Memory((unsigned char*)buffer, len, value.digist);
							QString pathkey = ((parent_path == "") ? "/" : parent_path) + file_info.baseName();
							pathkey.append(records[j].path.c_str());

							mapPathToMd5[pathkey] = value;
							QString loginfo = "MD5 info: " + pathkey;
							loginfo += ":";
							loginfo += (char*)value.digist;
							QuickLog(loginfo);
							delete buffer;
						}
					}
				}
			}
			if (!is_fpk)
			{
				if (file_info.fileName() == "asset_version.txt")
				{
					continue;
				}
				XMaker::MD5_Value value;
				QString pathkey = ((parent_path == "") ? QString("/") : parent_path) + file_info.fileName();
				std::string str_path_key = pathkey.toLocal8Bit();
				MD5File(std::string(file_info.absoluteFilePath().toLocal8Bit()).c_str(), value.digist);
				mapPathToMd5[pathkey] = value;
				QString loginfo = "MD5 info: " + pathkey;
				loginfo += ":";
				loginfo += (char*)value.digist;
				QuickLog(loginfo);
			}
		}
	}
}

bool XMaker::GenerateLocalMD5()
{
	if (select_index < 0 || select_index >= channel_list.size())
	{
		return false;
	}
	map_path_to_md5.clear();
	QString asset_root_dir = channel_list[select_index].local_asset_dir;
	QString asset_dir = asset_root_dir + QString("/asset");
	QDir dir(asset_dir);
	if (!dir.exists())
	{
		return true;
	}
	GenerateMD5(dir, "", map_path_to_md5);
	return true;
}

bool XMaker::LoadPreUploadFile()
{
	return true;
}

void XMaker::CompareFile(QDir& file_dir, QString parent_path, XPathcherFile& patchfile)
{
	int add_file_count = 0;
	QFileInfoList file_list = file_dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	//file_dir.setSorting(QDir::Name | QDir::Reversed);
	for (int file_index = 0; file_index < file_list.size(); file_index++)
	{
		const QFileInfo& file_info = file_list.at(file_index);
		std::string file_path = file_info.fileName().toLocal8Bit();
		if (file_path == "." || file_path == "..")
		{
			continue;
		}
		else if (file_info.isDir())
		{
			QString tmp_parent = parent_path;
			tmp_parent.append("/");
			tmp_parent.append(file_info.fileName());
			QString abs_path = file_dir.absolutePath();
			abs_path.append("/");
			abs_path.append(file_info.fileName());
			QDir dir(abs_path);
			if (!dir.exists())
			{
				continue;
			}
			CompareFile(dir, tmp_parent, patchfile);
		}
		else if (file_info.isFile())
		{
			QString cur_file_path = parent_path;
			cur_file_path.append("/");
			cur_file_path.append(file_info.fileName());
			std::string strCurFile = cur_file_path.toLocal8Bit();
			bool is_fpk = false;
			for (int i = 0; i < FPK_COUNT; i++)
			{
				if(file_info.fileName() == (g_FpkName[i] + QString(".fpk")) )
				{
					is_fpk = true;
					XFilePackageEasy fpe;
					std::string absulote_path = file_info.absoluteFilePath().toLocal8Bit();
					if (!fpe.InitPackage(absulote_path.c_str()))
					{
						continue;
						//return;
					}
					XFilePackageEasy::PackageRecords& records = fpe.GetPackageRecords();
					for (int j = 0; j < records.size(); j++)
					{
						void* buffer = NULL;
						int len = 0;
						unsigned char digist[16];
						if (fpe.ReadFileContent(&records[j], &buffer, &len))
						{
							XMaker::MD5_Value value;
							MD5Memory((unsigned char*)buffer, len, value.digist);
							QString pathkey = ((parent_path == "") ? QString("/") : parent_path) + file_info.baseName();
							//pathkey.append( "/");
							pathkey.append(records[j].path.c_str());
							std::string strpathkey = pathkey.toLocal8Bit();
							PathToMd5Map::iterator iter = map_path_to_md5.find(pathkey);
							if (iter != map_path_to_md5.end() && iter.value() == value)
							{
								continue;
								delete buffer;
							}
							cache_path_to_md5[pathkey] = value;
							patchfile.AddFile(buffer, len, records[j].path.c_str(), g_FpkName[i]);
							QString strPatchInfo = QString("patch_msg: add file[") + records[j].path.c_str();
							strPatchInfo += QString("] to package -> ") + QString(g_FpkName[i]);
							add_file_count++;
							QuickUploadInfo(strPatchInfo);
							delete buffer;
						}
					}
				}
			}
			if (!is_fpk)
			{
				XMaker::MD5_Value value;
				QString pathkey = ((parent_path == "") ? QString("/") : parent_path) + file_info.fileName();
				MD5File(std::string(file_info.absoluteFilePath().toLocal8Bit()).c_str(), value.digist);
				PathToMd5Map::iterator iter = map_path_to_md5.find(pathkey);
				if (iter != map_path_to_md5.end() && iter.value() == value)
				{
					continue;
				}
				cache_path_to_md5[pathkey] = value;
				patchfile.AddFile(std::string(file_info.absoluteFilePath().toLocal8Bit()).c_str(), std::string(pathkey.toLocal8Bit()).c_str(), NULL);
				add_file_count++;
				QString strPatchInfo = QString("patch_msg: add file ->") + std::string(pathkey.toLocal8Bit()).c_str();
				QuickUploadInfo(strPatchInfo);
			}
		}
	}
	patchfile.QuickWriteValue(add_file_count);
}

bool XMaker::GenerateUploadFile()
{
	if (select_index < 0 || select_index >= channel_list.size())
	{
		return false;
	}
	cache_path_to_md5 = map_path_to_md5;
	QString asset_root_dir = channel_list[select_index].local_asset_dir;
	QString asset_upload_dir = asset_root_dir + "/upload";
	XPathcherFile patch;
	char file_name[128] = "\0";
	sprintf(file_name, "patch_%d_%d_%d.p", av.main_version, av.sub_version, av.asset_version + 1);
	std::string patch_dir = asset_root_dir.toLocal8Bit();
	patch_dir += "/upload/";
	patch_dir += file_name;
	if (!patch.CreatePatch(patch_dir.c_str()))
	{
		return false;
	}
	patch.DelFiles(std::vector<DeleteItem>());
	std::string ver_dir = asset_root_dir.toLocal8Bit();
	ver_dir += "/upload/";
	ver_dir += "asset_version.txt";
	XFile ver_fp;
	if(ver_fp.OpenFile(ver_dir.c_str(), "wt"))
	{
		fprintf(ver_fp.GetFileHandle(), "%d.%d.%d", av.main_version, av.sub_version, av.asset_version + 1);
	}
	ver_fp.CloseFile();

	QString asset_preupload_dir = asset_root_dir;
	asset_preupload_dir += "/preupload";
	QDir dir(asset_preupload_dir);
	if (!dir.exists())
	{
		return true;
	}
	CompareFile(dir, "", patch);
	SaveMD5(asset_upload_dir, map_path_to_md5);
	return true;
}