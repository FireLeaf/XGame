/********************************************************************************
** Form generated from reading UI file 'xfpkunpacker.ui'
**
** Created: Tue Jul 21 21:10:13 2015
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XFPKUNPACKER_H
#define UI_XFPKUNPACKER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XFPKUnPackerClass
{
public:
    QWidget *centralWidget;
    QPushButton *BtnOpenFPK;
    QLineEdit *lineEdit;
    QPushButton *BtnUnPack;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XFPKUnPackerClass)
    {
        if (XFPKUnPackerClass->objectName().isEmpty())
            XFPKUnPackerClass->setObjectName(QString::fromUtf8("XFPKUnPackerClass"));
        XFPKUnPackerClass->resize(600, 400);
        centralWidget = new QWidget(XFPKUnPackerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        BtnOpenFPK = new QPushButton(centralWidget);
        BtnOpenFPK->setObjectName(QString::fromUtf8("BtnOpenFPK"));
        BtnOpenFPK->setGeometry(QRect(40, 20, 75, 23));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(120, 20, 421, 20));
        BtnUnPack = new QPushButton(centralWidget);
        BtnUnPack->setObjectName(QString::fromUtf8("BtnUnPack"));
        BtnUnPack->setGeometry(QRect(60, 260, 75, 23));
        XFPKUnPackerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(XFPKUnPackerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        XFPKUnPackerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XFPKUnPackerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        XFPKUnPackerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(XFPKUnPackerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        XFPKUnPackerClass->setStatusBar(statusBar);

        retranslateUi(XFPKUnPackerClass);

        QMetaObject::connectSlotsByName(XFPKUnPackerClass);
    } // setupUi

    void retranslateUi(QMainWindow *XFPKUnPackerClass)
    {
        XFPKUnPackerClass->setWindowTitle(QApplication::translate("XFPKUnPackerClass", "XFPKUnPacker", 0, QApplication::UnicodeUTF8));
        BtnOpenFPK->setText(QApplication::translate("XFPKUnPackerClass", "\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        BtnUnPack->setText(QApplication::translate("XFPKUnPackerClass", "\350\247\243\345\216\213", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class XFPKUnPackerClass: public Ui_XFPKUnPackerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XFPKUNPACKER_H
