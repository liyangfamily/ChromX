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
#if(0)
    /*SPIDALL*/
    SPIDAll test;
    test.TD_PID_P_Parma = 1;
    test.TD_PID_I_Parma = 1;
    test.TD_PID_D_Parma = 1;
    test.TI_PID_P_Parma = 2;
    test.TI_PID_I_Parma = 2;
    test.TI_PID_D_Parma = 2;
    test.COLUMN_PID_P_Parma = 3;
    test.COLUMN_PID_I_Parma = 3;
    test.COLUMN_PID_D_Parma = 3;

    //quint16 ret = gChromXTestParamSet.writePIDAll(test);
    quint16 ret = gChromXTestParamSet.readPIDAll();
#endif
#if(1)
    /*RunParamSet*/
    SRunParamSet test;
    test.COLUMNFanCloseTemperature = 0x22CC;
    test.testData_AutoRepo = 1;
    //quint16 ret = gChromXTestParamSet.writeRunParam(test);
    quint16 ret = gChromXTestParamSet.readRunParam();
#endif


//    gChromXMainCtrl.readHardwareVersion();
//    gChromXMainCtrl.readARMSoftwareVersion();
//    gChromXMainCtrl.readStartSelfTest();

//    gChromXSingleStatus.readTDCurTemperature(false);
//    gChromXSingleStatus.readTICurTemperature(false);
//    gChromXSingleStatus.readCOLUMNTemperature(false);
//    gChromXSingleStatus.readMicroPIDValue(false);
//    gChromXSingleStatus.readEPCPressure(false);
    ICore::showMessageCCEAPIResult(ret);
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


//***************************************************3、单控控制单元（0x12）按钮事件***************************************************************/
void MainWindow::on_btnWritePumpVoltage_clicked()
{
    quint16 ret = gChromXSingleCtrl.writePumpVoltage(ui->spinBoxPumpVoltage->value());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteSampleValve_clicked()
{
    quint16 ret = gChromXSingleCtrl.writeSampleValve(ui->radioBtnSampleValveON->isChecked());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteDetectValve_clicked()
{
    quint16 ret = gChromXSingleCtrl.writeDetectValve(ui->radioBtnDetectValveON->isChecked());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteFan_clicked()
{
    quint16 ret = gChromXSingleCtrl.writeFan(ui->radioBtnFanON->isChecked());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteTDModule_clicked()
{
    SSingleDeviceCtrl TDModule;
    TDModule.startTime = ui->spinBoxTDStartTime->value();
    TDModule.PWMValue = ui->spinBoxTDPWM->value();
    TDModule.startSwitch = ui->radioBtnTDON->isChecked();
    quint16 ret = gChromXSingleCtrl.writeTDModule(TDModule.startTime, TDModule.PWMValue,
                                                  TDModule.startSwitch);
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteTIModule_clicked()
{
    SSingleDeviceCtrl TIModule;
    TIModule.startTime = ui->spinBoxTIStartTime->value();
    TIModule.PWMValue = ui->spinBoxTIPWM->value();
    TIModule.startSwitch = ui->radioBtnTION->isChecked();
    quint16 ret = gChromXSingleCtrl.writeTIModule(TIModule.startTime, TIModule.PWMValue,
                                                  TIModule.startSwitch);
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteCOLUMNModule_clicked()
{
    SSingleDeviceCtrl COLUMNModule;
    COLUMNModule.startTime = ui->spinBoxCOLUMNStartTime->value();
    COLUMNModule.PWMValue = ui->spinBoxCOLUMNPWM->value();
    COLUMNModule.startSwitch = ui->radioBtnCOLUMNON->isChecked();
    quint16 ret = gChromXSingleCtrl.writeCOLUMNModule(COLUMNModule.startTime, COLUMNModule.PWMValue,
                                                      COLUMNModule.startSwitch);
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteMicroPIDModule_clicked()
{
    SSingleMicroPIDCtrl microPIDModule;
    microPIDModule.biasVoltage = ui->spinBoxMicroPIDBiasVoltage->value();
    microPIDModule.freq = ui->spinBoxMicroPIDFreq->value();
    microPIDModule.startSwitch = ui->radioBtnMicroPIDON->isChecked();
    quint16 ret = gChromXSingleCtrl.writeMicroPIDModule(microPIDModule.biasVoltage,
                                                        microPIDModule.freq, microPIDModule.startSwitch);
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteEPCVoltage_clicked()
{
    quint16 ret = gChromXSingleCtrl.writeEPCVoltage(ui->spinBoxEPCVoltage->value());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnWriteEPCSwitch_clicked()
{
    quint16 ret = gChromXSingleCtrl.writeEPCSwitch(ui->radioBtnEPCON->isChecked());
    ICore::showMessageCCEAPIResult(ret);
}

void MainWindow::on_btnReadPumpVoltage_clicked()
{
    quint16 ret = gChromXSingleCtrl.readPumpVoltage();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxPumpVoltage_Read->setValue(gChromXSingleCtrl.getPumpVoltage());
}

void MainWindow::on_btnReadSampleValve_clicked()
{
    quint16 ret = gChromXSingleCtrl.readSampleValve();
    ICore::showMessageCCEAPIResult(ret);
    if(gChromXSingleCtrl.getSampleValve() == 1)
        ui->radioBtnSampleValveON_Read->setChecked(true);
    if(gChromXSingleCtrl.getSampleValve() == 0)
        ui->radioBtnSampleValveOFF_Read->setChecked(true);
}

void MainWindow::on_btnReadDetectValve_clicked()
{
    quint16 ret = gChromXSingleCtrl.readDetectValve();
    ICore::showMessageCCEAPIResult(ret);
    if(gChromXSingleCtrl.getDetectValve() == 1)
        ui->radioBtnDetectValveON_Read->setChecked(true);
    if(gChromXSingleCtrl.getDetectValve() == 0)
        ui->radioBtnDetectValveOFF_Read->setChecked(true);
}

void MainWindow::on_btnReadFan_clicked()
{
    quint16 ret = gChromXSingleCtrl.readFan();
    ICore::showMessageCCEAPIResult(ret);
    if(gChromXSingleCtrl.getFan() == 1)
        ui->radioBtnFanON_Read->setChecked(true);
    if(gChromXSingleCtrl.getFan() == 0)
        ui->radioBtnFanOFF_Read->setChecked(true);
}

void MainWindow::on_btnReadTDModule_clicked()
{
    quint16 ret = gChromXSingleCtrl.readTDModule();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTDStartTime_Read->setValue(gChromXSingleCtrl.getTDAllData().startTime);
    ui->spinBoxTDPWM_Read->setValue(gChromXSingleCtrl.getTDAllData().PWMValue);

    if (gChromXSingleCtrl.getTDAllData().startSwitch == 1)
        ui->radioBtnTDON_Read->setChecked(true);
    if (gChromXSingleCtrl.getTDAllData().startSwitch == 0)
        ui->radioBtnTDOFF_Read->setChecked(true);
}

void MainWindow::on_btnReadTIModule_clicked()
{
    quint16 ret = gChromXSingleCtrl.readTIModule();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxTIStartTime_Read->setValue(gChromXSingleCtrl.getTIAllData().startTime);
    ui->spinBoxTIPWM_Read->setValue(gChromXSingleCtrl.getTIAllData().PWMValue);

    if (gChromXSingleCtrl.getTIAllData().startSwitch == 1)
        ui->radioBtnTION_Read->setChecked(true);
    if (gChromXSingleCtrl.getTIAllData().startSwitch == 0)
        ui->radioBtnTIOFF_Read->setChecked(true);
}

void MainWindow::on_btnReadCOLUMNModule_clicked()
{
    quint16 ret = gChromXSingleCtrl.readCOLUMNModule();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxCOLUMNStartTime_Read->setValue(gChromXSingleCtrl.getCOLUMNAllData().startTime);
    ui->spinBoxCOLUMNPWM_Read->setValue(gChromXSingleCtrl.getCOLUMNAllData().PWMValue);

    if (gChromXSingleCtrl.getCOLUMNAllData().startSwitch == 1)
        ui->radioBtnCOLUMNON_Read->setChecked(true);
    if (gChromXSingleCtrl.getCOLUMNAllData().startSwitch == 0)
        ui->radioBtnCOLUMNOFF_Read->setChecked(true);
}

void MainWindow::on_btnReadMicroPIDModule_clicked()
{
    quint16 ret = gChromXSingleCtrl.readMicroPIDModule();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxMicroPIDBiasVoltage_Read->setValue(gChromXSingleCtrl.getMicroPIDAllData().biasVoltage);
    ui->spinBoxMicroPIDFreq_Read->setValue(gChromXSingleCtrl.getMicroPIDAllData().freq);
    if (gChromXSingleCtrl.getMicroPIDAllData().startSwitch == 1)
        ui->radioBtnMicroPIDON_Read->setChecked(true);
    if (gChromXSingleCtrl.getMicroPIDAllData().startSwitch == 0)
        ui->radioBtnMicroPIDOFF_Read->setChecked(true);
}

void MainWindow::on_btnReadEPCVoltage_clicked()
{
    quint16 ret = gChromXSingleCtrl.readEPCVoltage();
    ICore::showMessageCCEAPIResult(ret);
    ui->spinBoxEPCVoltage_Read->setValue(gChromXSingleCtrl.getEPCVoltage());
}

void MainWindow::on_btnReadEPCSwitch_clicked()
{
    quint16 ret = gChromXSingleCtrl.readEPCSwitch();
    ICore::showMessageCCEAPIResult(ret);
    if(gChromXSingleCtrl.getEPCSwitch() == 1)
        ui->radioBtnEPCON_Read->setChecked(true);
    if(gChromXSingleCtrl.getEPCSwitch() == 0)
        ui->radioBtnEPCOFF_Read->setChecked(true);
}

//***************************************************4、单控状态单元（0x13）按钮事件***************************************************************/
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

