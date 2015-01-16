/*******************************************************************************
	FILE:		XLog.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/16

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XLOG_H_
#define __COCONAT_XLOG_H_

#include <string>
#include "XFile.h"

class XLog
{
public:
	struct LogItem 
	{
		std::string tag;
		std::string file_name;
		XFile file;
	};
public:
	XLog();
	~XLog();
	static XLog& Get();
public:
	void SetLogDir(const char* path){log_path = path;}
	void LogOutput(bool device_print, const char* tag, const char* szMsg, ...);
	bool AddLogItem(const char* tag, const char* file_name);
protected:
	LogItem* FindLogItem(const char* tag);
protected:
	std::vector<LogItem*> log_items;
	std::string log_path;
};

#endif