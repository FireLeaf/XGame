/********************************************************************************
** Form generated from reading UI file 'xfpkpacker.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XFPKPACKER_H
#define UI_XFPKPACKER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XFPKPackerClass
{
public:
    QWidget *centralWidget;
    QTreeView *treeView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XFPKPackerClass)
    {
        if (XFPKPackerClass->objectName().isEmpty())
            XFPKPackerClass->setObjectName(QString::fromUtf8("XFPKPackerClass"));
        XFPKPackerClass->resize(600, 400);
        centralWidget = new QWidget(XFPKPackerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(100, 60, 256, 192));
        treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView->header()->setHighlightSections(false);
        XFPKPackerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(XFPKPackerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        XFPKPackerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XFPKPackerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        XFPKPackerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(XFPKPackerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        XFPKPackerClass->setStatusBar(statusBar);

        retranslateUi(XFPKPackerClass);

        QMetaObject::connectSlotsByName(XFPKPackerClass);
    } // setupUi

    void retranslateUi(QMainWindow *XFPKPackerClass)
    {
        XFPKPackerClass->setWindowTitle(QApplication::translate("XFPKPackerClass", "XFPKPacker", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class XFPKPackerClass: public Ui_XFPKPackerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XFPKPACKER_H
