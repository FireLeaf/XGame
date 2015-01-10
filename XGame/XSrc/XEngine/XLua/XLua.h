/****************************************************
 *		FileName: XLua.h
 *		FileDesc: C++调用Lua
 *		FileAuthor:
 *		CreateTime: 2014/11/03 22:09:40
 *		CopyRight:
 ***************************************************/

#ifndef __XLUA_H__
#define __XLUA_H__

#pragma warning( push )
#pragma warning( disable : 4996 )

// 包含 Lua 头文件
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
	// 创建 Lua 运行环境
	int CreateLuaRuntime();
private:
	// Lua 栈
	lua_State* m_pLuaState;
};

#pragma warning( pop )

#endif // XLua