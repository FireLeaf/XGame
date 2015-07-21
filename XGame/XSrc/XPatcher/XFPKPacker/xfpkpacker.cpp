#include "xfpkpacker.h"
#include <QtGui/QFileDialog.h>
#include "XPacker.h"
#include "qt/qdir.h"
#include "qt/qfileinfo.h"
#include "qt/qmessagebox.h"

XFPKPacker::XFPKPacker(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	InitWindow();
}

XFPKPacker::~XFPKPacker()
{
	ReleaseWindow();
}

bool XFPKPacker::InitWindow()
{
	connect(ui.ButtonOpen, SIGNAL(clicked()), this, SLOT(BtnOpenAssetDir()));
	connect(ui.ButtonGenerateFPX, SIGNAL(clicked()), this, SLOT(BtnGenerateFPK()));
	if (!dir_tree.Init(ui.TreeAsset))
	{
		return false;
	}
	
	return true;
}

void XFPKPacker::ReleaseWindow()
{

}

void XFPKPacker::BtnOpenAssetDir()
{
	QString dir = QFileDialog::getExistingDirectory();
	if (dir.data())
	{
		ui.EditAssetDir->setText(dir);
		ScanDir();
	}
}

void AddFileInfo(QDir& file_dir, QString parent_path)
{
	QFileInfoList file_list = file_dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	//file_dir.setSorting(QDir::Name | QDir::Reversed);
	for (int file_index = 0; file_index < file_list.size(); file_index++)
	{
		const QFileInfo& file_info = file_list.at(file_index);
		std::string file_path = file_info.fileName().toLocal8Bit();
		if (file_path == "." || file_path == "..")
		{
			continue;
		}
		else if (file_info.isDir())
		{
			QString tmp_parent = parent_path;
			tmp_parent.append("/");
			tmp_parent.append(file_info.fileName());
			QString abs_path = file_dir.absolutePath();
			abs_path.append("/");
			abs_path.append(file_info.fileName());
			QDir dir(abs_path);
			if (!dir.exists())
			{
				continue;
			}
			AddFileInfo(dir, tmp_parent);
		}
		else if (file_info.isFile())
		{
			QString cur_file_path = parent_path;
			cur_file_path.append("/");
			cur_file_path.append(file_info.fileName());
			std::string strCurFile = cur_file_path.toLocal8Bit();
			XPacker::Get().AddFile(strCurFile.c_str());
		}
	}
}

void XFPKPacker::BtnGenerateFPK()
{
	QString asset_path = ui.EditAssetDir->text();
	QString fpk_name = ui.EditFPKName->text();
	fpk_name.append(".fpk");
	QDir dir(asset_path);
	if (!dir.exists())
	{
		Assert(0);
		return;
	}
//	dir.setFilter(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//	dir.setSorting(QDir::Name | QDir::Reversed);
	//QString::toStdString会崩溃
	std::string strAssetPath = asset_path.toLocal8Bit();
	std::string strFpkName = fpk_name.toLocal8Bit();
	if(XPacker::Get().Init(strAssetPath.c_str(), strFpkName.c_str()));
	{
		AddFileInfo(dir, QString(""));
		XPacker::Get().Fllush();
	}
	QMessageBox::information(NULL, QString("恭喜"), QString("打包完成"));
}

void XFPKPacker::ScanDir()
{
	QString dir = ui.EditAssetDir->text();
	std::string str = dir.toLocal8Bit();
	const char* ch = str.c_str();
	dir_tree.ScanDir(ch);
}