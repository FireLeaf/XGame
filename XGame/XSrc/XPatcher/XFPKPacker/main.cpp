#include "xfpkpacker.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	XFPKPacker w;
	w.show();
	return a.exec();
}
