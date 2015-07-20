// file_demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "io.h"
//#include <string>
#include "XFilePackage.h"
#include <string>

void SetFileSize(const char* file, long size)
{
	FILE* fp = fopen(file, "wb");
	//if (fp)
	//	_chsize(_fileno(fp), size);
	fclose(fp);
}



void WriteFileDis(const char* path)
{
	SetFileSize(path, 1000);
	FILE* fps[10];
	for (int i = 0; i < 10; i++)
	{
		fps[i] = fopen(path, "r+b");
	}
	for (int i = 0; i < 10; i++)
	{
		fseek(fps[i], i * 100, SEEK_SET);
		char buf[10] = {'\0'};
		memset(buf, i + 'a', 10);
		for (int j = 0; j < 10; j++)
		{
			memset(buf, i + j + 'a', 10);
			fwrite(buf, 1, 10, fps[i]);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//const char* path = "C:\\Users\\pc\\Desktop\\mp.mb";
	//WriteFileDis(path);
	XFilePackageEasy package;
	if (!package.InitPackage("E:\\ProjectXGame\\XGame\\XSrc\\XPatcher\\XFPKPacker\\拳皇97.fpk"))
	{
		return -1;
	}

	while(true)
	{
		char buf[1024] = "";
		char op = '\0';
		char file[1023] = "";
		scanf("%s", buf);
		if (strcmp(buf, "quit") == 0)
		{
			break;
		}
		try
		{
			int ret = sscanf(buf, "%c:%s", &op, file);
			if (op == '-')
			{
				package.RemoveFile(file);
			}
			else if(op == '+')
			{
				char srcfile[1024] = "";
				sscanf(buf, "%c:%[^-]-%[^-]", &op, srcfile, file);
				package.AddFile(srcfile, file);
			}
		}
		catch(...)
		{

		}
	}
	package.SavePackageRecords();
	package.Flush();
	package.CloseFile();
	return 0;
}

