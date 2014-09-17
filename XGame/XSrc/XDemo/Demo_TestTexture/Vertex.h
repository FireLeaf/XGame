#ifndef _VERTEX_H
#define _VERTEX_H
#include <d3dx9.h>
#include <d3d9.h>

struct Vertex 
{
	float x,y,z;
	static const DWORD dwFVF;
	Vertex(float _x,float _y,float _z){
		x=_x;y=_y;z=_z;
	}
};
#endif