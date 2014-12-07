/**************************************************************************
/*		FileName: XBufferStream.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/12/07 10:51:44
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XBUFFERSTREAM__H
#define __XBUFFERSTREAM__H

class XBufferStream
{

};

template<typename T>
class XSimpleVectorStream
{
	enum
	{
		EACH_INC_STREAM = 128,
		MAX_INC_STREAM = 1024,
	};
public:
	XSimpleVectorStream() : pData(NULL), total_len(0), reserve_len(0), offset(0), alloc_time(0){}
	//XSimpleVectorStream& operator << (const char*);
	void PushStream(const T* t, int len)
	{
		if (total_len + len > reserve_len )
		{
			alloc_time++;
			int alloc_count = alloc_time * EACH_INC_STREAM;
			alloc_count = alloc_count > MAX_INC_STREAM ? MAX_INC_STREAM : alloc_count;
			alloc_count += (total_len + len - reserve_len);
			T* ptr = (t*)malloc(alloc_count);
			if (!ptr)
			{
				Assert(0);
				return;
			}
			if(pData)
				memcpy(ptr, pData, total_len);
			memcpy(ptr + total_len, (const void*)t, len);
			pData = ptr;
			total_len += len;
		}
	}
public:
	T* GetData(){return pData;}
	void ReserveLen(int len){}
	void ReleaseStream(){if(pData) delete pData; pData = NULL; total_len = 0; reserve_len = 0; offset = 0; alloc_time(0)}
protected:
	T* pData;
	int alloc_time;
	int reserve_len;
	int total_len;
	int offset;
};

#endif // XBufferStream