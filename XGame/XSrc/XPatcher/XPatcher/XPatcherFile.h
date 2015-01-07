/**************************************************************************
/*		FileName: XPatcherFile.h
/*		FileDesc: �����ļ���ʽ
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
		PA_ADD_FILE,//����ļ�
		PA_REP_FILE,//�����ļ�
		PA_DEL_FILE,//ɾ���ļ�
	};
	struct PatcherItem 
	{
		std::string path;//·��
		std::string package;//������ĳ�������ǲ���
		bool in_package;//�Ƿ����ڰ���
		int action;//����
		int offset;//�ļ����ݵ�ƫ��
		int buf_len;//ѹ��
		int org_len;//ԭ��ѹ������
	};
};

#endif // XPatcherFile