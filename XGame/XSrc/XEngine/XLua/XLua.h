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
#include "LuaLib/lua.hpp"

class CXLua
{
public:
	typedef void (*pFnErrorHandler)(const char* pError);
public:
	// Lua 运行环境初始化
	CXLua();
	// 释放 Lua_State
	virtual ~CXLua();

public:
	const char* GetErrorString();

	// 执行包含 Lua 代码字符串、文件
	bool RunScript(const char* pFileName);
	bool RunString(const char* pCommand);

	bool AddFunction(const char* pFunctionName, lua_CFunction pFunction);

	// 获取 Lua 传递给 LuaGlue 函数的参数, 参数位置从1开始
	const char* GetStringArgument(int num, const char* pDefault = NULL);
	double GetNumberArgument(int num, double dDefault = 0.0);

	// C++ 向 Lua 栈存入返回值, LuaGlue 函数返回值个数和入栈个数一致, 支持多个返回值
	bool PushString(const char* pString);
	bool PushNumber(double value);

public:
	void SetErrorHandler(pFnErrorHandler pErrHandler);

	lua_State* GetScriptContext();

private:
	void CreateLuaRuntime();
	void HandleError(char* format, ...);
private:
	// Lua 栈
	lua_State* m_pLuaState;
	pFnErrorHandler m_pErrorHandler;
};

#include "XLua.inl"

#endif // XLua