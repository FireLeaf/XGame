/**************************************************************************
/*		FileName: XActObject.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/27 21:13:07
/*		CopyRight: yc 
/*************************************************************************/


#ifndef __XACTOBJECT__H
#define __XACTOBJECT__H

class XActObject
{
public:
	XActObject():parent(NULL){}
public:
	const XAABB& GetAABB() const {return aabb;}
	const XVector3& GetPos() 
	{
		if (parent)
		{
			abs_pos = parent->GetPos() + loc_pos;
		}
		return abs_pos;
	}
	const XVector3& GetDir() const {return dir;}

	XActObject* GetParent() {return parent;}

	void SetAABB(const XAABB& aabb){this->aabb = aabb;}
	void SetPos(const XVector3& pos){loc_pos = pos; abs_pos = loc_pos;}
	void SetParent(XActObject* parent){this->parent = parent;}
protected:
	XAABB aabb;
	XVector3 abs_pos;//����λ��
	XVector3 loc_pos;//���λ��
	XMatrix abs_mat;
	XMatrix loc_mat;
	XVector3 dir;//����
	XVector3 scal;//����
	XActObject* parent;
};

#endif // XActObject