/********************************************************************************
** Form generated from reading UI file 'xgamepatcher.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XGAMEPATCHER_H
#define UI_XGAMEPATCHER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XGamePatcherClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XGamePatcherClass)
    {
        if (XGamePatcherClass->objectName().isEmpty())
            XGamePatcherClass->setObjectName(QString::fromUtf8("XGamePatcherClass"));
        XGamePatcherClass->resize(600, 400);
        menuBar = new QMenuBar(XGamePatcherClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        XGamePatcherClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XGamePatcherClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        XGamePatcherClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(XGamePatcherClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        XGamePatcherClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(XGamePatcherClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        XGamePatcherClass->setStatusBar(statusBar);

        retranslateUi(XGamePatcherClass);

        QMetaObject::connectSlotsByName(XGamePatcherClass);
    } // setupUi

    void retranslateUi(QMainWindow *XGamePatcherClass)
    {
        XGamePatcherClass->setWindowTitle(QApplication::translate("XGamePatcherClass", "XGamePatcher", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class XGamePatcherClass: public Ui_XGamePatcherClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XGAMEPATCHER_H
