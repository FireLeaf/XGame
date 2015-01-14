/****************************************************
 *		FileName: XLua.cpp
 *		FileDesc: 
 *		FileAuthor:
 *		CreateTime: 2014/11/03 22:12:44
 *		CopyRight:
 ***************************************************/

#pragma warning( push )
#pragma warning( disable : 4996 )

#include "XLua.h"

CXLua::CXLua()
{
	
}

CXLua::~CXLua()
{

}

int CXLua::CreateLuaRuntime()
{
	m_pLuaState = luaL_newstate();
	if(m_pLuaState == NULL)
	{
		OutputDebug("cannot create state: not enough memory");
		return EXIT_FAILURE;
	}

	// ³õÊ¼»¯ Lua ³ÌÐò¿â
	luaopen_base(m_pLuaState);
	luaopen_package(m_pLuaState);
	luaopen_coroutine(m_pLuaState);
	luaopen_table(m_pLuaState);
	luaopen_io(m_pLuaState);
	luaopen_os(m_pLuaState);
	luaopen_string(m_pLuaState);
	luaopen_bit32(m_pLuaState);
	luaopen_math(m_pLuaState);
	luaopen_debug(m_pLuaState);

	return EXIT_SUCCESS;
}

#pragma warning( pop )
