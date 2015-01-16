/*******************************************************************************
	FILE:		XLog.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/16

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XLog.h"
#include <time.h>
#include <stdarg.h>
#include "XSys.h"

static const int MaxLogFileLength = 1 * 1024 * 1024;

XLog::XLog()
{

}

XLog::~XLog()
{
	for (int i = 0; i < log_items.size(); i++)
	{
		log_items[i]->file.CloseFile();
	}
}

XLog& XLog::Get()
{
	static XLog inst;
	return inst;
}

XLog::LogItem* XLog::FindLogItem(const char* tag)
{
	if (!tag)
	{
		return NULL;
	}

	for (int i = 0; i < log_items.size(); i++)
	{
		if (log_items[i])
		{
			if (strcmp(tag, log_items[i]->tag.c_str()) == 0)
			{
				return log_items[i];
			}
		}
	}

	return NULL;
}

bool XLog::AddLogItem(const char* tag, const char* file_name)
{
	if (FindLogItem(tag))
	{
		return true;
	}
	std::string path = log_path + "/";
	path += file_name;
	if(XSys::XCreateFile(path.c_str()))
	{
		LogItem* item = new LogItem;
		Assert(item);
		if(!item->file.OpenFile(path.c_str(), "at"))
		{
			delete item;
			return false;
		}
		item->file_name = path;
		item->tag = tag;
		log_items.push_back(item);
		//如果日志很长了 那么清掉
		long file_size = item->file.Length();
		if (file_size > MaxLogFileLength)
		{
			XSys::XSetFileSize(item->file.GetFileHandle(), 1);
			LogOutput(true, tag, "clear log info! log file size: %d!", file_size);
		}
		return true;
	}
	return false;
}

void XLog::LogOutput(bool device_print, const char* tag, const char* msg, ...)
{
	char msg_buf[1024];
	va_list args_list;
	va_start(args_list, msg);
	vsnprintf(msg_buf, sizeof(msg_buf), msg, args_list);
	va_end(args_list);

	if (device_print)
	{
		XSys::XLogOutput(msg_buf);
	}

	LogItem* log_item = FindLogItem(tag);
	if (log_item)
	{
		time_t tt = time(NULL);
		struct tm* t = gmtime(&tt);
		fprintf(log_item->file.GetFileHandle(), "%d-%d-%d %d:%d:%d %s\n", t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, msg_buf);
		log_item->file.Flush();
	}
}