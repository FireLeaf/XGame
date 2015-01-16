/****************************************************
 *		FileName: XLuaRegistry.h
 *		FileDesc: Lua 扩展库 注册表操作类
 *		FileAuthor:
 *		CreateTime: 2015/01/16 22:09:40
 *		CopyRight:
 ***************************************************/

#ifndef __XLUA_REGISTRY_H__
#define __XLUA_REGISTRY_H__

#include "XLuaDefine.h"

class XLuaRegistry
{
public:
	XLuaRegistry();
	virtual ~XLuaRegistry();

	void Init(const char* baseKey);

	int GetInteger(const char* key);
	std::string GetString(const char* key);

	void SetInteger(const char* key, int value);
	void SetString(const char* key, const char* value);

protected:
	std::string m_strBaseKey;
};

#endif