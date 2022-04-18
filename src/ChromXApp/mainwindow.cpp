#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

#include <CCE_Core/CCEUIHelper>
#include <CCE_CommunicatEngine/CCECluster>
//#include <CCE_CommunicatEngine/CCEPackageDispatcher>

#include <CCE_ChromXItem/CCEChromXDevice>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<CCEUIHelper::appName();

}

MainWindow::~MainWindow()
{
    delete ui;
    //通信库准备退出
    gCluster->readyToExit();
    int maxWait = 0;
    while (!gCluster->canExit())
    {
        QCoreApplication::processEvents();
        QThread::currentThread()->msleep(200);
        maxWait++;
        if (maxWait >= 150)
        {
            if (QMessageBox::information(0, tr("Confire Exit"),
                                         tr("There are still some clients alive in the server. continue waiting?"),
                                         QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                maxWait = 0;
            else
                break;
        }
    }
}

void MainWindow::on_btn_Refresh_clicked()
{
    qDebug()<<gChromXDetectServer.isConnect();
}

void MainWindow::on_btn_Connect_clicked()
{
    gChromXDetectServer.autoConnect();
}

void MainWindow::on_btn_ReadHardwareVer_clicked()
{
    gChromXMainCtrl.readHardwareVersion(true,1000);
}

