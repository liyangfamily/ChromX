#include "cxatestparamset.h"
#include "ui_cxatestparamset.h"

CXATestParamSet::CXATestParamSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CXATestParamSet)
{
    ui->setupUi(this);
}

CXATestParamSet::~CXATestParamSet()
{
    delete ui;
}
