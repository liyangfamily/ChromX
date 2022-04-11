#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <CCE_Core/CCEUIHelper>
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
}

