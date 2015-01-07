/**************************************************************************
/*		FileName: XPatcherFile.h
/*		FileDesc: 补丁文件格式
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/30 23:42:30
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XPATCHERFILE__H
#define __XPATCHERFILE__H

class XPathcherFile : public XFilePackageEasy
{
	enum PatcherAction
	{
		PA_ADD_FILE,//添加文件
		PA_REP_FILE,//更换文件
		PA_DEL_FILE,//删除文件
	};
	struct PatcherItem 
	{
		std::string path;//路径
		std::string package;//仅当在某个包里是才填
		bool in_package;//是否是在包里
		int action;//操作
		int offset;//文件内容的偏移
		int buf_len;//压缩
		int org_len;//原来压缩长度
	};
};

#endif // XPatcherFile