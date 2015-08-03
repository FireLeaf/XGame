/**************************************************************************
/*		FileName: XPatcherFile.h
/*		FileDesc: �����ļ���ʽ
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
		PA_ADD_FILE,//����ļ�
		PA_REP_FILE,//�����ļ�
		PA_DEL_FILE,//ɾ���ļ�
		PA_MOD_FILE,//�����ļ�
	};

	enum 
	{
		PATCH_SAFE_SIZE = 1024,
	};

	struct DeleteItem 
	{
		std::string path;//·��,������ǰ�����ôͳһ���ڸ���Ŀ¼��
		std::string package;//������ĳ�������ǲ���
		DeleteItem()
		{
			path = "";
			package = "";
		}
	};
	
	struct AddItem 
	{
		int compress_type;
		std::string path;//·��,������ǰ�����ôͳһ���ڸ���Ŀ¼��
		std::string package;//������ĳ�������ǲ���
		int buf_len;//ѹ��
		int org_len;//ԭ��ѹ������
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