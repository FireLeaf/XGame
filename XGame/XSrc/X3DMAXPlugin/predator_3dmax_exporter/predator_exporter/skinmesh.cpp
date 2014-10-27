/************************************************************************
    file_name 	: skinmesh.cpp								 
	file_desc 	:														 
	file_author	: YangCao											 
	file_time	: 2013/12/16 22:53					 
**************************************************************************/

#include "skinmesh.h"

int CSkeleton::FindBoneIndex(INode* pNode)
{
	for (int i = 0; i < m_vecBones.size(); i++)
	{
		if (0 == strcmp(pNode->GetName() , m_vecBones[i]))
		{
			return i;
		}
	}
	return -1;
}

void CSkeleton::SaveBone(FILE* fp, INode* pNode, bool bRoot)
{
	if (!fp || !pNode)
	{
		return;
	}

	Bone_t bone;
	bone.NodeInitTM = pNode->GetNodeTM(0);
	memset(&bone, 0, sizeof(Bone_t));
	strncpy(bone.Name, pNode->GetName(), 32);
	if (bRoot)
	{
		
	}else
	{
		INode* pPNode = pNode->GetParentNode();
		strncpy(bone.ParentName, pNode->GetName(), 32);
	}
	
	fwrite(&node, sizeof(node), 1, fp);
	for (int i = 0; i < pNode->NumberOfChildren(); i++)
	{
		SaveBone(fp, pNode->GetChildNode(i), false);
	}

}

void CSkeleton::SaveSkeleton(const char* szFileName, INode* pRootNode)
{
	FILE *fp = fopen(szFileName, "wb");
	if (fp)
	{
		SaveBone(fp, pRootNode, true);
		fclose(fp);
	}
}