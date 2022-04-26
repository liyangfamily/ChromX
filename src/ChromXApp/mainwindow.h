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
    void on_btn_ReadHardwareVer_clicked();
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
