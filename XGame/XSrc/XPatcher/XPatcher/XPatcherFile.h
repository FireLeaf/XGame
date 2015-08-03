/**************************************************************************
/*		FileName: XPatcherFile.h
/*		FileDesc: 补丁文件格式
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/30 23:42:30
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XPATCHERFILE__H
#define __XPATCHERFILE__H

#include "XFilePackage.h"
#include "XFilePackManager.h"
class XPathcherFile : public XFile
{
	enum PatcherAction
	{
		PA_ADD_FILE,//添加文件
		PA_REP_FILE,//更换文件
		PA_DEL_FILE,//删除文件
		PA_MOD_FILE,//更新文件
	};

	enum 
	{
		PATCH_SAFE_SIZE = 1024,
	};

	struct DeleteItem 
	{
		std::string path;//路径,如果不是包里那么统一放在更新目录下
		std::string package;//仅当在某个包里是才填
		DeleteItem()
		{
			path = "";
			package = "";
		}
	};
	
	struct AddItem 
	{
		int compress_type;
		std::string path;//路径,如果不是包里那么统一放在更新目录下
		std::string package;//仅当在某个包里是才填
		int buf_len;//压缩
		int org_len;//原来压缩长度
		AddItem()
		{
			compress_type = NONE_COMPRESS;
			path = "";
			package = "";
			buf_len = 0;
			org_len = 0;
		}
	};
public:
	bool LoadPatch(const char* path);
	bool ApplyPatch(XFilePackManage* pFilePackMan);
	bool AddFile(const char* src_file, const char* file, const char* package = NULL);
	bool AddFile(const void* buf, int len, const char* file, const char* package = NULL);
	bool DelFiles(std::vector<DeleteItem>& vecDelItems);
	bool DelFile(const DeleteItem& delItem);
	bool DelFile(const char* file, const char* package = NULL);
	bool CreatePatch(const char* path);
	bool FlushPath();
protected:
	bool ReadFileContent(const AddItem& addItem, void** buff, int* len);
	bool CompressFileData(const void** data, int len, int& outlen, int& compress_type, void** outBuffer);
protected:
	std::vector<DeleteItem> patch_del_items;
	//std::vector<AddItem> patch_add_items;

};

#endif // XPatcherFile