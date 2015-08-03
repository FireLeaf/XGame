#include "xpatchmaker.h"
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	XPatchMaker w;
	w.show();
	return a.exec();
}
