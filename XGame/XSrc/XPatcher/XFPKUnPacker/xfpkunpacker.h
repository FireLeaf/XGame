#ifndef XFPKUNPACKER_H
#define XFPKUNPACKER_H

#include <QtGui/QMainWindow>
#include "ui_xfpkunpacker.h"

class XFPKUnPacker : public QMainWindow
{
	Q_OBJECT

public:
	XFPKUnPacker(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XFPKUnPacker();
protected:
	bool InitWindow();
	void Release();
private slots:
	void OnBtnChooseFPK();
	void OnBtnUnPack();
private:
	Ui::XFPKUnPackerClass ui;
};

#endif // XFPKUNPACKER_H
