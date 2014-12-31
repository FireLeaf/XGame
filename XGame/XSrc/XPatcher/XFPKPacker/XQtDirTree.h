/*******************************************************************************
	FILE:		XQtDirTree.h
	
	DESCRIPTTION:	É¨ÃèÎÄ¼þÄ¿Â¼
	
	CREATED BY: YangCao, 2014/12/26

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XQTDIRTREE_H_
#define __COCONAT_XQTDIRTREE_H_

class XQtDirTree : public QTreeView
{
public:
	XQtDirTree(QWidget *parent = 0);
public:
	void ScanDir(const char* szPath);
};

#endif