/*******************************************************************************
	FILE:		XPathcherFile.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/16

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XPatcherFile.h"
#include "XFilePackManager.h"

bool XPathcherFile::ApplyPatcher()
{
	for (int i = 0; i < patcher_items.size(); i++)
	{
		const PatcherItem& item = patcher_items[i];
		if (item.in_package)
		{
			switch(item.action)
			{
			case PA_ADD_FILE:
				//XFilePackManage::Get().AddFile(item->package.c_str(), item->path.c_str(), )
				break;
			}
		}
		else
		{

		}
	}

	return true;
}