#include "cxatestparamset.h"
#include "ui_cxatestparamset.h"

#include "app.h"
#include "icore.h"

#include <JsonHelper/cxajsontestparamset.h>
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
    gChromXTestParamSet.readPIDAll();
}

void CXATestParamSet::on_btn_SaveASDefault_clicked()
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
}

void CXATestParamSet::initSignalAndSlot()
{
    connect(ui->comboBox_TD_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged),
         [=](int index){ changeTabelShowMode(ui->tableWidgetTD,bool(index)); });
    connect(ui->comboBox_TI_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged),
         [=](int index){ changeTabelShowMode(ui->tableWidgetTI,bool(index)); });
    connect(ui->comboBox_COLUMN_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged),
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
    updateJsonFromUI();
    if(!m_jsonTestParamSet->saveJsonAs(fileName)){
        return false;
    }
    return true;
}

void CXATestParamSet::updateUIFromJson()
{
    STestParamSet tempParam = m_jsonTestParamSet->testParam();
    //Base Settings
    ui->lineEdit_configName->setText(m_jsonTestParamSet->name());
    ui->spinBox_cleaningTime->setValue(tempParam.runParamSet.cleaningTime);
    ui->spinBox_samplingTime->setValue(tempParam.runParamSet.samplingTime);
    ui->spinBox_samplingPumpVolt->setValue(tempParam.runParamSet.samplingPumpVoltage);
    ui->spinBox_EPCControlVolt->setValue(tempParam.runParamSet.EPCControlVoltage);

    //PID Settings
    ui->doubleSpinBox_TD_P->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_TD,SPIDAll::EPID_P));
    ui->doubleSpinBox_TD_I->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_TD,SPIDAll::EPID_I));
    ui->doubleSpinBox_TD_D->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_TD,SPIDAll::EPID_D));

    ui->doubleSpinBox_TI_P->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_TI,SPIDAll::EPID_P));
    ui->doubleSpinBox_TI_I->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_TI,SPIDAll::EPID_I));
    ui->doubleSpinBox_TI_D->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_TI,SPIDAll::EPID_D));

    ui->doubleSpinBox_COLUMN_P->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_COLUMN,SPIDAll::EPID_P));
    ui->doubleSpinBox_COLUMN_I->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_COLUMN,SPIDAll::EPID_I));
    ui->doubleSpinBox_COLUMN_D->setValue(tempParam.PIDAll.getValueToUI(SPIDAll::ED_COLUMN,SPIDAll::EPID_D));

    //Dector
    ui->spinBox_baseLineVolt->setValue(tempParam.runParamSet.microPIDCtrl.baseLineVoltage);
    ui->spinBox_plasmaFreq->setValue(tempParam.runParamSet.microPIDCtrl.plasmaFreq);
    ui->spinBox_samplingFreq->setValue(tempParam.runParamSet.microPIDCtrl.samplingFreq);

    //FANS
    ui->spinBox_COLUMNFanCloseTemperature->setValue(tempParam.runParamSet.COLUMNFanCloseTemperature);

    //TD
    STDCtrl& TDCtrl = tempParam.runParamSet.TDCtrl;
    ui->comboBox_TD_Mode->setCurrentIndex((bool)TDCtrl.controlMode);
    ui->spinBox_TD_CounterBlowingTime->setValue(TDCtrl.CounterBlowingTime);
    ui->checkBox_TD_TestPcg->setChecked((bool)TDCtrl.BeforeTDStartup_TestPCG);
    ui->spinBox_TD_LowLimit->setValue(TDCtrl.TDStart_CarrierPressure_LowLimit);
    ui->spinBox_TD_UpLimit->setValue(TDCtrl.TDStart_CarrierPressure_UpLimit);
    ui->checkBox_TD_TestTITemper->setChecked((bool)TDCtrl.BeforeTDStartup_TestTITemperature);
    ui->spinBox_TD_TITemperMax->setValue(TDCtrl.BeforeTDStartup_TITemperature_Max); //TODO: 公式计算
    showTableValueFromJson(ui->tableWidgetTD,TDCtrl.timeCtrlArray,TDCtrl.PIDCtrlArray,5,TDCtrl.controlMode);

    //TI
    STICtrl& TICtrl = tempParam.runParamSet.TICtrl;
    ui->comboBox_TI_Mode->setCurrentIndex((bool)TICtrl.controlMode);
    ui->checkBox_TI_TestMicroPID->setChecked((bool)TICtrl.BeforeTIStartup_TestMicroPID);
    ui->doubleSpinBox_TI_MicroPIDValueMin->setValue(TICtrl.BeforeTIStartup_MicroPIDValue_Min);
    showTableValueFromJson(ui->tableWidgetTI,TICtrl.timeCtrlArray,TICtrl.PIDCtrlArray,5,TICtrl.controlMode);

    //COLUMN
    SCOLUMNCtrl& COLUMNCtrl = tempParam.runParamSet.COLUMNCtrl;
    ui->comboBox_COLUMN_Mode->setCurrentIndex((bool)COLUMNCtrl.controlMode);
    showTableValueFromJson(ui->tableWidgetCOLUMN,COLUMNCtrl.timeCtrlArray,COLUMNCtrl.PIDCtrlArray,8,COLUMNCtrl.controlMode);

    //Pressure
    SPressureMode& PressureMode = tempParam.pressureMode;
    showTableValueFromJson(ui->tableWidgetPressure,PressureMode.pressureCtrlArray,8);
}

void CXATestParamSet::updateJsonFromUI()
{
    STestParamSet tempParam = m_jsonTestParamSet->testParam();
    //Base Settings
    m_jsonTestParamSet->setName(ui->lineEdit_configName->text());
    tempParam.runParamSet.cleaningTime = ui->spinBox_cleaningTime->value();
    tempParam.runParamSet.samplingTime = ui->spinBox_samplingTime->value();
    tempParam.runParamSet.samplingPumpVoltage = ui->spinBox_samplingPumpVolt->value();
    tempParam.runParamSet.EPCControlVoltage = ui->spinBox_EPCControlVolt->value();

    tempParam.runParamSet.testData_AutoRepo = 1;
    //PID Settings
    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_TD,SPIDAll::EPID_P,ui->doubleSpinBox_TD_P->value());
    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_TD,SPIDAll::EPID_I,ui->doubleSpinBox_TD_I->value());
    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_TD,SPIDAll::EPID_D,ui->doubleSpinBox_TD_D->value());

    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_TI,SPIDAll::EPID_P,ui->doubleSpinBox_TI_P->value());
    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_TI,SPIDAll::EPID_I,ui->doubleSpinBox_TI_I->value());
    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_TI,SPIDAll::EPID_D,ui->doubleSpinBox_TI_D->value());

    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_COLUMN,SPIDAll::EPID_P,ui->doubleSpinBox_COLUMN_P->value());
    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_COLUMN,SPIDAll::EPID_I,ui->doubleSpinBox_COLUMN_I->value());
    tempParam.PIDAll.setValueFromUI(SPIDAll::ED_COLUMN,SPIDAll::EPID_D,ui->doubleSpinBox_COLUMN_D->value());

    //Dector
    tempParam.runParamSet.microPIDCtrl.baseLineVoltage = ui->spinBox_baseLineVolt->value();
    tempParam.runParamSet.microPIDCtrl.plasmaFreq = ui->spinBox_plasmaFreq->value();
    tempParam.runParamSet.microPIDCtrl.samplingFreq = ui->spinBox_samplingFreq->value();

    //FANS
    tempParam.runParamSet.COLUMNFanCloseTemperature = ui->spinBox_COLUMNFanCloseTemperature->value();

    //TD
    STDCtrl& TDCtrl = tempParam.runParamSet.TDCtrl;
    TDCtrl.controlMode = (bool)ui->comboBox_TD_Mode->currentIndex();
    TDCtrl.CounterBlowingTime = ui->spinBox_TD_CounterBlowingTime->value();
    TDCtrl.BeforeTDStartup_TestPCG = ui->checkBox_TD_TestPcg->isChecked();
    TDCtrl.TDStart_CarrierPressure_LowLimit = ui->spinBox_TD_LowLimit->value();
    TDCtrl.TDStart_CarrierPressure_UpLimit = ui->spinBox_TD_UpLimit->value();
    TDCtrl.BeforeTDStartup_TestTITemperature = ui->checkBox_TD_TestTITemper->isChecked();
    TDCtrl.BeforeTDStartup_TITemperature_Max = ui->spinBox_TD_TITemperMax->value();
    getTableValueToJson(ui->tableWidgetTD,TDCtrl.timeCtrlArray,TDCtrl.PIDCtrlArray,5);

    //TI
    STICtrl& TICtrl = tempParam.runParamSet.TICtrl;
    TICtrl.controlMode = (bool)ui->comboBox_TI_Mode->currentIndex();
    TICtrl.BeforeTIStartup_TestMicroPID = ui->checkBox_TI_TestMicroPID->isChecked();
    TICtrl.BeforeTIStartup_MicroPIDValue_Min = ui->doubleSpinBox_TI_MicroPIDValueMin->value();
    getTableValueToJson(ui->tableWidgetTI,TICtrl.timeCtrlArray,TICtrl.PIDCtrlArray,5);

    //COLUMN
    SCOLUMNCtrl& COLUMNCtrl = tempParam.runParamSet.COLUMNCtrl;
    COLUMNCtrl.controlMode = (bool)ui->comboBox_COLUMN_Mode->currentIndex();
    getTableValueToJson(ui->tableWidgetCOLUMN,COLUMNCtrl.timeCtrlArray,COLUMNCtrl.PIDCtrlArray,8);

    //Pressure
    SPressureMode& PressureMode = tempParam.pressureMode;
    getTableValueToJson(ui->tableWidgetPressure,PressureMode.pressureCtrlArray,8);

    m_jsonTestParamSet->setTestParam(tempParam);
}

void CXATestParamSet::showTableValueFromJson(QTableWidget *table, STimeCtrl *timeCtrl, SPIDCtrl *PIDCtrl, int rowSize,bool autoPID)
{
    if(!table||!timeCtrl||!PIDCtrl){
        return;
    }
    const int RowCount = rowSize;
    table->clearContents();
    table->setRowCount(RowCount);//总行数
    for(int i = 0;i<RowCount;++i){
        STimeCtrl& tempTimeCtrl = timeCtrl[i];
        SPIDCtrl& tempPIDCtrl = PIDCtrl[i];

        table->setItem(i, ECol_PIDTimeValue, new QTableWidgetItem(QString::number(tempPIDCtrl.timeValue)));
        table->item(i, ECol_PIDTimeValue)->setTextAlignment(Qt::AlignCenter);

        QString str = QString::number(double(tempPIDCtrl.temperatureValue)/10.0,'f',1);
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

void CXATestParamSet::getTableValueToJson(QTableWidget *table, STimeCtrl *timeCtrl, SPIDCtrl *PIDCtrl,int rowSize)
{
    if(!table||!timeCtrl||!PIDCtrl){
        return;
    }
    const int RowCount = table->rowCount();
    if(rowSize!=RowCount){
        return;
    }
    for(int i = 0;i<RowCount;++i){
        STimeCtrl& tempTimeCtrl = timeCtrl[i];
        SPIDCtrl& tempPIDCtrl = PIDCtrl[i];

        tempPIDCtrl.timeValue = table->item(i, ECol_PIDTimeValue)->text().toUShort();
        tempPIDCtrl.temperatureValue = table->item(i, ECol_PIDTemperValue)->text().toUShort();

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
