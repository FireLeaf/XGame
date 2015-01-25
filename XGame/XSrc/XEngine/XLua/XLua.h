/****************************************************
 *		FileName: XLua.h
 *		FileDesc: C++����Lua
 *		FileAuthor:
 *		CreateTime: 2014/11/03 22:09:40
 *		CopyRight:
 ***************************************************/

#ifndef __XLUA_H__
#define __XLUA_H__

#include "XLuaPubFunc.h"

// ���� Lua ͷ�ļ�
#include "LuaLib/lua.hpp"

class CXLua
{
public:
	typedef void (*pFnErrorHandler)(const char* pError);
public:
	// Lua ���л�����ʼ��
	CXLua();
	// �ͷ� Lua_State
	virtual ~CXLua();

public:
	const char* GetErrorString();

	// ִ�а��� Lua �����ַ������ļ�
	bool RunScript(const char* pFileName);
	bool RunString(const char* pCommand);

	bool AddFunction(const char* pFunctionName, lua_CFunction pFunction);

	// ��ȡ Lua ���ݸ� LuaGlue �����Ĳ���, ����λ�ô�1��ʼ
	const char* GetStringArgument(int num, const char* pDefault = NULL);
	double GetNumberArgument(int num, double dDefault = 0.0);

	// C++ �� Lua ջ���뷵��ֵ, LuaGlue ��������ֵ��������ջ����һ��, ֧�ֶ������ֵ
	bool PushString(const char* pString);
	bool PushNumber(double value);

public:
	void SetErrorHandler(pFnErrorHandler pErrHandler);

	lua_State* GetScriptContext();

private:
	void CreateLuaRuntime();
	void HandleError(char* format, ...);
private:
	// Lua ջ
	lua_State* m_pLuaState;
	pFnErrorHandler m_pErrorHandler;
};

#include "XLua.inl"

#endif // XLua