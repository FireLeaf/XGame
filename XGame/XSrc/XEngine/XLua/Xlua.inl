/****************************************************
 *		FileName: XLua.inl
 *		FileDesc: XLua 类内联函数
 *		FileAuthor:
 *		CreateTime: 2015/01/19 22:09:52
 *		CopyRight:
 ***************************************************/

inline void CXLua::SetErrorHandler(pFnErrorHandler pErrHandler) { m_pErrorHandler = pErrHandler; }

inline lua_State* CXLua::GetScriptContext() { return m_pLuaState; }