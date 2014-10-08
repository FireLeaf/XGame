/**************************************************************************
/*		FileName: XSkeltonFrame.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 2:08:03
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XSKELTONFRAME__H
#define __XSKELTONFRAME__H

struct XSkelton
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
	XSkelton():parent_index(-1), first_child_index(-1), next_sbling(-1), cur_index(-1){}
};

class XSkeltonFrame
{
public:
	void Tick(xuint32 time_delta)
	{
		
	}

	void UpdateSkelton(XSkelton* ptr_skelton, XSkelton* ptr_parent_skelton)
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
			UpdateSkelton(skeltons[ptr_skelton->next_sbling], ptr_parent_skelton);
		//更新孩子
		if(-1 != ptr_skelton->first_child_index)
			UpdateSkelton(skeltons[ptr_skelton->first_child_index], ptr_skelton);
	}
protected:
	std::vector<XSkelton*> skeltons;
};

#endif // XSkeltonFrame