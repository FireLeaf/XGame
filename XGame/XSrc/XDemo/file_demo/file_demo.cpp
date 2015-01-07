// file_demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "io.h"
#include "string"

void SetFileSize(const char* file, long size)
{
	FILE* fp = fopen(file, "wb");
	if (fp)
		_chsize(_fileno(fp), size);
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
	const char* path = "C:\\Users\\pc\\Desktop\\mp.mb";
	WriteFileDis(path);
	return 0;
}

