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

private:
	Ui::XFPKPackerClass ui;
};

#endif // XFPKPACKER_H
