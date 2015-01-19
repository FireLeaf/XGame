/****************************************************
 *		FileName: XLua.cpp
 *		FileDesc: 
 *		FileAuthor:
 *		CreateTime: 2014/11/03 22:12:44
 *		CopyRight:
 ***************************************************/

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

#include "XLua.h"

CXLua::CXLua()
{
	m_pErrorHandler = NULL;

	CreateLuaRuntime();
}

CXLua::~CXLua()
{
	if(m_pLuaState)
		lua_close(m_pLuaState);
}

void CXLua::CreateLuaRuntime()
{
	m_pLuaState = luaL_newstate();
	if(m_pLuaState == NULL)
	{
		OutputDebug("cannot create state: not enough memory");
		return;
	}

	// ��ʼ�� Lua �����
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
}

void CXLua::HandleError(char* format, ...)
{
	if(m_pErrorHandler)
	{
		char szBuf[1024] = {0};

		va_list argPtr;

		va_start(argPtr, format);
		vsprintf(szBuf, format, argPtr);
		va_end(argPtr);

		m_pErrorHandler(szBuf);
	}
}

bool CXLua::RunScript(const char* pFileName)
{
	if(!m_pLuaState)
		return false;

	if(luaL_loadfile(m_pLuaState, pFileName) != 0)
	{
		HandleError("Lua Error - Script Load. Script Name:[%s] Error Message:[%s]", pFileName, GetErrorString());

		return false;
	}

	// ָ�������ĸ���, ���ؽ���ĸ���(����Ķ������ٵ���nil����), ��������(Ҫ�ڱ����ú������������ջ֮ǰ��ջ)
	if(lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0) != 0)	// ������Ϣ��ջ���Ƚ������Ͳ�����ջ���Ƴ���
	{
		HandleError("Lua Error - Script Run. Script Name:[%s] Error Message:[%s]", pFileName, GetErrorString());

		return false;
	}

	return true;
}

bool CXLua::RunString(const char* pCommand)
{
	if(!m_pLuaState)
		return false;

	if(luaL_loadbuffer(m_pLuaState, pCommand, strlen(pCommand), NULL) != 0)
	{
		HandleError("Lua Error - Script Load. Script Name:[%s] Error Message:[%s]", pCommand, GetErrorString());

		return false;
	}

	if(lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0) != 0)
	{
		HandleError("Lua Error - Script Run. Script Name:[%s] Error Message:[%s]", pCommand, GetErrorString());

		return false;
	}

	return true;
}

const char* CXLua::GetErrorString()
{
	if(!m_pLuaState)
	{
		// ������Ϣ����ջ�� (-1) ���һ��ֵ�Ƿ�Ϊ string ������
		return luaL_checkstring(m_pLuaState, -1);
	}
	else
		return NULL;
}

bool CXLua::AddFunction(const char* pFunctionName, LuaFunction pFunction)
{
	if(!m_pLuaState)
		return false;

	lua_register(m_pLuaState, pFunctionName, pFunction);

	return true;
}

const char* CXLua::GetStringArgument(int num, const char* pDefault /*= NULL*/)
{
	if(m_pLuaState)
		return luaL_optstring(m_pLuaState, num, pDefault);
	else
		return pDefault;
}

double CXLua::GetNumberArgument(int num, double dDefault /*= 0.0*/)
{
	if(m_pLuaState)
		return luaL_optnumber(m_pLuaState, num, dDefault);
	else
		return dDefault;
}

bool CXLua::PushString(const char* pString)
{
	if(!m_pLuaState)
		return false;

	lua_pushstring(m_pLuaState, pString);

	return true;
}

bool CXLua::PushNumber(double value)
{
	if(!m_pLuaState)
		return false;

	lua_pushnumber(m_pLuaState, value);

	return true;
}

#ifdef WIN32
#pragma warning( pop )
#endif
