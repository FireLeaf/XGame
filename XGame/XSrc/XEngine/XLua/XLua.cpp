/****************************************************
 *		FileName: XLua.cpp
 *		FileDesc: 
 *		FileAuthor:
 *		CreateTime: 2014/11/03 22:12:44
 *		CopyRight:
 ***************************************************/

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

#include "XLua.h"

/* Dummy pointer for getting unique keys for Lua's registry. */
static char const executeKey = 0;

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

	// 初始化 Lua 程序库
	luaL_openlibs(m_pLuaState);

	// Put some callback functions in the scripting environment.
	/*static const luaL_Reg callbacks[] = {
		{ "test", cfun_test}
	}
	luaL_register(m_pLuaState, "XGame", callbacks);*/
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

	// 指定参数的个数, 返回结果的个数(多余的丢弃，少的用nil补足), 错误处理函数(要在被调用函数和其参数入栈之前入栈)
	if(lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0) != 0)	// 错误信息入栈（先将函数和参数从栈中移除）
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

	// 编译了Lua代码，如果没有错误，则返回0，同时将编译后的程序块压入虚拟栈中。
	if(luaL_loadbuffer(m_pLuaState, pCommand, strlen(pCommand), NULL) != 0)
	{
		HandleError("Lua Error - Script Load. Script Name:[%s] Error Message:[%s]", pCommand, GetErrorString());

		return false;
	}

	// 将程序块从栈中弹出，并在保护模式下运行该程序块。执行成功返回0，否则将错误信息压入栈中。
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
		// 错误信息会入栈顶 (-1) 检测一个值是否为 string 并返回
		return luaL_checkstring(m_pLuaState, -1);
	}
	else
		return NULL;
}

bool CXLua::AddFunction(const char* pFunctionName, lua_CFunction pFunction)
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

// 期望得到extra数量的空闲槽位
int CXLua::CheckStack( int extra )
{
	if(m_pLuaState)
		return lua_checkstack(m_pLuaState, extra);
	return -1;
}

void CXLua::StackDump()
{
	// 返回栈中元素的个数
	// 索引: 第一个压入栈的为1，第二个为2; -1表示为栈顶元素，-2为栈顶下面的元素
	if(m_pLuaState)
	{
		int iType = 0;
		char szBuf[256] = {0};

		int top = lua_gettop(m_pLuaState);
		for(int i = 1; i <= top; ++i)
		{
			iType = lua_type(m_pLuaState, i);
			switch(iType)
			{
			case LUA_TSTRING:
				{
					sprintf(szBuf, "%d - [%s]\n", i, lua_tostring(m_pLuaState, i));
					OutputDebug(szBuf);
				}
				break;
			case LUA_TBOOLEAN:
				{
					sprintf(szBuf, "%d - [%s]\n", i, lua_toboolean(m_pLuaState, i)?"true":"false");
					OutputDebug(szBuf);
				}
				break;
			case LUA_TNUMBER:
				{
					sprintf(szBuf, "%d - [%g]\n", i, lua_tonumber(m_pLuaState, i));
					OutputDebug(szBuf);
				}
				break;
			default:
				{
					sprintf(szBuf, "%d - [%s]\n", i, lua_typename(m_pLuaState,iType));
					OutputDebug(szBuf);
				}
				break;
			}
		}
	}
}

/**
 * Loads the "package" package into the Lua environment.
 * This action is inherently unsafe, as Lua scripts will now be able to
 * load C libraries on their own, hence granting them the same privileges
 * as the binary itsef.
 */
void CXLua::LoadPackage()
{
	lua_State* L = m_pLuaState;
	lua_pushcfunction(L, luaopen_package);
	lua_pushstring(L, "package");
	lua_call(L, 1, 0);
}

/**
 * Runs a script on a stack containing @a nArgs arguments.
 * @return true if the script was successful and @a nRets return values are available.
 */
bool CXLua::Execute( const char* prog, int iArgs, int iRets )
{
	lua_State* L = m_pLuaState;

	// Compile script into a variadic function.
	int iRes = luaL_loadstring(L, prog);
	if(iRes)
	{
		const char* m = lua_tostring(L, -1);
		//OutputDebug(m);
		lua_pop(L, 1);
		return false;
	}

	// Place the function before its arguments.
	if(iArgs)
		lua_insert(L, -1 - iArgs);

	return LuaWPcall(iArgs, iRets);
}

/**
 * Calls a Lua function stored below its @a nArgs arguments at the top of the stack.
 * @param nRets LUA_MULTRET for unbounded return values.
 * @return true if the call was successful and @a nRets return values are available.
 */
bool CXLua::LuaWPcall( int iArgs, int iRets)
{
	if(!m_pLuaState)
		return false;

	// Load the error handler before the function and its arguments.
	lua_pushlightuserdata(m_pLuaState, static_cast<void *>(const_cast<char *>(&executeKey)));

	lua_rawget(m_pLuaState, LUA_REGISTRYINDEX);
	lua_insert(m_pLuaState, -2 - iArgs);

	int iErrorHandlerIndex = lua_gettop(m_pLuaState) - iArgs - 1;

	// Call the function.
	int iRes = lua_pcall(m_pLuaState, iArgs, iRets, -2 - iArgs);
	XluaJailbreakException::Rethow();

	if(iRes)
	{
		/*
		 * When an exception is thrown which doesn't derive from
		 * std::exception m will be NULL pointer.
		 */
		const char* pCh = lua_tostring(m_pLuaState, -1);
		if(pCh)
		{
			OutputDebug("Lua error: %s", pCh);
		}
		else
		{
			OutputDebug("Lua caught unknown exception.");
		}

		lua_pop(m_pLuaState, 2);
		return false;
	}

	// Remove the error handler.
	lua_remove(m_pLuaState, iErrorHandlerIndex);

	return true;
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
