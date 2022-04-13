#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <CCE_Core/CCEUIHelper>
#include <CCE_CommunicatEngine/CCEMainCtrlPackage>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<CCEUIHelper::appName();
    CCEMainCtrlPackage_WriteHardwareVersion test(0x20);
    qDebug()<<CCEUIHelper::byteArrayToHexStr(test.build().getDataToSend());
}

MainWindow::~MainWindow()
{
    delete ui;
}

