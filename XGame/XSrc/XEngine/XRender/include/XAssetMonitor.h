/**************************************************************************
/*		FileName: XAssetMonitor.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 20/08/09
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XASSETMONITOR__H
#define __XASSETMONITOR__H

class XAssetMonitor
{
public:
	typedef XStlext::hash_map<int, XAsset*> MapAsset;
public:
	XAssetMonitor() : m_iCurDataStamp(0){}
public:
	xint32 GetAssetDataStamp(){return ++m_iCurDataStamp;}
public:
	virtual void UpdateAsset(XAsset* pAsset) = 0;
	virtual bool CreateAsset(XAsset* pAsset) = 0;
	virtual void ReleaseAsset(XAsset* pAsset) = 0;
protected:
	MapAsset m_mapAsset;
	xint32 m_iCurDataStamp;
};

#endif // XAssetMonitor