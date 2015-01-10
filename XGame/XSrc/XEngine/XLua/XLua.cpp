/****************************************************
 *		FileName: XLua.cpp
 *		FileDesc: 
 *		FileAuthor:
 *		CreateTime: 2014/11/03 22:12:44
 *		CopyRight:
 ***************************************************/

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
		//OutputDebugStr("cannot create state: not enough memory");
		//return EXIT_FAILURE;
		return 1;
	}

	//return EXIT_SUCCESS;
	return 0;
}
