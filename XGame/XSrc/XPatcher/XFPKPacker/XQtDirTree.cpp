/*******************************************************************************
	FILE:		XQtDirTree.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/12/26

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XQtDirTree.h"
#include "qt/QHeaderView.h"
#include "Qt/qstandarditemmodel.h"
#include "qt/qdir.h"

XQtDirTree::XQtDirTree()
{
	tree_ctrl = NULL;
	dir_model = NULL;
	scan_path = "";
}

bool XQtDirTree::Init(QTreeView* tc)
{
	Release();

	tree_ctrl = tc;
	if (!tree_ctrl)
	{
		return false;
	}
	dir_model = new QDirModel;
	if (!dir_model)
	{
		return false;
	}
	dir_model->setReadOnly(true);
	dir_model->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

	tree_ctrl->setModel(dir_model);
	tree_ctrl->header()->setStretchLastSection(true);
	tree_ctrl->header()->setSortIndicator(0, Qt::AscendingOrder);
	tree_ctrl->header()->setSortIndicatorShown(true);
	tree_ctrl->header()->setClickable(true);

	return true;
}

void XQtDirTree::Release()
{
	tree_ctrl = NULL;
	if (dir_model)
	{
		delete dir_model;
		dir_model = NULL;
	}
}

void XQtDirTree::ScanDir(const char* szPath)
{
	if (!dir_model || !tree_ctrl)
	{
		return;
	}
	tree_ctrl->reset();
	scan_path = szPath;
	QModelIndex index = dir_model->index(scan_path);
	tree_ctrl->setCurrentIndex(index);
	tree_ctrl->scrollTo(index);
	tree_ctrl->setCurrentIndex(index);
	tree_ctrl->resizeColumnToContents(0);
}