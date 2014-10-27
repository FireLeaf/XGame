/*************************************************************************/
/*  file_name 	: predator_skin_mesh.h									 */
/*	file_desc 	:														 */
/*	file_author	: YangCao												 */
/*	file_time	: 2013/11/19 22:33										 */
/*************************************************************************/
#ifndef _H_PREDATOR_SKIN_MESH_H_
#define _H_PREDATOR_SKIN_MESH_H_

#include <string>
#include <vector>
using namespace std;

struct p_i_vertex
{
	IDirect3DVertexDeclaration9 vd;
};

struct p_s_vertexp : public p_i_vertex
{
	D3DXVECTOR3 v_pos;
};

struct p_s_vertexpc : public p_i_vertex
{
	D3DXVECTOR3 v_pos;
	D3DCOLOR	c_diffuse;
};

struct p_s_vertexpt : public p_i_vertex
{

};

struct p_s_skinvertex : public p_i_vertex
{
	D3DXVECTOR3 v_pos;
	D3DXVECTOR3 v_normal;
	D3DXVECTOR2 v_texcoord;
	float weight[4];
	union
	{
		DWORD indices_mat;
		byte bt_indices[4];
	};
	
};

struct p_i_index
{

};

struct p_s_index16 : public p_i_index
{
	WORD index;
};

struct p_i_texture
{

};

struct p_s_texture2d
{
	string str_texture_path;
	IDirect3DTexture9* texture;
};

struct p_i_material
{
	D3DMATERIAL9 material;
};

struct p_s_material2d : public p_i_material
{
	p_s_texture2d* texture;
};

struct p_s_sub_skinmesh
{
	vector<p_s_skinvertex> vertexs;
	vector<p_s_index16> indices;
	p_s_material2d mtrl;
};

struct p_s_skinmesh
{
	vector<p_s_skinmesh*> vec_sub_meshes;
};

struct p_s_keyframe
{
	float time_stamp;//时间点
	D3DXVECTOR3 translation;//偏移
	D3DXVECTOR3 scal;//缩放
	D3DXMATRIX rot;//旋转
	D3DXQUATERNION quat;//四元素
};

struct p_s_kf_set
{
	std::vector<p_s_keyframe> vec_key_frame;
};

struct p_s_action
{
	string str_action_name;
	p_s_kf_set key_set;
};

struct p_s_bone
{
	string str_bone_name;
	p_s_bone* ptr_first_slibing;
	p_s_bone* ptr_parent;
	p_s_bone* ptr_first_child;
	
	D3DXMATRIX mat_loc;
	D3DXMATRIX mat_abs;
};



#endif