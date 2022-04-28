#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class ICore;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ICore *m_coreImpl = nullptr;

public slots:

    void on_btn_Refresh_clicked();
    void on_btn_Connect_clicked();
    void on_btn_ReadTest_clicked();
    void on_btn_WriteTest_clicked();
    //Main Ctrl
    void on_btnReadHardVer_clicked();
    void on_btnWriteHardVer_clicked();
    void on_btnReadSoftVer_clicked();
    void on_btnWriteSoftVer_clicked();
    void on_btnStartSelfTest_clicked();
    void on_btnStopSelfTest_clicked();

    //LED Panel Unit
    void on_btnReadLEDHardVer_clicked();
    void on_btnReadLEDPressure_clicked();
    void on_btnWriteLEDLight_clicked();
    void on_btnReadEnvironment_clicked();

    //Single Ctrl
    void on_btnWritePumpVoltage_clicked();
    void on_btnWriteSampleValve_clicked();
    void on_btnWriteDetectValve_clicked();
    void on_btnWriteFan_clicked();
    void on_btnWriteTDModule_clicked();
    void on_btnWriteTIModule_clicked();
    void on_btnWriteCOLUMNModule_clicked();
    void on_btnWriteMicroPIDModule_clicked();
    void on_btnWriteEPCVoltage_clicked();
    void on_btnWriteEPCSwitch_clicked();
    void on_btnReadPumpVoltage_clicked();
    void on_btnReadSampleValve_clicked();
    void on_btnReadDetectValve_clicked();
    void on_btnReadFan_clicked();
    void on_btnReadTDModule_clicked();
    void on_btnReadTIModule_clicked();
    void on_btnReadCOLUMNModule_clicked();
    void on_btnReadMicroPIDModule_clicked();
    void on_btnReadEPCVoltage_clicked();
    void on_btnReadEPCSwitch_clicked();

    //Single Status
    void on_btnReadTDTemperature_clicked();
    void on_btnReadTITemperature_clicked();
    void on_btnReadCOLUMNTemperature_clicked();
    void on_btnReadMicroPIDValue_clicked();
    void on_btnReadEPCPressure_clicked();

    //Test Data
    void on_btnReadTestDataTDTemperature_clicked();
    void on_btnReadTestDataTITemperature_clicked();
    void on_btnReadTestDataCOLUMNTemperature_clicked();
    void on_btnReadTestDataMicroPIDValue_clicked();

    //Status Warn
    void on_btnReadWarnExistAbnormal_clicked();
    void on_btnReadWarnDeviceStatus_clicked();
    void on_btnReadWarnSelfTestStatus_clicked();
    void on_btnReadWarnMachineTest_clicked();
private:
    void initUI();
};
#endif // MAINWINDOW_H
