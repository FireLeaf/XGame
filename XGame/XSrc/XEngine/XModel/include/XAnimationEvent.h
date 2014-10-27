/*******************************************************************************
	FILE:		XAnimationEvent.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/10/27

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XANIMATIONEVENT_H_
#define __COCONAT_XANIMATIONEVENT_H_

struct XAnimationEvent 
{
	virtual void TrigEvent(int frame/*����*/) = 0;
};

class XAnimSound : public XAnimationEvent
{
public:
	virtual void TrigEvent(int frame);
protected:
	std::string sound_id;//�п�����·���п�����
};

class XAnimEffect : public XAnimationEvent
{
public:

protected:
	//...
};


#endif