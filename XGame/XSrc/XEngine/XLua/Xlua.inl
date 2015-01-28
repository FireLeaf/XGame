/****************************************************
 *		FileName: XLua.inl
 *		FileDesc: XLua 类内联函数
 *		FileAuthor:
 *		CreateTime: 2015/01/19 22:09:52
 *		CopyRight:
 ***************************************************/

inline void CXLua::SetErrorHandler(pFnErrorHandler pErrHandler) { m_pErrorHandler = pErrHandler; }

inline lua_State* CXLua::GetScriptContext() { return m_pLuaState; }

inline bool CXLua::PushNumber(double value)
{
	if(m_pLuaState)
	{
		lua_pushnumber(m_pLuaState, value);
		return true;
	}
	else
		return false;
}

inline bool CXLua::PushMemoryStr(const char* s, size_t len)
{
	if(m_pLuaState)
	{
		lua_pushlstring(m_pLuaState, s, len);
		return true;
	}
	else
		return false;
}

// Lua生成拷贝副本
inline bool CXLua::PushString(const char* pString)
{
	if(m_pLuaState)
	{
		lua_pushstring(m_pLuaState, pString);
		return true;
	}
	else
		return false;
}

inline bool CXLua::PushNil()
{
	if(m_pLuaState)
	{
		lua_pushnil(m_pLuaState);
		return true;
	}
	else
		return false;
}

inline bool CXLua::PushBoolean(bool b)
{
	if(m_pLuaState)
	{
		lua_pushboolean(m_pLuaState, b ? 1 : 0);
		return true;
	}
	else
		return false;
}

inline bool CXLua::PushInteger(int val)
{
	if(m_pLuaState)
	{
		lua_pushinteger(m_pLuaState, (lua_Integer)val);
		return true;
	}
	else
		return false;
}