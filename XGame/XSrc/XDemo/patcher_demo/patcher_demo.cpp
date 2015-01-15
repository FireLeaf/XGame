// patcher_demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "XPatcherDownload.h"

int _tmain(int argc, _TCHAR* argv[])
{

	curl_global_init(CURL_GLOBAL_ALL);
	XPatcherDownload pd;
	pd.DownloadFile("http://img.my.csdn.net/uploads/201005/30/0_1275231052eqfF.gif", "ddd.gif", 3);
	char sz[100];
	curl_global_cleanup();
	scanf("%s", sz);
	return 0;
}

