/*******************************************************************************
	FILE:		XPathcherFile.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/16

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XPatcherFile.h"
#include "XFilePackManager.h"
const char* g_pSandBoxDir;

#define PATCH_MAGIC 0x8d6f7a55
#define PATCH_VERSION 1

bool XPathcherFile::CreatePatch(const char* path)
{
	if (!OpenFile(path, "wb"))
	{
		return false;
	}
	QuickWriteValue<int>(PATCH_MAGIC);
	QuickWriteValue<int>(PATCH_VERSION);
	return true;
}

bool XPathcherFile::DelFiles(std::vector<DeleteItem>& vecDelItems)
{
	int iCount = (int)vecDelItems.size();
	QuickWriteValue(iCount);
	for (int i = 0; i < iCount; i++)
	{
		DelFile(vecDelItems[i]);
	}
	return true;
}

bool XPathcherFile::DelFile(const DeleteItem& delItem)
{
	return DelFile(delItem.path.c_str(), delItem.package.c_str());
}

bool XPathcherFile::DelFile(const char* file, const char* package /* = NULL */)
{
	std::string strPath = file;
	std::string strPackage = package ? package : "";
	QuickWriteString(strPath);
	QuickWriteString(strPackage);
}

bool XPathcherFile::AddFile(const char* src_file, const char* file, const char* package /* = NULL */)
{
	XFile fp;
	if (!fp.OpenFile(src_file))
	{
		return;
	}
	int iLength = fp.Length();
	void* buffer = (void*)malloc(iLength);
	fp.SafeRead(buffer, 1, iLength, PATCH_SAFE_SIZE);
	int iOutLen = 0;
	int iCompressType = NONE_COMPRESS;
	if (CompressFileData(&buffer, iLength, iOutLen, iCompressType))
	{
		AddItem addItem;
		addItem.compress_type = iCompressType;
		addItem.path = file;
		addItem.package = package ? package : "";
		addItem.buf_len = iOutLen;
		addItem.org_len = iLength;
		QuickWriteValue(addItem.compress_type);
		QuickWriteString(addItem.path);
		QuickWriteString(addItem.package);
		QuickWriteValue(addItem.buf_len);
		QuickWriteValue(addItem.org_len);
		SafeWrite(buffer, 1, iOutLen, PATCH_SAFE_SIZE);
		delete buffer;
		return true;
	}
	delete buffer;
	return false;
}

bool XPathcherFile::ApplyPatch(XFilePackManage* pFilePackMan)
{
	if (pFilePackMan)
	{
		return false;
	}

	int iMagicNumber = 0;
	int iVersion = 0;
	QuickReadValue<int>(iMagicNumber);
	QuickReadValue<int>(iVersion);

	if (iMagicNumber != PATCH_MAGIC || iVersion != PATCH_VERSION)
	{
		return false;
	}

	std::string strUpdateDir = std::string(g_pSandBoxDir) + "/";
	strUpdateDir += "PatchAsset";

	int iDelItemSize = 0;
	QuickReadValue<int>(iDelItemSize);
	for (int i = 0; i < iDelItemSize; i++)
	{
		DeleteItem item;//= patch_del_items[i];
		QuickReadString(item.path);
		QuickReadString(item.package);
		if (item.package != "")
		{
			XFilePackageEasy* pack = pFilePackMan->FindPack(item.package.c_str());
			if (pack)
			{
				pack->RemoveFile(item.path.c_str());
			}
			else
			{
				//unknow package
			}
		}
		else
		{
			//delete file
// 			std::string filePath = strUpdateDir + item.path;
// 			if (XSys::XIsFileExist(filePath.c_str()))
// 			{
// 				XSys::XDeleteFile(filePath.c_str());
// 			}
		}
	}

	while (true)
	{
		if(feof( GetFileHandle()))
		{
			break;
		}
		AddItem addItem;
		QuickReadValue(addItem.compress_type);
		QuickReadString(addItem.path);
		QuickReadString(addItem.package);
		QuickReadValue(addItem.buf_len);
		QuickReadValue(addItem.org_len);
		if (addItem.org_len <= 0)
		{
			Assert(0);
			continue;
		}
		void* buf = NULL;
		if(!ReadFileContent(addItem, &buf))
		{
			continue;
		}

		if (addItem.package != "")
		{
			XFilePackageEasy* pack = pFilePackMan->FindPack(addItem.package.c_str());
			if (pack)
			{
				pack->AppendFileFromData(addItem.path, buf, addItem.org_len);
			}
			else
			{
				//unknow package
			}
		}
		else
		{
			std::string filePath = strUpdateDir + item.path;
			XFile fp;
			if (fp.OpenFile(filePath.c_str(), "wb"))
			{
				SafeWrite(buf, 1, addItem.org_len, PATCH_SAFE_SIZE);
			}
		}
		delete buf;
	}

	return true;
}

bool XPathcherFile::ReadFileContent(const AddItem& addItem, void** buff)
{
	xulong file_len = Length();
	xulong file_cur = Tell();
	if (addItem.buf_len < 0 || file_cur + addItem.buf_len > file_len)
	{
		return false;
	}

	*buff = (void*)(malloc(addItem->buf_len));
	if (!*buff)
	{
		return false;
	}
	if(addItem->buf_len != SafeRead(*buff,1, addItem->buf_len, PATCH_SAFE_SIZE))
	{
		delete *buff;
		*buff = NULL;
		Assert(0);
		//未能完全读取
		return false;
	}
	switch(addItem->compress_type)
	{
	case NONE_COMPRESS:
		{
			if (len)
			{
				*len = addItem->buf_len;
			}
		}
		break;
	case Z_LIB_COMPRESS:
		{
			void* uncomp_buff = (void*)(malloc(addItem->org_len));
			if (!uncomp_buff)
			{
				Assert(0);
				delete *buff;
				*buff = NULL;
				return false;
			}
			uLongf uncomp_len = addItem->org_len;
			int ret = uncompress((unsigned char*)uncomp_buff, &uncomp_len, (const unsigned char*)*buff, addItem->buf_len);
			switch(ret)
			{
			case Z_OK:
				{
// 					if (len)
// 					{
// 						*len = addItem->org_len;
// 					}
					delete *buff;
					*buff = uncomp_buff;
				}
				break;
			default:
				Assert(0);
				break;
			}
		}
		break;
	case _7Z_COMPRESS:
		break;
	default:
		break;
	}
	return true;
}