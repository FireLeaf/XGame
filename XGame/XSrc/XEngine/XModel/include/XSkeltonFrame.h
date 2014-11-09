/**************************************************************************
/*		FileName: XSkeltonFrame.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 2:08:03
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XSKELTONFRAME__H
#define __XSKELTONFRAME__H

#include <string>
#include "XType.h"
class XMatrix;

struct XBone
{
	std::string name;
// 	std::string parent_name;
// 	std::string first_child_name;
// 	std::string next_sbling_name;
	int parent_index;
	int first_child_index;
	int next_sbling;
	int cur_index;
//	XMatrix init_matrix;
	XMatrix inv_offset_matrix;
	XMatrix tran_matrix;//¹Ç÷À¶¯»­²åÖµ³öÀ´µÄ¾ØÕó * ¸¸¹Ç÷ÀµÄ¾ø¶Ô¾ØÕó
	XMatrix abs_matrix;//inv_offset_matrix * tran_matrix
	XBone():parent_index(-1), first_child_index(-1), next_sbling(-1), cur_index(-1){}
};

class XSkeltonFrame
{
public:
	void Tick(xuint32 time_delta);
	void UpdateSkelton(XBone* ptr_skelton, XBone* ptr_parent_skelton);
	void GetBlendMatrix(XMatrix* blend_matrices, int max_matrix);
public:
	bool Load(const char* skelton_file);
public:
	int GetBoneCount(){return (int)(bone_ptr_array.size());}
	XBone* GetBone(int index){if(index < 0 || index >= (int)bone_ptr_array.size()) return NULL; return bone_ptr_array[index];}
	XBone* FindBone(const char* name);

	void ResetSkelton();
protected:
	std::vector<XBone*> bone_ptr_array;
};

#endif // XSkeltonFrame