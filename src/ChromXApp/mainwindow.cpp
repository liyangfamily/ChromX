#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "app.h"
#include "icore.h"

#include <CCE_Core/CCEUIHelper>
#include <CCE_CommunicatEngine/CCECluster>
//#include <CCE_CommunicatEngine/CCEPackageDispatcher>

#include <CCE_ChromXItem/CCEChromXDevice>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_coreImpl(new ICore(this))
{
    ui->setupUi(this);
    qDebug()<<CCEUIHelper::appName();
    initUI();
}

MainWindow::~MainWindow()
{
    delete m_coreImpl;
    m_coreImpl = nullptr;

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
    if(App::mainCOM.isEmpty()&&App::assistCOM.isEmpty()){
        gChromXDetectServer.autoConnect();
    }
    else{
        gChromXDetectServer.connectMainCOM(App::mainCOM);
        gChromXDetectServer.connectAssistCOM(App::assistCOM);
    }
}

void MainWindow::on_btn_ReadHardwareVer_clicked()
{
//    gChromXMainCtrl.readHardwareVersion();
//    gChromXMainCtrl.readARMSoftwareVersion();
//    gChromXMainCtrl.readStartSelfTest();

    gChromXSingleStatus.readTDCurTemperature(false);
    gChromXSingleStatus.readTICurTemperature(false);
    gChromXSingleStatus.readCOLUMNTemperature(false);
    gChromXSingleStatus.readMicroPIDValue(false);
    gChromXSingleStatus.readEPCPressure(false);
}

void MainWindow::on_btnReadHardVer_clicked()
{
    quint16 ret = gChromXMainCtrl.readHardwareVersion();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxHardVer->setValue(gChromXMainCtrl.getHardwareVersion());
}

void MainWindow::on_btnWriteHardVer_clicked()
{
    quint16 ret = gChromXMainCtrl.writeHardwareVersion(ui->spinBoxHardVer->value());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnReadSoftVer_clicked()
{
    quint16 ret = gChromXMainCtrl.readARMSoftwareVersion();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxSoftVer->setValue(gChromXMainCtrl.getARMSoftwareVersion());
}

void MainWindow::on_btnWriteSoftVer_clicked()
{
    quint16 ret = gChromXMainCtrl.writeARMSoftwareVersion(ui->spinBoxSoftVer->value());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnStartSelfTest_clicked()
{
    quint16 ret = gChromXMainCtrl.writeStartSelfTest(1);
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnStopSelfTest_clicked()
{
    quint16 ret = gChromXMainCtrl.writeStartSelfTest(0);
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnReadLEDHardVer_clicked()
{
    quint16 ret = gChromXLEDPanelUnit.readHardwareVersion();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxLEDHardVer->setValue(gChromXLEDPanelUnit.getHardwareVersion());
}

void MainWindow::on_btnReadLEDPressure_clicked()
{
    quint16 ret = gChromXLEDPanelUnit.readAllPressureSensor();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxLEDCarrierGas->setValue(gChromXLEDPanelUnit.getCarrierGasPressure());
    ui->spinBoxLEDSamplingPump->setValue(gChromXLEDPanelUnit.getSamplingPumpPressure());
    ui->spinBoxLEDAuxGas->setValue(gChromXLEDPanelUnit.getAuxGasPressure());
}

void MainWindow::on_btnWriteLEDLight_clicked()
{
    SLEDPanelLight light;
    light.progress = ui->progressBarLEDProgress->value()/2;
    light.light1 = ui->radioBtnLEDLight1ON->isChecked();
    light.light2 = ui->radioBtnLEDLight2ON->isChecked();
    light.light3 = ui->radioBtnLEDLight3ON->isChecked();
    light.light4 = ui->radioBtnLEDLight4ON->isChecked();
    light.light5 = ui->radioBtnLEDLight5ON->isChecked();
    quint16 ret = gChromXLEDPanelUnit.writeAllLight(light);
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnReadEnvironment_clicked()
{
    quint16 ret = gChromXLEDPanelUnit.readAllEvnSensor();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxEnvTemperature->setValue(gChromXLEDPanelUnit.getEnvTemperature());
    ui->spinBoxEnvHumidity->setValue(gChromXLEDPanelUnit.getEnvHumidity());
    ui->spinBoxEnvPressure->setValue(gChromXLEDPanelUnit.getEnvPressure());
}

void MainWindow::on_btnReadTDTemperature_clicked()
{
    quint16 ret = gChromXSingleStatus.readTDCurTemperature();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTDTemperature->setValue(gChromXSingleStatus.getTDCurTemperature());
}

void MainWindow::on_btnReadTITemperature_clicked()
{
    quint16 ret = gChromXSingleStatus.readTICurTemperature();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTITemperature->setValue(gChromXSingleStatus.getTICurTemperature());
}

void MainWindow::on_btnReadCOLUMNTemperature_clicked()
{
    quint16 ret = gChromXSingleStatus.readCOLUMNTemperature();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxCOLUMNTemperature->setValue(gChromXSingleStatus.getCOLUMNTemperature());
}

void MainWindow::on_btnReadMicroPIDValue_clicked()
{
    quint16 ret = gChromXSingleStatus.readMicroPIDValue();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxMicroPIDValue->setValue(gChromXSingleStatus.getMicroPIDValue());
}

void MainWindow::on_btnReadEPCPressure_clicked()
{
    quint16 ret = gChromXSingleStatus.readEPCPressure();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxEPCPressure->setValue(gChromXSingleStatus.getEPCPressure());
}

void MainWindow::on_btnReadTestDataTDTemperature_clicked()
{
    quint16 ret = gChromXTestData.readTDCurTemperature();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTestDataTDTemperature->setValue(gChromXTestData.getTDCurTemperature());
}

void MainWindow::on_btnReadTestDataTITemperature_clicked()
{
    quint16 ret = gChromXTestData.readTICurTemperature();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTestDataTITemperature->setValue(gChromXTestData.getTICurTemperature());
}

void MainWindow::on_btnReadTestDataCOLUMNTemperature_clicked()
{
    quint16 ret = gChromXTestData.readCOLUMNTemperature();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTestDataCOLUMNTemperature->setValue(gChromXTestData.getCOLUMNTemperature());
}

void MainWindow::on_btnReadTestDataMicroPIDValue_clicked()
{
    quint16 ret = gChromXTestData.readMicroPIDValue();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTestDataMicroPIDValue->setValue(gChromXTestData.getMicroPIDValue());
}

void MainWindow::on_btnReadWarnExistAbnormal_clicked()
{
    quint16 ret = gChromXStatusWarn.readExistAbnormal();
    ICore::showMessageCCEAPIResult(ret);
    if(gChromXStatusWarn.getExistAbnormal()){
        ui->radioBtnWarnExistAbnormalYes->setChecked(true);
    }
    else{
        ui->radioBtnWarnExistAbnormalNo->setChecked(true);
    }
}

void MainWindow::on_btnReadWarnDeviceStatus_clicked()
{
    quint16 ret = gChromXStatusWarn.readDeviceStatus();
    ICore::showMessageCCEAPIResult(ret);
    bool bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_CarrierGasRunningOut);
    ui->radioBtnWarnDeviceStatus0->setChecked(bStatus);
    bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_UnableToMaintainCarrierGasPressure);
    ui->radioBtnWarnDeviceStatus1->setChecked(bStatus);
    bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_SamplingPressureAnomaly);
    ui->radioBtnWarnDeviceStatus2->setChecked(bStatus);
    bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_TDFault);
    ui->radioBtnWarnDeviceStatus3->setChecked(bStatus);
    bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_TIFault);
    ui->radioBtnWarnDeviceStatus4->setChecked(bStatus);
    bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_COLUMNFault);
    ui->radioBtnWarnDeviceStatus5->setChecked(bStatus);
    bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_PIDFault);
    ui->radioBtnWarnDeviceStatus6->setChecked(bStatus);
    bStatus = gChromXStatusWarn.testDeviceStatus(CCEStatusWarnDevice::EDeviceStatus::EDS_OvertemperatureAlarm);
    ui->radioBtnWarnDeviceStatus7->setChecked(bStatus);
}

void MainWindow::on_btnReadWarnSelfTestStatus_clicked()
{
    quint16 ret = gChromXStatusWarn.readDeviceSelfTestStatus();
    ICore::showMessageCCEAPIResult(ret);
    bool bStatus = gChromXStatusWarn.getDeviceSelfTestStatus();
    if(bStatus){
        ui->radioBtnWarnSelfTestComplete->setChecked(true);
    }
    else{
        ui->radioBtnWarnSelfTestInProcess->setChecked(true);
    }
}

void MainWindow::on_btnReadWarnMachineTest_clicked()
{
    quint16 ret = gChromXStatusWarn.readDeviceTestComplete();
    ICore::showMessageCCEAPIResult(ret);
    bool bStatus = gChromXStatusWarn.getDeviceTestComplete();
    if(bStatus){
        ui->radioBtnWarnMachineTestComplete->setChecked(true);
    }
    else{
        ui->radioBtnWarnMachineTestInProcess->setChecked(true);
    }
}

void MainWindow::initUI()
{
    connect(ui->spinBoxLEDProgress, QOverload<int>::of(&QSpinBox::valueChanged),this,[=](int i){
            ui->progressBarLEDProgress->setValue(i);
    });
    ui->spinBoxLEDProgress->setValue(0);
    ui->progressBarLEDProgress->setValue(0);
}

