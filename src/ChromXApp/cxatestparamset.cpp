#include "cxatestparamset.h"
#include "ui_cxatestparamset.h"

#include "app.h"
#include "icore.h"

#include <JsonHelper/cxajsontestparamset.h>
#include <TransUiValueConvertor/cxatestparamsetconvertor.h>
#include <Chart/cxachartwidget.h>
#include <CCE_ChromXItem/CCEChromXDevice>

#include <QFileDialog>
#include <QStandardItemModel>
#include <QVBoxLayout>


CXATestParamSet::CXATestParamSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CXATestParamSet),
    m_jsonTestParamSet(new CXAJsonTestParamSet),
    m_chart(new CXAChartWidget(CXAChartWidget::ECM_TestData,this))
{
    ui->setupUi(this);
    initUI();
}

CXATestParamSet::~CXATestParamSet()
{
    delete ui;
}

void CXATestParamSet::on_btn_Run_clicked()
{
    updateJsonFromUI();
    STestParamSet&& testParam = m_jsonTestParamSet->testParam();
    //SPIDAll& pidAll = testParam.PIDAll;
    SRunParamSet& runParam = testParam.runParamSet;
    SPressureMode& pressureMode = testParam.pressureMode;
    runParam.testData_AutoRepo = 1;
    /*SPIDALL*/
   // quint16 ret = gChromXTestParamSet.writePIDAll(pidAll);
    //ICore::showMessageCCEAPIResult(ret);
    //if(ret == CCEAPI::EResult::ER_Success){
     quint16 ret = gChromXTestParamSet.writeRunParam(runParam);
        ICore::showMessageCCEAPIResult(ret);
    //}
    if(ret == CCEAPI::EResult::ER_Success){
        ret = gChromXTestParamSet.writePressureMode(pressureMode);
        ICore::showMessageCCEAPIResult(ret);
    }
    if(ret == CCEAPI::EResult::ER_Success){
        ret = gChromXTestParamSet.writeTestStatus(1);
        ICore::showMessageCCEAPIResult(ret);
    }
    m_chart->slot_clearChart();

    saveTemplateFile(CXAJsonTestParamSet::lastUsedFilePath());
}

void CXATestParamSet::on_btn_Stop_clicked()
{
    quint16 ret = gChromXTestParamSet.writeTestStatus(0);
    ICore::showMessageCCEAPIResult(ret);
}

void CXATestParamSet::on_btn_Reset_clicked()
{
//    if(QFile::exists(CXAJsonTestParamSet::lastUsedFilePath())){
//        m_jsonTestParamSet->readJsonFrom(CXAJsonTestParamSet::lastUsedFilePath());
//    }
//    else if(QFile::exists(CXAJsonTestParamSet::defaultFilePath())){
//        m_jsonTestParamSet->readJsonFrom(CXAJsonTestParamSet::defaultFilePath());
//    }
//    else{
        m_jsonTestParamSet->setTestParam(STestParamSet());
        //saveTemplateFile(CXAJsonTestParamSet::defaultFilePath());
    //}
    updateUIFromJson();
}

void CXATestParamSet::on_btn_ToChart_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void CXATestParamSet::on_btn_ChartBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void CXATestParamSet::on_btn_PIDSetRead_clicked()
{
    quint16 ret = gChromXTestParamSet.readPIDAll();
    ICore::showMessageCCEAPIResult(ret);
    if(ret == CCEAPI::EResult::ER_Success){
        SPIDAll& pidAll = gChromXTestParamSet.getPIDAll();

        SUiPIDAll uiPIDAll;
        CXATestParamSetConvertor temporary;
        temporary.convertPIDAllParamToUi(pidAll, uiPIDAll);

        //PID Settings
        ui->doubleSpinBox_TD_P->setValue(uiPIDAll.TD_PID_P_Parma);
        ui->doubleSpinBox_TD_I->setValue(uiPIDAll.TD_PID_I_Parma);
        ui->doubleSpinBox_TD_D->setValue(uiPIDAll.TD_PID_D_Parma);

        ui->doubleSpinBox_TI_P->setValue(uiPIDAll.TI_PID_P_Parma);
        ui->doubleSpinBox_TI_I->setValue(uiPIDAll.TI_PID_I_Parma);
        ui->doubleSpinBox_TI_D->setValue(uiPIDAll.TI_PID_D_Parma);

        ui->doubleSpinBox_COLUMN_P->setValue(uiPIDAll.COLUMN_PID_P_Parma);
        ui->doubleSpinBox_COLUMN_I->setValue(uiPIDAll.COLUMN_PID_I_Parma);
        ui->doubleSpinBox_COLUMN_D->setValue(uiPIDAll.COLUMN_PID_D_Parma);
    }
}

void CXATestParamSet::on_btn_PIDSetWrite_clicked()
{
     SUiPIDAll uiPIDAll;
     //PID Settings
     uiPIDAll.TD_PID_P_Parma = ui->doubleSpinBox_TD_P->value();
     uiPIDAll.TD_PID_I_Parma = ui->doubleSpinBox_TD_I->value();
     uiPIDAll.TD_PID_D_Parma = ui->doubleSpinBox_TD_D->value();

     uiPIDAll.TI_PID_P_Parma = ui->doubleSpinBox_TI_P->value();
     uiPIDAll.TI_PID_I_Parma = ui->doubleSpinBox_TI_I->value();
     uiPIDAll.TI_PID_D_Parma = ui->doubleSpinBox_TI_D->value();

     uiPIDAll.COLUMN_PID_P_Parma = ui->doubleSpinBox_COLUMN_P->value();
     uiPIDAll.COLUMN_PID_I_Parma = ui->doubleSpinBox_COLUMN_I->value();
     uiPIDAll.COLUMN_PID_D_Parma = ui->doubleSpinBox_COLUMN_D->value();

     SPIDAll pidAll;
     CXATestParamSetConvertor temporary;
     temporary.convertPIDAllParamFromUi(uiPIDAll, pidAll);

    /*SPIDALL*/
    quint16 ret = gChromXTestParamSet.writePIDAll(pidAll);
    ICore::showMessageCCEAPIResult(ret);
}

void CXATestParamSet::on_btn_SaveASDefault_clicked()
{
    saveTemplateFile(CXAJsonTestParamSet::defaultFilePath());
}

void CXATestParamSet::on_btn_SaveAs_clicked()
{
    exportTemplateFile();
}

void CXATestParamSet::on_btn_ImportTemplate_clicked()
{
    importTemplateFile();
}

void CXATestParamSet::on_btn_PressureRead_clicked()
{
    quint16 ret = gChromXTestParamSet.readPressureMode();
    ICore::showMessageCCEAPIResult(ret);
    if(ret == CCEAPI::EResult::ER_Success){
        SPressureMode& temp = gChromXTestParamSet.getPressureMode();
        showTableValueFromJson(ui->tableWidgetPressure,temp.pressureCtrlArray,8);
    }
    ret = gChromXTestParamSet.readCounterBlowingTime();
    ICore::showMessageCCEAPIResult(ret);
    if(ret == CCEAPI::EResult::ER_Success){
        ui->spinBox_TD_CounterBlowingTime->setValue(gChromXTestParamSet.getCounterBlowingTime());
    }
}

void CXATestParamSet::on_btn_PressureWrite_clicked()
{
    SPressureMode tempPressure;
    getTableValueToJson(ui->tableWidgetPressure,tempPressure.pressureCtrlArray,8);
    quint16 ret = gChromXTestParamSet.writePressureMode(tempPressure);
    ICore::showMessageCCEAPIResult(ret);
    ret = gChromXTestParamSet.writeCounterBlowingTime(ui->spinBox_TD_CounterBlowingTime->value());
    ICore::showMessageCCEAPIResult(ret);
}

void CXATestParamSet::tableWidgetCellChanged(int row, int column)
{
    QTableWidget* senderObj = qobject_cast<QTableWidget*>(this->sender());
    if(!senderObj){
        return;
    }
    if(column==ETableCol::ECol_PIDTimeValue){
        computTablePIDValue(senderObj,row);
    }
    else if(column==ETableCol::ECol_PIDTemperValue){
        computTablePIDValue(senderObj,row);
    }
}

void CXATestParamSet::initUI()
{
    initSignalAndSlot();
    initChartUI();

    ui->tableWidgetTD->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tableWidgetTD->setItemDelegateForColumn(ETableCol::ECol_PIDTimeValue,new SpinBoxDelegate(this));
    ui->tableWidgetTD->setItemDelegateForColumn(ETableCol::ECol_PIDTemperValue,new DoubleSpinBoxDelegate(this));
    ui->tableWidgetTD->setItemDelegateForColumn(ETableCol::ECol_TimeValue,new SpinBoxDelegate(this));
    ui->tableWidgetTD->setItemDelegateForColumn(ETableCol::ECol_TimePWMValue,new SpinBoxDelegate(0,100,this));
    ui->tableWidgetTD->setItemDelegateForColumn(ETableCol::ECol_PIDTimeDifferenceValue,new ReadOnlyDelegate(this));
    ui->tableWidgetTD->setItemDelegateForColumn(ETableCol::ECol_PIDTemperRate,new ReadOnlyDelegate(this));

    ui->tableWidgetTI->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tableWidgetTI->setItemDelegateForColumn(ETableCol::ECol_PIDTimeValue,new SpinBoxDelegate(this));
    ui->tableWidgetTI->setItemDelegateForColumn(ETableCol::ECol_PIDTemperValue,new DoubleSpinBoxDelegate(this));
    ui->tableWidgetTI->setItemDelegateForColumn(ETableCol::ECol_TimeValue,new SpinBoxDelegate(this));
    ui->tableWidgetTI->setItemDelegateForColumn(ETableCol::ECol_TimePWMValue,new SpinBoxDelegate(0,100,this));
    ui->tableWidgetTI->setItemDelegateForColumn(ETableCol::ECol_PIDTimeDifferenceValue,new ReadOnlyDelegate(this));
    ui->tableWidgetTI->setItemDelegateForColumn(ETableCol::ECol_PIDTemperRate,new ReadOnlyDelegate(this));

    ui->tableWidgetCOLUMN->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tableWidgetCOLUMN->setItemDelegateForColumn(ETableCol::ECol_PIDTimeValue,new SpinBoxDelegate(this));
    ui->tableWidgetCOLUMN->setItemDelegateForColumn(ETableCol::ECol_PIDTemperValue,new DoubleSpinBoxDelegate(this));
    ui->tableWidgetCOLUMN->setItemDelegateForColumn(ETableCol::ECol_TimeValue,new SpinBoxDelegate(this));
    ui->tableWidgetCOLUMN->setItemDelegateForColumn(ETableCol::ECol_TimePWMValue,new SpinBoxDelegate(0,100,this));
    ui->tableWidgetCOLUMN->setItemDelegateForColumn(ETableCol::ECol_PIDTimeDifferenceValue,new ReadOnlyDelegate(this));
    ui->tableWidgetCOLUMN->setItemDelegateForColumn(ETableCol::ECol_PIDTemperRate,new ReadOnlyDelegate(this));

    ui->tableWidgetPressure->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tableWidgetPressure->setItemDelegateForColumn(0,new SpinBoxDelegate(this));
    ui->tableWidgetPressure->setItemDelegateForColumn(1,new SpinBoxDelegate(this));

    changeTabelShowMode(ui->tableWidgetTD,false);
    changeTabelShowMode(ui->tableWidgetTI,false);
    changeTabelShowMode(ui->tableWidgetCOLUMN,false);

    //载入上一次的配置文件
    if(QFile::exists(CXAJsonTestParamSet::lastUsedFilePath())){
        m_jsonTestParamSet->readJsonFrom(CXAJsonTestParamSet::lastUsedFilePath());
    }
    updateUIFromJson();
}

void CXATestParamSet::initSignalAndSlot()
{
    connect(ui->comboBox_TD_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged),this,
         [=](int index){ changeTabelShowMode(ui->tableWidgetTD,bool(index)); });
    connect(ui->comboBox_TI_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged),this,
         [=](int index){ changeTabelShowMode(ui->tableWidgetTI,bool(index)); });
    connect(ui->comboBox_COLUMN_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged),this,
         [=](int index){ changeTabelShowMode(ui->tableWidgetCOLUMN,bool(index)); });

    connect(ui->tableWidgetTD,&QTableWidget::cellChanged,this,&CXATestParamSet::tableWidgetCellChanged);
    connect(ui->tableWidgetTI,&QTableWidget::cellChanged,this,&CXATestParamSet::tableWidgetCellChanged);
    connect(ui->tableWidgetCOLUMN,&QTableWidget::cellChanged,this,&CXATestParamSet::tableWidgetCellChanged);
}

void CXATestParamSet::initChartUI()
{
    QVBoxLayout *vBoxlayout = new QVBoxLayout();
    vBoxlayout->addWidget(m_chart);
    vBoxlayout->setMargin(0);
    ui->frameChart->setLayout(vBoxlayout);
}

bool CXATestParamSet::importTemplateFile()
{
    QString selectFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select File"),
                                                    App::lastOpenPath,
                                                    QString("%1").arg("Config File(*.json)"),
                                                    &selectFilter);
    if (fileName.isEmpty()) {
        return false;
    }
    App::lastOpenPath = fileName;
    App::lastOpenTestParamPath = fileName;
    App::writeConfig();

    if(!m_jsonTestParamSet->readJsonFrom(fileName)){
        return false;
    }
    updateUIFromJson();
    return true;
}

bool CXATestParamSet::exportTemplateFile()
{
    QString selectFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    App::lastOpenPath,
                                                    QString("%1").arg("Config File(*.json)"),
                                                    &selectFilter);
    if (fileName.isEmpty()) {
        return false;
    }
    App::lastOpenPath = fileName;
    App::writeConfig();


    QFileInfo fileInfo(fileName);
    ui->lineEdit_configName->setText(fileInfo.baseName());
    if(!saveTemplateFile(fileName)){
        return false;
    }
    return true;
}

bool CXATestParamSet::saveTemplateFile(QString filePath)
{
    updateJsonFromUI();
    QFileInfo fileInfo(filePath);
    m_jsonTestParamSet->setName(fileInfo.baseName());
    return m_jsonTestParamSet->saveJsonAs(filePath);
}

void CXATestParamSet::updateUIFromJson()
{
    STestParamSet tempParam = m_jsonTestParamSet->testParam();
    SUiTestParamSet uiTempParam;

    CXATestParamSetConvertor temporary;
    temporary.convertTestParamSetToUi(tempParam, uiTempParam);
    //Base Settings
    ui->lineEdit_configName->setText(m_jsonTestParamSet->name());

    ui->spinBox_cleaningTime->setValue(uiTempParam.runParamSet.cleaningTime);
    ui->spinBox_samplingTime->setValue(uiTempParam.runParamSet.samplingTime);
    ui->spinBox_samplingPumpVolt->setValue(uiTempParam.runParamSet.samplingPumpVoltage);
    ui->spinBox_EPCControlVolt->setValue(uiTempParam.runParamSet.EPCControlVoltage);

    //PID Settings
    ui->doubleSpinBox_TD_P->setValue(uiTempParam.PIDAll.TD_PID_P_Parma);
    ui->doubleSpinBox_TD_I->setValue(uiTempParam.PIDAll.TD_PID_I_Parma);
    ui->doubleSpinBox_TD_D->setValue(uiTempParam.PIDAll.TD_PID_D_Parma);

    ui->doubleSpinBox_TI_P->setValue(uiTempParam.PIDAll.TI_PID_P_Parma);
    ui->doubleSpinBox_TI_I->setValue(uiTempParam.PIDAll.TI_PID_I_Parma);
    ui->doubleSpinBox_TI_D->setValue(uiTempParam.PIDAll.TI_PID_D_Parma);

    ui->doubleSpinBox_COLUMN_P->setValue(uiTempParam.PIDAll.COLUMN_PID_P_Parma);
    ui->doubleSpinBox_COLUMN_I->setValue(uiTempParam.PIDAll.COLUMN_PID_I_Parma);
    ui->doubleSpinBox_COLUMN_D->setValue(uiTempParam.PIDAll.COLUMN_PID_D_Parma);

    //Dector
    SUiMicroPIDCtrl& uiMicroPIDCtrl = uiTempParam.runParamSet.microPIDCtrl;
    ui->spinBox_baseLineVolt->setValue(uiMicroPIDCtrl.baseLineVoltage);
    ui->spinBox_plasmaFreq->setValue(uiMicroPIDCtrl.plasmaFreq);
    ui->spinBox_samplingFreq->setValue(uiMicroPIDCtrl.samplingFreq);

    //FANS
    ui->doubleSpinBox_COLUMNFanCloseTemperature->setValue(uiTempParam.runParamSet.COLUMNFanCloseTemperature);

    //TD
    SUiTDCtrl& uiTDCtrl = uiTempParam.runParamSet.TDCtrl;
    ui->comboBox_TD_Mode->setCurrentIndex(uiTDCtrl.controlMode);
    ui->spinBox_TD_CounterBlowingTime->setValue(uiTDCtrl.CounterBlowingTime);
    ui->checkBox_TD_TestPcg->setChecked(uiTDCtrl.BeforeTDStartup_TestPCG);
    ui->spinBox_TD_LowLimit->setValue(uiTDCtrl.TDStart_CarrierPressure_LowLimit);
    ui->spinBox_TD_UpLimit->setValue(uiTDCtrl.TDStart_CarrierPressure_UpLimit);
    ui->checkBox_TD_TestTITemper->setChecked(uiTDCtrl.BeforeTDStartup_TestTITemperature);
    ui->doubleSpinBox_TD_TITemperMax->setValue(uiTDCtrl.BeforeTDStartup_TITemperature_Max); //TODO: 公式计算
    showTableValueFromJson(ui->tableWidgetTD, uiTDCtrl.timeCtrlArray, uiTDCtrl.PIDCtrlArray, 5, uiTDCtrl.controlMode);

    //TI
    SUiTICtrl& uiTICtrl = uiTempParam.runParamSet.TICtrl;
    ui->comboBox_TI_Mode->setCurrentIndex(uiTICtrl.controlMode);
    ui->checkBox_TI_TestMicroPID->setChecked(uiTICtrl.BeforeTIStartup_TestMicroPID);
    ui->doubleSpinBox_TI_MicroPIDValueMin->setValue(uiTICtrl.BeforeTIStartup_MicroPIDValue_Min);
    showTableValueFromJson(ui->tableWidgetTI, uiTICtrl.timeCtrlArray, uiTICtrl.PIDCtrlArray, 5, uiTICtrl.controlMode);

    //COLUMN
    SUiCOLUMNCtrl& uiCOLUMNCtrl = uiTempParam.runParamSet.COLUMNCtrl;
    ui->comboBox_COLUMN_Mode->setCurrentIndex(uiCOLUMNCtrl.controlMode);
    showTableValueFromJson(ui->tableWidgetCOLUMN, uiCOLUMNCtrl.timeCtrlArray, uiCOLUMNCtrl.PIDCtrlArray, 8, uiCOLUMNCtrl.controlMode);

    //Pressure
    SPressureMode& PressureMode = tempParam.pressureMode;
    showTableValueFromJson(ui->tableWidgetPressure,PressureMode.pressureCtrlArray,8);
}

void CXATestParamSet::updateJsonFromUI()
{
    STestParamSet tempParam = m_jsonTestParamSet->testParam();
    SUiTestParamSet uiTempParam;
    //Base Settings
    m_jsonTestParamSet->setName(ui->lineEdit_configName->text());
    uiTempParam.runParamSet.cleaningTime = ui->spinBox_cleaningTime->value();
    uiTempParam.runParamSet.samplingTime = ui->spinBox_samplingTime->value();
    uiTempParam.runParamSet.samplingPumpVoltage = ui->spinBox_samplingPumpVolt->value();
    uiTempParam.runParamSet.EPCControlVoltage = ui->spinBox_EPCControlVolt->value();

    uiTempParam.runParamSet.testData_AutoRepo = 1;
    //PID Settings
    uiTempParam.PIDAll.TD_PID_P_Parma = ui->doubleSpinBox_TD_P->value();
    uiTempParam.PIDAll.TD_PID_I_Parma = ui->doubleSpinBox_TD_I->value();
    uiTempParam.PIDAll.TD_PID_D_Parma = ui->doubleSpinBox_TD_D->value();

    uiTempParam.PIDAll.TI_PID_P_Parma = ui->doubleSpinBox_TI_P->value();
    uiTempParam.PIDAll.TI_PID_I_Parma = ui->doubleSpinBox_TI_I->value();
    uiTempParam.PIDAll.TI_PID_D_Parma = ui->doubleSpinBox_TI_D->value();

    uiTempParam.PIDAll.COLUMN_PID_P_Parma = ui->doubleSpinBox_COLUMN_P->value();
    uiTempParam.PIDAll.COLUMN_PID_I_Parma = ui->doubleSpinBox_COLUMN_I->value();
    uiTempParam.PIDAll.COLUMN_PID_D_Parma = ui->doubleSpinBox_COLUMN_D->value();

    //Dector
    SUiMicroPIDCtrl& uiMicroPIDCtrl = uiTempParam.runParamSet.microPIDCtrl;
    uiMicroPIDCtrl.baseLineVoltage = ui->spinBox_baseLineVolt->value();
    uiMicroPIDCtrl.plasmaFreq = ui->spinBox_plasmaFreq->value();
    uiMicroPIDCtrl.samplingFreq = ui->spinBox_samplingFreq->value();

    //FANS
    uiTempParam.runParamSet.COLUMNFanCloseTemperature = ui->doubleSpinBox_COLUMNFanCloseTemperature->value();

    //TD
    SUiTDCtrl& uiTDCtrl = uiTempParam.runParamSet.TDCtrl;
    uiTDCtrl.controlMode = (bool)ui->comboBox_TD_Mode->currentIndex();
    uiTDCtrl.CounterBlowingTime = ui->spinBox_TD_CounterBlowingTime->value();
    uiTDCtrl.BeforeTDStartup_TestPCG = ui->checkBox_TD_TestPcg->isChecked();
    uiTDCtrl.TDStart_CarrierPressure_LowLimit = ui->spinBox_TD_LowLimit->value();
    uiTDCtrl.TDStart_CarrierPressure_UpLimit = ui->spinBox_TD_UpLimit->value();
    uiTDCtrl.BeforeTDStartup_TestTITemperature = ui->checkBox_TD_TestTITemper->isChecked();
    uiTDCtrl.BeforeTDStartup_TITemperature_Max = ui->doubleSpinBox_TD_TITemperMax->value();
    getTableValueToJson(ui->tableWidgetTD, uiTDCtrl.timeCtrlArray, uiTDCtrl.PIDCtrlArray, 5);

    //TI
    SUiTICtrl& uiTICtrl = uiTempParam.runParamSet.TICtrl;
    uiTICtrl.controlMode = (bool)ui->comboBox_TI_Mode->currentIndex();
    uiTICtrl.BeforeTIStartup_TestMicroPID = ui->checkBox_TI_TestMicroPID->isChecked();
    uiTICtrl.BeforeTIStartup_MicroPIDValue_Min = ui->doubleSpinBox_TI_MicroPIDValueMin->value();
    getTableValueToJson(ui->tableWidgetTI, uiTICtrl.timeCtrlArray, uiTICtrl.PIDCtrlArray, 5);

    //COLUMN
    SUiCOLUMNCtrl& uiCOLUMNCtrl = uiTempParam.runParamSet.COLUMNCtrl;
    uiCOLUMNCtrl.controlMode = (bool)ui->comboBox_COLUMN_Mode->currentIndex();
    getTableValueToJson(ui->tableWidgetCOLUMN, uiCOLUMNCtrl.timeCtrlArray, uiCOLUMNCtrl.PIDCtrlArray, 8);

    //Pressure
    SPressureMode& PressureMode = tempParam.pressureMode;
    getTableValueToJson(ui->tableWidgetPressure, PressureMode.pressureCtrlArray, 8);

    CXATestParamSetConvertor temporary;
    temporary.convertTestParamSetFromUi(uiTempParam, tempParam);

    m_jsonTestParamSet->setTestParam(tempParam);
}

void CXATestParamSet::showTableValueFromJson(QTableWidget* table, SUiTimeCtrl* uiTimeCtrl, SUiPIDCtrl* uiPIDCtrl, int rowSize, bool autoPID)
{
    if(!table||!uiTimeCtrl||!uiPIDCtrl){
        return;
    }
    const int RowCount = rowSize;
    table->clearContents();
    table->setRowCount(RowCount);//总行数
    for(int i = 0;i<RowCount;++i){
        SUiTimeCtrl& tempTimeCtrl = uiTimeCtrl[i];
        SUiPIDCtrl& tempPIDCtrl = uiPIDCtrl[i];

        table->setItem(i, ECol_PIDTimeValue, new QTableWidgetItem(QString::number(tempPIDCtrl.timeValue)));
        table->item(i, ECol_PIDTimeValue)->setTextAlignment(Qt::AlignCenter);

        if(tempPIDCtrl.temperatureValue<0){
            tempPIDCtrl.temperatureValue = 0;
        }
        QString str = QString::number(tempPIDCtrl.temperatureValue, 'f', 1);
        table->setItem(i, ECol_PIDTemperValue, new QTableWidgetItem(str)); //TODO: 公式计算
        table->item(i, ECol_PIDTemperValue)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, ECol_TimeValue, new QTableWidgetItem(QString::number(tempTimeCtrl.timeValue)));
        table->item(i, ECol_TimeValue)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, ECol_TimePWMValue, new QTableWidgetItem(QString::number(tempTimeCtrl.PWMValue)));
        table->item(i, ECol_TimePWMValue)->setTextAlignment(Qt::AlignCenter);
    }
    computTablePIDValue(table);
    changeTabelShowMode(table,autoPID);
}

void CXATestParamSet::showTableValueFromJson(QTableWidget *table, SPressureCtrl *pressureCtrl, int rowSize)
{
    if(!table||!pressureCtrl){
        return;
    }
    const int RowCount = rowSize;
    table->clearContents();
    table->setRowCount(RowCount);//总行数
    for(int i = 0;i<RowCount;++i){
        SPressureCtrl& tempPressureCtrl = pressureCtrl[i];

        table->setItem(i, 0, new QTableWidgetItem(QString::number(tempPressureCtrl.timeValue)));
        table->item(i, 0)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 1, new QTableWidgetItem(QString::number(tempPressureCtrl.pressureValue)));
        table->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }
}

void CXATestParamSet::getTableValueToJson(QTableWidget *table, SUiTimeCtrl *uiTimeCtrl, SUiPIDCtrl *uiPIDCtrl, int rowSize)
{
    if(!table||!uiTimeCtrl||!uiPIDCtrl){
        return;
    }
    const int RowCount = table->rowCount();
    if(rowSize!=RowCount){
        return;
    }
    for(int i = 0; i < RowCount; ++i) {
        SUiTimeCtrl& tempTimeCtrl = uiTimeCtrl[i];
        SUiPIDCtrl& tempPIDCtrl = uiPIDCtrl[i];

        tempPIDCtrl.timeValue = table->item(i, ECol_PIDTimeValue)->text().toUShort();
        tempPIDCtrl.temperatureValue = table->item(i, ECol_PIDTemperValue)->text().toDouble();

        tempTimeCtrl.timeValue = table->item(i, ECol_TimeValue)->text().toUShort();
        tempTimeCtrl.PWMValue = table->item(i, ECol_TimePWMValue)->text().toUShort();
    }
}

void CXATestParamSet::getTableValueToJson(QTableWidget *table, SPressureCtrl *pressureCtrl, int rowSize)
{
    if(!table||!pressureCtrl){
        return;
    }
    const int RowCount = table->rowCount();
    if(rowSize!=RowCount){
        return;
    }
    for(int i = 0;i<RowCount;++i){
        SPressureCtrl& tempPressureCtrl = pressureCtrl[i];

        tempPressureCtrl.timeValue = table->item(i, 0)->text().toUShort();
        tempPressureCtrl.pressureValue = table->item(i, 1)->text().toUShort();
    }
}

void CXATestParamSet::computTablePIDValue(QTableWidget *table,int rowIndex)
{
    if(!table){
        return;
    }
    double TimeConversion = (table==ui->tableWidgetTI)?60*1000:60;
    const int RowCount = table->rowCount();
    for(int i = 0;i<RowCount;++i){
        if(rowIndex!=-1&&rowIndex!=i){
            continue;
        }
        double timediffer = 0;
        int temperdiffer = 0;
        double temperRate = 0;
        QTableWidgetItem* curRowItem = table->item(i, ECol_PIDTimeValue);
        QTableWidgetItem* lastRowItem = table->item(i - 1, ECol_PIDTimeValue);
        if(curRowItem&&lastRowItem){
            timediffer = curRowItem->text().toUShort() - lastRowItem->text().toUShort();
        }
        curRowItem = table->item(i, ECol_PIDTemperValue);
        lastRowItem = table->item(i - 1, ECol_PIDTemperValue);
        if(curRowItem&&lastRowItem&&timediffer>0){
            temperdiffer = curRowItem->text().toDouble() - lastRowItem->text().toDouble();
            temperRate = (temperdiffer/timediffer)*TimeConversion;
        }
        table->setItem(i, ECol_PIDTimeDifferenceValue, new QTableWidgetItem(QString::number(timediffer)));
        table->item(i, ECol_PIDTimeDifferenceValue)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, ECol_PIDTemperRate, new QTableWidgetItem(QString::number(temperRate,'f',1)));
        table->item(i, ECol_PIDTemperRate)->setTextAlignment(Qt::AlignCenter);
    }
}

void CXATestParamSet::changeTabelShowMode(QTableWidget *table, bool autoPID)
{
    if(!table){
        return;
    }
    if(autoPID){
        table->showColumn(ETableCol::ECol_PIDTimeValue);
        table->showColumn(ETableCol::ECol_PIDTemperValue);
        table->showColumn(ETableCol::ECol_PIDTimeDifferenceValue);
        table->showColumn(ETableCol::ECol_PIDTemperRate);
        table->hideColumn(ETableCol::ECol_TimeValue);
        table->hideColumn(ETableCol::ECol_TimePWMValue);
    }else{
        table->hideColumn(ETableCol::ECol_PIDTimeValue);
        table->hideColumn(ETableCol::ECol_PIDTemperValue);
        table->hideColumn(ETableCol::ECol_PIDTimeDifferenceValue);
        table->hideColumn(ETableCol::ECol_PIDTemperRate);
        table->showColumn(ETableCol::ECol_TimeValue);
        table->showColumn(ETableCol::ECol_TimePWMValue);
    }
}
