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

template<typename T>
T xMax(T a, T b)
{
	return a > b ? a : b;
}

template<typename T>
T xMin(T a, T b)
{
	return a < b ? a : b;
}

template<typename T>
struct XBufferData
{
public:
	XBufferData() : buffer(NULL), count(0){}
	~XBufferData()
	{
		ASSERT(!buffer);//这里提倡程序释放
		Release();
	}
public:
	void Lock(void** data){data = &buffer;}
	void UnLock();
	bool Allocate(int num)
	{
		Release();
		count = num;
		buffer = new (typename T)[count];
		if (!buffer)
		{
			return false;
			Assert(0);
		}

		return true;
	}
	void Release()
	{
		if (buffer)
		{
			delete []buffer;
			buffer = NULL;
		}
		count = 0;
	}
protected:
	typename T* buffer;
	xint32 count;
};

#endif // XTemplate