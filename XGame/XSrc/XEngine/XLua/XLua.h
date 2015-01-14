/****************************************************
 *		FileName: XLua.h
 *		FileDesc: C++调用Lua
 *		FileAuthor:
 *		CreateTime: 2014/11/03 22:09:40
 *		CopyRight:
 ***************************************************/

#ifndef __XLUA_H__
#define __XLUA_H__

#include "XLuaPubFunc.h"

// 包含 Lua 头文件
extern "C" {
#include "LuaLib/lua.h"
#include "LuaLib/lualib.h"
#include "LuaLib/lauxlib.h"
};

#define LuaGlue extern "C" int
extern "C" {
typedef int (*LuaFunction)(struct lua_State* pLuaState);
};

class CXLua
{
public:
	// Lua 运行环境初始化
	CXLua();
	// 释放 Lua_State
	virtual ~CXLua();

public:
	// 执行包含 Lua 代码字符串、文件
	bool RunScript(const char* pFileName);
	bool RunString(const char* pCommand);

	bool AddFunction(const char* pFunctionName, LuaFunction pFunction);
	const char* GetErrorString();

	// 获取 Lua 传递给 LuaGlue 函数的参数, 参数位置从1开始
	const char* GetStringArgument(int num, const char* pDefault = NULL);
	double GetNumberArgument(int num, double dDefault = 0.0);

	// C++ 向 Lua 栈存入返回值, LuaGlue 函数返回值个数和入栈个数一致, 支持多个返回值
	void PushString(const char* pString);
	void PushNumber(double value);
private:
	// Lua 栈
	lua_State* m_pLuaState;
};

#endif // XLua