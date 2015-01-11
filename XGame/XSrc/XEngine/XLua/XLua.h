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
	// Lua ���л�����ʼ��
	CXLua();
	// �ͷ� Lua_State
	virtual ~CXLua();

public:
	// ִ�а��� Lua �����ַ������ļ�
	bool RunScript(const char* pFileName);
	bool RunString(const char* pCommand);

	bool AddFunction(const char* pFunctionName, LuaFunction pFunction);
	const char* GetErrorString();

	// ��ȡ Lua ���ݸ� LuaGlue �����Ĳ���, ����λ�ô�1��ʼ
	const char* GetStringArgument(int num, const char* pDefault = NULL);
	double GetNumberArgument(int num, double dDefault = 0.0);

	// C++ �� Lua ջ���뷵��ֵ, LuaGlue ��������ֵ��������ջ����һ��, ֧�ֶ������ֵ
	void PushString(const char* pString);
	void PushNumber(double value);
private:
	// Lua ջ
	lua_State* m_pLuaState;
};

#endif // XLua