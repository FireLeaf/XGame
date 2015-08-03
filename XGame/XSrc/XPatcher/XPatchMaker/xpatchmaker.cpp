#include "xpatchmaker.h"
#include "XMaker.h"
#include "Qt/qstandarditemmodel.h"
XPatchMaker::XPatchMaker(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	if (ui.listViewLog) ui.listViewLog->setModel(new QStandardItemModel(this));
	if (ui.listViewUploadEvent) ui.listViewUploadEvent->setModel(new QStandardItemModel(this));
	connect(ui.comboChannel, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(OnComboSelect(const QString &)));  
	connect(ui.pushButtonMd5, SIGNAL(clicked()), this, SLOT(OnBtnMD5()));
	connect(ui.pushButtonGenPatch, SIGNAL(clicked()), this, SLOT(OnBtnGenPatch()));
	XMaker::GetInstance().Init(ui.listViewLog, ui.listViewUploadEvent);
	const std::vector<XMaker::Channel>& chanells = XMaker::GetInstance().GetChannels();
	if ( ui.comboChannel )
	{
		for (int i = 0; i < chanells.size() ; i++)
		{
			ui.comboChannel->addItem(chanells[i].channel_name);
		}
	}
}

void XPatchMaker::OnComboSelect(const QString &text)
{
	int iCurSelect = ui.comboChannel->currentIndex();
	if(!XMaker::GetInstance().SelectChannel(iCurSelect))
	{
		return;
	}
	const std::vector<XMaker::Channel>& chanells = XMaker::GetInstance().GetChannels();
	if (iCurSelect >= 0 && iCurSelect < chanells.size())
	{
		ui.labelLocalDir->setText(chanells[iCurSelect].local_asset_dir);
		ui.labelUploadUrl->setText(chanells[iCurSelect].upload_url);
		ui.labelAssetVer->setText(QString(XMaker::GetInstance().GetVersion()));
	}
}

void XPatchMaker::OnBtnMD5()
{
	XMaker::GetInstance().GenerateLocalMD5();
	XMaker::GetInstance().Save();
}

void XPatchMaker::OnBtnGenPatch()
{
	XMaker::GetInstance().GenerateUploadFile();
}

XPatchMaker::~XPatchMaker()
{

}
