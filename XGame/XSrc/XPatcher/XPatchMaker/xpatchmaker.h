#ifndef XPATCHMAKER_H
#define XPATCHMAKER_H

#include <QtGui/QMainWindow>
#include "ui_xpatchmaker.h"

class XPatchMaker : public QMainWindow
{
	Q_OBJECT

public:
	XPatchMaker(QWidget *parent = 0);
	~XPatchMaker();
private:
	Ui::XPatchMakerClass ui;
private slots: 
	void OnComboSelect(const QString &text);
	void OnBtnMD5();
	void OnBtnGenPatch();
};

#endif // XPATCHMAKER_H
