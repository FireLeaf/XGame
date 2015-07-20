#include "xfpkunpacker.h"
#include "qt/QFileDialog.h"
#include "XUnPacker.h"
#include "XFile.h"
#include "qt/qmessagebox.h"

XFPKUnPacker::XFPKUnPacker(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	InitWindow();
}

XFPKUnPacker::~XFPKUnPacker()
{
	Release();
}

bool XFPKUnPacker::InitWindow()
{
	connect(ui.BtnOpenFPK, SIGNAL(clicked()), this, SLOT(OnBtnChooseFPK()));
	connect(ui.BtnUnPack, SIGNAL(clicked()), this, SLOT(OnBtnUnPack()));

	return true;
}

void XFPKUnPacker::Release()
{

}

void XFPKUnPacker::OnBtnChooseFPK()
{
	QString fpk = QFileDialog::getOpenFileName(this, tr("Open File Package"), ".", tr("Package Files(*.fpk)"));
	if (fpk.data())
	{
		ui.lineEdit->setText(fpk);
	}
}

void XFPKUnPacker::OnBtnUnPack()
{
	QString fpk = ui.lineEdit->text();
	QString dir = QFileDialog::getExistingDirectory();
	if (dir.data())
	{
		XUnPacker::Get().UnPack(fpk, dir);
	}
	QMessageBox::information(NULL, QString("恭喜"), QString("解包完成"));
}