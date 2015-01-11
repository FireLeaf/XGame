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
}

class CXLua
{
public:
	CXLua();
	~CXLua();

public:
	// ���� Lua ���л���
	int CreateLuaRuntime();
private:
	// Lua ջ
	lua_State* m_pLuaState;
};

#endif // XLua