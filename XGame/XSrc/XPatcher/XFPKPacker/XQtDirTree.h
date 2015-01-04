/*******************************************************************************
	FILE:		XQtDirTree.h
	
	DESCRIPTTION:	É¨ÃèÎÄ¼þÄ¿Â¼
	
	CREATED BY: YangCao, 2014/12/26

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XQTDIRTREE_H_
#define __COCONAT_XQTDIRTREE_H_

#include "qt/qtreeview.h"
#include "qt/qdirmodel.h"

class XQtDirTree
{
public:
	XQtDirTree();
	bool Init(QTreeView* tc);
	void Release();
public:
	void ScanDir(const char* szPath);
private:
	QTreeView* tree_ctrl;
	QDirModel* dir_model;
	QModelIndex watch_index;
	QString scan_path;
};

#endif