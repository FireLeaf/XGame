/**************************************************************************
/*		FileName: XSkeltonFrame.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 2:08:03
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XSKELTONFRAME__H
#define __XSKELTONFRAME__H

struct XBone
{
	std::string name;
	std::string parent_name;
	std::string first_child_name;
	std::string next_sbling;
	int parent_index;
	int first_child_index;
	int next_sbling;
	int cur_index;
	XMatrix init_matrix;
	XMatrix inv_matrix;
	XMatrix abs_matrix;
	XBone():parent_index(-1), first_child_index(-1), next_sbling(-1), cur_index(-1){}
};

class XSkeltonFrame
{
public:
	void Tick(xuint32 time_delta)
	{
		
	}

	void UpdateSkelton(XBone* ptr_skelton, XBone* ptr_parent_skelton)
	{
		if (!ptr_skelton)
		{
			return;
		}

		if (ptr_parent_skelton)
		{
			ptr_skelton->abs_matrix = ptr_skelton->init_matrix * ptr_parent_skelton->abs_matrix;
		}
		else
		{
			ptr_skelton->abs_matrix = ptr_skelton->init_matrix;
		}

		//更新兄弟
		if(-1 != ptr_skelton->next_sbling)
			UpdateSkelton(bone_ptr_array[ptr_skelton->next_sbling], ptr_parent_skelton);
		//更新孩子
		if(-1 != ptr_skelton->first_child_index)
			UpdateSkelton(bone_ptr_array[ptr_skelton->first_child_index], ptr_skelton);
	}

public:
	int GetBoneCount(){return (int)(bone_ptr_array.size());}
	XBone* GetBone(int index){if(index < 0 || index >= bone_ptr_array.size()) return NULL; return bone_ptr_array[index];}
protected:
	std::vector<XBone*> bone_ptr_array;
};

#endif // XSkeltonFrame