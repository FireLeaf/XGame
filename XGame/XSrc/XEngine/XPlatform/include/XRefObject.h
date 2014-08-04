/*******************************************************************************
	FILE:		XRefObject.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/08/04

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XREFOBJECT_H_
#define __COCONAT_XREFOBJECT_H_

class XRefObject
{
public:
	XRefObject():iRefCount(0){}
	virtual ~XRefObject(){Assert(0 == iRefCount);}
public:
	void AddRef(){iRefCount++;}
	void Release()
	{
		iRefCount--;
		ASSERT()
		if (iRefCount <= 0)
		{
			delete this;
		}
	}
	xint32 GetRef(){return iRefCount;}
private:
	xint32 iRefCount;
};

#endif