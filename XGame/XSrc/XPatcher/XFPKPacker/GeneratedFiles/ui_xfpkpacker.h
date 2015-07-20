/********************************************************************************
** Form generated from reading UI file 'xfpkpacker.ui'
**
** Created: Mon Jul 20 23:44:25 2015
**      by: Qt User Interface Compiler version 4.7.4
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
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XFPKPackerClass
{
public:
    QWidget *centralWidget;
    QTreeView *TreeAsset;
    QPushButton *ButtonGenerateFPX;
    QLineEdit *EditAssetDir;
    QPushButton *ButtonOpen;
    QLineEdit *EditFPKName;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XFPKPackerClass)
    {
        if (XFPKPackerClass->objectName().isEmpty())
            XFPKPackerClass->setObjectName(QString::fromUtf8("XFPKPackerClass"));
        XFPKPackerClass->resize(600, 432);
        centralWidget = new QWidget(XFPKPackerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        TreeAsset = new QTreeView(centralWidget);
        TreeAsset->setObjectName(QString::fromUtf8("TreeAsset"));
        TreeAsset->setGeometry(QRect(30, 40, 531, 291));
        ButtonGenerateFPX = new QPushButton(centralWidget);
        ButtonGenerateFPX->setObjectName(QString::fromUtf8("ButtonGenerateFPX"));
        ButtonGenerateFPX->setGeometry(QRect(40, 340, 75, 23));
        EditAssetDir = new QLineEdit(centralWidget);
        EditAssetDir->setObjectName(QString::fromUtf8("EditAssetDir"));
        EditAssetDir->setGeometry(QRect(80, 10, 481, 20));
        EditAssetDir->setReadOnly(false);
        ButtonOpen = new QPushButton(centralWidget);
        ButtonOpen->setObjectName(QString::fromUtf8("ButtonOpen"));
        ButtonOpen->setGeometry(QRect(30, 10, 51, 23));
        EditFPKName = new QLineEdit(centralWidget);
        EditFPKName->setObjectName(QString::fromUtf8("EditFPKName"));
        EditFPKName->setGeometry(QRect(130, 340, 231, 20));
        XFPKPackerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(XFPKPackerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        XFPKPackerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XFPKPackerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        XFPKPackerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(XFPKPackerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        XFPKPackerClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());

        retranslateUi(XFPKPackerClass);

        QMetaObject::connectSlotsByName(XFPKPackerClass);
    } // setupUi

    void retranslateUi(QMainWindow *XFPKPackerClass)
    {
        XFPKPackerClass->setWindowTitle(QApplication::translate("XFPKPackerClass", "XFPKPacker", 0, QApplication::UnicodeUTF8));
        ButtonGenerateFPX->setText(QApplication::translate("XFPKPackerClass", "\347\224\237\346\210\220\345\214\205", 0, QApplication::UnicodeUTF8));
        ButtonOpen->setText(QApplication::translate("XFPKPackerClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("XFPKPackerClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class XFPKPackerClass: public Ui_XFPKPackerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XFPKPACKER_H
