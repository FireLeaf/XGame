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

	// lua_CFunction 函数返回值个数和入栈个数一致, 支持多个返回值
	bool AddFunction(const char* pFunctionName, lua_CFunction pFunction);

	// 获取 Lua 传递给 LuaGlue 函数的参数, 参数位置从1开始
	const char* GetStringArgument(int num, const char* pDefault = NULL);
	double GetNumberArgument(int num, double dDefault = 0.0);

	// 检查虚拟栈
	void StackDump();

	// 判断是否有足够的栈空间可用，一般，Lua会预留20个槽位
	int CheckStack(int extra);

public:
	void SetErrorHandler(pFnErrorHandler pErrHandler);

	lua_State* GetScriptContext();

	//////////////////C++ 向 Lua 栈存入元素///////////////////////////
	// 指定长度的内存数据
	bool PushMemoryStr(const char* s, size_t len);
	// 以零结尾的字符串，其长度可由strlen得出
	bool PushString(const char* pString);
	// 浮点数
	bool PushNumber(double value);
	// nil 值
	bool PushNil();
	// 布尔值
	bool PushBoolean(bool b);
	// 整形
	bool PushInteger(int val);

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