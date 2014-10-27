/*************************************************************************/
/*  file_name 	: scene_enum.cpp										 */
/*	file_desc 	:														 */
/*	file_author	: YangCao												 */
/*	file_time	: 2013/11/25 22:12										 */
/*************************************************************************/
#include "scene_enum.h"

p_c_scene_enum::p_c_scene_enum(const char* file_name)
{
	ptr_file = fopen(file_name, "wb");
	if (!ptr_file)
	{
		assert(0);
		return;
	}
}

p_c_scene_enum::~p_c_scene_enum()
{

}

int p_c_scene_enum::enum_call_back_proc(INode *node)
{
	if (!ptr_file)
	{
		return FALSE;
	}

	TimeValue tv = GetCOREInterface()->GetTime();

	Mtl* pMtl = node->GetMtl();
	pMtl->
	if (node->GetMtl())
	{
	}
}