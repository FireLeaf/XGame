/**************************************************************************
/*		FileName: XAssetMonitor.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 20/08/09
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XASSETMONITOR__H
#define __XASSETMONITOR__H

#include "XMacro.h"
#include "XType.h"
#include "XAsset.h"
#include <hash_map>

class XAssetMonitor
{
public:
	typedef XStlext::hash_map<xint32, XAsset*> MapAsset;
	//typedef XStl::vector<XAsset> ;
public:
	XAssetMonitor() : m_iCurDataStamp(0){}
public:
	xint32 GetAssetDataStamp(){return ++m_iCurDataStamp;}
	XAsset* GetAsset(xint32 id)
	{
		MapAsset::iterator iter = m_mapAsset.find(id);
		if (iter != m_mapAsset.end())
		{
			return iter->second;
		}
		return NULL;
	}
public:
	virtual void UpdateAsset(XAsset* pAsset) = 0;
	virtual XAsset* CreateAsset(ENUM_ASSET_TYPE asset_type, xbool bDynamic) = 0;
	virtual void ReleaseAsset(XAsset* pAsset) = 0;
protected:
	MapAsset m_mapAsset;
	xint32 m_iCurDataStamp;

};
extern XAssetMonitor* x_ptr_asset_monitor;
#endif // XAssetMonitor