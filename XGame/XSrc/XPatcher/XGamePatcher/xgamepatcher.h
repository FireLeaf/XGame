#ifndef XGAMEPATCHER_H
#define XGAMEPATCHER_H

#include <QtGui/QMainWindow>
#include "ui_xgamepatcher.h"

class XGamePatcher : public QMainWindow
{
	Q_OBJECT

public:
	XGamePatcher(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XGamePatcher();

private:
	Ui::XGamePatcherClass ui;
};

#endif // XGAMEPATCHER_H
