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

	return EXIT_SUCCESS;
}

#pragma warning( pop )
