#ifndef _CLOG_H
#define _CLOG_H
#include<stdio.h>
#include <windows.h>
#define LOGFILENAME "www.gamerefer.com.log"

class CLog
{
private:
//	char szFileName[512];
	FILE *fp;
	bool Write(char *prefix,char *info);
public:
	CLog();
	CLog(char *fileName);
	~CLog();

	bool WriteError(char *errorInfo);
	//bool WriteErrorW(wchar_t *errorInfo);
	bool WriteRight(char *rightInfo);
	bool WriteSeg(char *segInfo);
	bool WriteWarr(char *warronInfo);
	bool WriteInfo(char *charNormalInfo);
	bool WriteInfo(float *floatNormalInfo,int length);
	bool WriteInfo(byte *byteNormalInfo,int length);
};
#endif