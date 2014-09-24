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
public:
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
		Assert(!buffer);//这里提倡程序释放
		Release();
	}
public:
	void Lock(void** data){*data = (void*)buffer;}
	void UnLock(){}
	bool Allocate(xuint32 num)
	{
		Release();
		count = num * sizeof(T);
		buffer = new T[count];
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
	xuint32 Count(){return count;}
protected:
	typename T* buffer;
	xuint32 count;
};

#endif // XTemplate