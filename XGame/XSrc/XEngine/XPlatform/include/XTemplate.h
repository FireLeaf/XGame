/**************************************************************************
/*		FileName: XTemplate.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22:21:59
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XTEMPLATE__H
#define __XTEMPLATE__H

template<typename T>
class Singleton
{
	static T& GetInstance()
	{
		static T t;
		return t;
	}
};

#endif // XTemplate