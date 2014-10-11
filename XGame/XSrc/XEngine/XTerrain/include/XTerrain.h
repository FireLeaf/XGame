/**************************************************************************
/*		FileName: XTerrain.h
/*		FileDesc: �����͵ĵ�ͼ
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
	int x, y, z;//�ݲ���z
};

struct XTerrainVertex 
{
	XVector3 pos;
	XVector3 normal;
	XVector3 tex;//������������
};

struct XChunk//һ��С����
{
	XAnchorPos
};

struct XChunkArea//����Ķ������� �ļ�������
{

};

struct XTerrainArea//һ���ͼ����
{
	
};

class XTerrain
{
public:
	typedef stdext::hash_map<XAnchorPos, XTerrainArea*> PosToTerrAreaMap;
protected:
	PosToTerrAreaMap map_pos_to_terrarea;
	int area_count;//�����������
};

#endif // XTerrain