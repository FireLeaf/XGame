#include "xgamepatcher.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	XGamePatcher w;
	w.show();
	return a.exec();
}
