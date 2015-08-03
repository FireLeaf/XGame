/**************************************************************************
/*		FileName: XPatcher.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/27 22:49:24
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XPATCHER__H
#define __XPATCHER__H

enum PATCHER_EVENT
{
	PE_DNS_RESLOVE,//DNS����
	PE_DOWNLOAD_PATCHER_CONFIG,//�������·������ϵ�patcher����
	PE_UPDATE_TO_NEXT_VERSION,//���µ���һ�汾����Դ
	PE_UPDATE_COMPLETE,//�������
};

enum PATCHER_STATE
{
	STATE_DNS_RESOLVE,
	STATE_WAIT_PATCHER_CONFIG,
	STATE_UPDATE_ASSET,
	STATE_COMBINE_ASSET,//�������������Դ
	STATE_COMPLETE,
};

enum HttpDownloadErrorReason
{
	HDErrorNone = 0,
	HDErrorTimeout = 1,
	HDErrorInterrupt = 2,
	HDErrorInternal = 3,
};

struct DownloadFileBlock 
{
	std::string url;
	std::string dest;
	int length;
	int use_seconds;
};

struct UploadFileBlock 
{
	std::string url;
	std::string src;
	int length;
	int use_seconds;
};

class DownloadWatcher//���ؼ�����
{
public:
	virtual void OnDownloadBegin(int ) = 0;//��ʼ����
	virtual void OnDownloadChange(int total_size, int cur_size, int real_speed) = 0;//
	virtual void OnDownloadEnd() = 0;//
	virtual void OnDownloadFinish() = 0;
};

typedef void (*pfnDownloadCallback)(DownloadFileBlock dfb);
typedef void (*pfnUploadCallback)(UploadFileBlock ufb);

class XPatcher
{
public:
	struct AssetVersion 
	{
		int main_version;
		int sub_version;
		int asset_version;
	};

	struct PatcherEvent
	{
		int event_type;
		int parm1;
		int parm2;
		int parm3;
	};

	struct PatcherState 
	{
		int state;
		int cur_version;
		int target_version;
		bool is_complete;
		int total_file;
		int getted_file;
		int real_speed;
	};
public:
	void TrigEvent(const PatcherEvent& pe);

	
	void QueryPatcherState(PatcherState& ps);

	void DownloadFile(const char* url, const char* dest_file, pfnDownloadCallback func);
	void UploadFile(const char* src_file, const char* url, pfnUploadCallback func);
protected:

};

#endif // XPatcher