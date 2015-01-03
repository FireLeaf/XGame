#ifndef XFPKPACKER_H
#define XFPKPACKER_H

#include <QtGui/QMainWindow>
#include "ui_xfpkpacker.h"

class XFPKPacker : public QMainWindow
{
	Q_OBJECT

public:
	XFPKPacker(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XFPKPacker();
protected:
	virtual bool InitWindow();
	virtual void ReleaseWindow();
protected:
	void ScanDir();//É¨ÃèÎÄ¼þ
private slots:
	void BtnOpenAssetDir();
	void BtnGenerateFPK();
private:
	Ui::XFPKPackerClass ui;
};

#endif // XFPKPACKER_H
