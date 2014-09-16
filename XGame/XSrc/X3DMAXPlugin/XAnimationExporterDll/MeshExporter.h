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
	void SetExporterFilePath(const char* file)
	{
		export_file = file;
	}
	void ExporterTestModel()
	{
		Interface* ip = GetCOREInterface();
		if (!ip)
		{
			return;
		}
		FILE* fp = fopen(export_file.c_str(), "wb");
		if (!fp)
		{
			return;
		}
		INode* pNode = ip->GetSelNode(0);
		if (!pNode)
		{
			return;
		}

		Mesh* pMesh = MeshExporterUtil::GetMesh(pNode, 0);
		if (!pMesh)
		{
			return;
		}

		int iVertexNum = pMesh->numVerts;
		fwrite(&iVertexNum, sizeof(int), 1, fp);
		//PosVertex* vertices = new PosVertex[iVertexNum];
		fwrite(pMesh->verts, sizeof(Point3), iVertexNum, fp);

		int iFaceNum = pMesh->numFaces;
		fwrite(&iFaceNum, sizeof(iFaceNum), 1, fp);
		
		unsigned short* indices = new unsigned short[iFaceNum * 3];
		for (int i = 0; i < iFaceNum; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				indices[i * 3 + j] = (unsigned short)pMesh->faces[i].v[j];
			}
		}

		fwrite(indices, sizeof(unsigned short), iFaceNum * 3, fp);
		fclose(fp);
	}
protected:
	std::string export_file;
};


#endif // MeshExporter