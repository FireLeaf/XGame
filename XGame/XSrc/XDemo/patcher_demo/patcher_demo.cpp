// patcher_demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "XPatcherDownload.h"
#include "XLog.h"

int _tmain(int argc, _TCHAR* argv[])
{
	XLog::Get().SetLogDir("./");
	XLog::Get().AddLogItem("download", "patcher.log");
	XLog::Get().LogOutput(true, "download", "�������ү%d", 123);
	curl_global_init(CURL_GLOBAL_ALL);
	XPatcherDownload pd;
	pd.DownloadFile("http://img.my.csdn.net/uploads/201005/30/0_1275231052eqfF.gif", "ddd.gif", 3);
	char sz[100];
	curl_global_cleanup();
	scanf("%s", sz);
	return 0;
}

