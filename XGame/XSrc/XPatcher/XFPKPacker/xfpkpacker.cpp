#include "xfpkpacker.h"
#include <QtGui/QFileDialog.h>
#include "XPacker.h"

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

	}
}

void XFPKPacker::BtnGenerateFPK()
{
	if(XPacker::Get().Init("F:\\Game\\ɽկdota\\AssetBundle\\", "F:\\Game\\ɽկdota\\first.fpk"))
	{
		XPacker::Get().AddFile("hello.txt");
		XPacker::Get().Fllush();
	}
	
}