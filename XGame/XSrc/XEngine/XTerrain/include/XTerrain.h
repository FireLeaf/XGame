/**************************************************************************
/*		FileName: XTerrain.h
/*		FileDesc: 无限型的地图
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/08 22:42:46
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XTERRAIN__H
#define __XTERRAIN__H

/*******

*******/
struct XAnchorPos
{
	int x, y, z;//暂不用z
};

struct XTerrainVertex 
{
	XVector3 pos;
	XVector3 normal;
	XVector3 tex;//此纹理坐标是
};

struct XChunk//一个小区块
{
	XAnchorPos
};

struct XChunkArea//共享的顶点数据 的几个区块
{

};

struct XTerrainArea//一块地图区域
{
	
};

class XTerrain
{
public:
	typedef stdext::hash_map<XAnchorPos, XTerrainArea*> PosToTerrAreaMap;
protected:
	PosToTerrAreaMap map_pos_to_terrarea;
	int area_count;//子区域的数量
};

#endif // XTerrain