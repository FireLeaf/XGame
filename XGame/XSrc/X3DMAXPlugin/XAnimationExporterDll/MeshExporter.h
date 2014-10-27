/**************************************************************************
/*		FileName: MeshExporter.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/05/28 23/29/46
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __MESHEXPORTER__H
#define __MESHEXPORTER__H

#include "MeshExporterUtil.h"

struct PosVertex 
{
	float x, y, z;
};

struct ColorVertex 
{
	float x,y,z;
	unsigned int color;
};


class XMeshExporter
{
public:
	static XMeshExporter& Get()
	{
		static XMeshExporter inst;
		return inst;
	}
public:
	void SetExporterFilePath(const char* file){export_file = file;}
	void ExporterTestModel();
	void ExportSkinMesh(INode* node, std::string& file);//µ¼³öÃÉÆ¤
protected:
	std::string export_file;
};


#endif // MeshExporter