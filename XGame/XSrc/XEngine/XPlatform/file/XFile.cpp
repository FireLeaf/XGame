/**************************************************************************
/*  FileName 	: XFile.cpp								 
/*	FileDesc 	: 													 
/*	FileAuthor	: YangCao											 
/*	FileTime	: 2014/5/8 0:04	
/*  CopyRight	: 				 
**************************************************************************/

#include "XFile.h"
bool XFile::OpenFile(const xchar* szFileName, const xchar* szOpenMode)
{
	CloseFile();
	m_fp = fopen(szFileName, szOpenMode);
	if (!m_fp)
	{
		return false;
	}

	return true;
}