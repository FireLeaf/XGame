/*************************************************************************/
/*  file_name 	: scene_enum.h											 */
/*	file_desc 	:														 */
/*	file_author	: YangCao												 */
/*	file_time	: 2013/11/25 21:58										 */
/*************************************************************************/
#ifndef _H_SCENE_ENUM_H_
#define _H_SCENE_ENUM_H_
#include "max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
class p_c_scene_enum : public ITreeEnumProc
{
protected:
	FILE* ptr_file;
public:
	p_c_scene_enum(const char* file_name);
	~p_c_scene_enum();
protected:
	int enum_call_back_proc(INode *node);
};

#endif