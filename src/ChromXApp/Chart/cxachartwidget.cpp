﻿#include "cxachartwidget.h"
#include "ui_cxatestparamsetchart.h"

#include "cxatestdatatablemodel.h"
#include "cxasinglestatustablemodel.h"
#include "cxacustomchartview.h"

#include <CCE_ChromXItem/CCEChromXDevice>

#include <QtGlobal>
#include <QStringList>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableView>
#include <QToolButton>
#include <QtWidgets/QHeaderView>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>
#include <QtCharts/QVXYModelMapper>
#include <QtCharts/QLegendMarker>
QT_CHARTS_USE_NAMESPACE

CXAChartWidget::CXAChartWidget(EChartMode mode/* = ECM_TestData*/, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CXATestParamSetChart),
    m_axisX(new QValueAxis()),
    m_axisY2(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_x(0),
    m_y(10),
    m_y2(10),
    m_chartMode(mode)
{
    ui->setupUi(this);
    initUI();
}

CXAChartWidget::~CXAChartWidget()
{
    m_timer.stop();
    delete ui;
}

void CXAChartWidget::setChannelVisible(EDataChannel channel, bool visible)
{
    for(auto&& item:m_seriesInfoList){
        if(item.channel == channel){
            setSeriesVisible(item.lineSeries,visible);
            break;
        }
    }
}

bool CXAChartWidget::event(QEvent *e)
{
    if (e->type() == CCEPackageEvent::s_disPatcherPackage_eventType)
    {
        CCEPackageEvent *tempE = static_cast<CCEPackageEvent*>(e);
        if(tempE->unitAddr()==CCEPackage::EUnitAddr::EUA_TestData){
            if(/*tempE->frameType()==CCEPackage::EFrameType::EFT_ReportFrame&&*/
                    tempE->cmdNum()==CCEAbstractTestDataPackage::ECommand::EC_Read_TDCurTemperature){
                qDebug()<<"Event Got Report TestData!";
                handle_TestData_Read_AllInfo(tempE->package().data());
            }
        }
        else if(tempE->unitAddr()==CCEPackage::EUnitAddr::EUA_SingleStatus){
            switch (tempE->cmdNum()){
            case CCEAbstractSingleStatusPackage::ECommand::EC_Read_TDCurTemperature:
                qDebug()<<"Event Got TDCurTemperature!";
                handle_SingleStatus_Read_TDCurTemperature(tempE->package().data());
                break;
            case CCEAbstractSingleStatusPackage::ECommand::EC_Read_TICurTemperature:
                qDebug()<<"Event Got TICurTemperature!";
                handle_SingleStatus_Read_TICurTemperature(tempE->package().data());
                break;
            case CCEAbstractSingleStatusPackage::ECommand::EC_Read_COLUMNTemperature:
                qDebug()<<"Event Got COLUMNTemperature!";
                handle_SingleStatus_Read_COLUMNCurTemperature(tempE->package().data());
                break;
            case CCEAbstractSingleStatusPackage::ECommand::EC_Read_MicroPIDValue:
                qDebug()<<"Event Got MicroPIDValue!";
                handle_SingleStatus_Read_MicroPIDValue(tempE->package().data());
                break;
            case CCEAbstractSingleStatusPackage::ECommand::EC_Read_EPCPressure:
                qDebug()<<"Event Got EPCPressure!";
                handle_SingleStatus_Read_EPCPressure(tempE->package().data());
                break;
            default:
                break;
            }
        }
    }
    return QWidget::event(e);
}

void CXAChartWidget::initUI()
{
    initToolButton();
    initModel();
    initChart();

    // create main layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(m_toolBtnLayout,0,0,1,2);
    mainLayout->addWidget(m_tableView, 1, 0);
    mainLayout->addWidget(m_chartView, 1, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(0, 0);
    setLayout(mainLayout);

    this->setLayout(mainLayout);
    //m_tableView->setVisible(false);
}

void CXAChartWidget::initToolButton()
{
    m_toolBtnLayout = new QHBoxLayout;
    QToolButton* scatterSeriseVisible = new QToolButton;
    scatterSeriseVisible->setText(tr("Scatter Serise"));
    scatterSeriseVisible->setCheckable(true);
    scatterSeriseVisible->setChecked(false);
    connect(scatterSeriseVisible,&QToolButton::clicked,this,&CXAChartWidget::slot_scatterSeriesVisible);
    m_toolBtnLayout->addWidget(scatterSeriseVisible);

    QToolButton* fitInView = new QToolButton;
    fitInView->setText(tr("Fit In View"));
    connect(fitInView,&QToolButton::clicked,this,&CXAChartWidget::slot_fitInView);
    m_toolBtnLayout->addWidget(fitInView);

    QToolButton* refreshMode = new QToolButton;
    refreshMode->setText(tr("Dynamic Mode"));
    refreshMode->setCheckable(true);
    refreshMode->setChecked(true);
    connect(refreshMode,&QToolButton::clicked,this,&CXAChartWidget::slot_checkedDynamicMode);
    m_toolBtnLayout->addWidget(refreshMode);

    QToolButton* clearBtn = new QToolButton;
    clearBtn->setText(tr("Clear"));
    connect(clearBtn,&QToolButton::clicked,this,&CXAChartWidget::slot_clearChart);
    m_toolBtnLayout->addWidget(clearBtn);

    QToolButton* importCSVBtn = new QToolButton;
    importCSVBtn->setText(tr("Import CSV"));
    connect(importCSVBtn,&QToolButton::clicked,this,&CXAChartWidget::slot_importCSV);
    m_toolBtnLayout->addWidget(importCSVBtn);

    QToolButton* circleReadBtn = new QToolButton;
    circleReadBtn->setText(tr("Circle Read"));
    connect(circleReadBtn,&QToolButton::clicked,this,[=]{
        if(m_timer.isActive()){
            m_timer.stop();
        }
        else{
            m_timer.start();
        }
    });
    m_toolBtnLayout->addWidget(circleReadBtn);
}

void CXAChartWidget::initChart()
{
    m_seriesInfoList.clear();

    //init axis
    m_axisX->setTickCount(10);
    m_axisX->setRange(0, m_defalutXMax);
    m_axisY2->setTickCount(10);
    m_axisY2->setRange(0, m_y2);
    m_axisY->setTickCount(10);
    m_axisY->setRange(0, m_y);

    //init chart
    m_chart = new QChart();
    m_chart->setTitle("Simple line chart example");
    m_chart->addAxis(m_axisX,Qt::AlignBottom);
    m_chart->addAxis(m_axisY,Qt::AlignLeft);
    m_chart->addAxis(m_axisY2,Qt::AlignRight);

    //init chartView
    m_chartView = new CXACustomChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setRenderHint(QPainter::HighQualityAntialiasing);

    //add channel
    //channel 1
    addChannel(EDC_TDTemper,"TD Temper",0,1,m_axisX,m_axisY2);
    //channel 2
    addChannel(EDC_TITemper,"TI Temper",0,2,m_axisX,m_axisY2);
    //channel 3
    addChannel(EDC_COLUMNTemper,"COLUMN Temper",0,3,m_axisX,m_axisY2);
    //channel 4
    addChannel(EDC_MicroPID,"Micro PID",0,4,m_axisX,m_axisY);
    if(m_chartMode == ECM_SingleStatus){
        //channel 5
        addChannel(EDC_EPCPressure,"EPC Pressure",0,5,m_axisX,m_axisY2);
    }

    //other operation
    slot_scatterSeriesVisible(false);

    //connect signals
    QObject::connect(m_model, &CXATestDataTableModel::rowsInserted, this, &CXAChartWidget::handleDataAppend);

    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &CXAChartWidget::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &CXAChartWidget::handleMarkerClicked);
    }
}

void CXAChartWidget::initModel()
{
    if(m_chartMode==ECM_TestData){
        m_model = new CXATestDataTableModel;
        gDispatcher->registerDispatcherPackage(CCETestDataPackage_ReadAllInfo(),this);
        gDispatcher->registerDispatcherPackage(CCETestDataPackage_ReportAllInfo(),this);
    }
    else{
        m_model = new CXASingleStatusTableModel;
        gDispatcher->registerDispatcherPackage(CCESingleStatusPackage_ReadTDCurTemperature(),this);
        gDispatcher->registerDispatcherPackage(CCESingleStatusPackage_ReadTICurTemperature(),this);
        gDispatcher->registerDispatcherPackage(CCESingleStatusPackage_ReadCOLUMNTemperature(),this);
        gDispatcher->registerDispatcherPackage(CCESingleStatusPackage_ReadMicroPIDValue(),this);
        gDispatcher->registerDispatcherPackage(CCESingleStatusPackage_ReadEPCPressure(),this);
    }
    m_tableView = new QTableView;
    m_tableView->setModel(m_model);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //Test Code
    connect(&m_timer,&QTimer::timeout,this,[=]{
        if(m_chartMode == ECM_TestData){
            gChromXTestData.readAllInfo(true);
//            static unsigned int s_index = 0;
//            s_index += 1;
//            data.curTestRunTime = s_index;
//            data.TDCurTemperature = QRandomGenerator::global()->bounded(500);
//            data.TICurTemperature = QRandomGenerator::global()->bounded(500);
//            data.COLUMNTemperature = QRandomGenerator::global()->bounded(500);
//            data.MicroPIDValue = QRandomGenerator::global()->bounded(0xFFFFFFFF);
//            qobject_cast<CXATestDataTableModel*>(m_model)->appendModelData(data);
        }else{
            SSingleStatus data;
            data.TDCurTemperature = QRandomGenerator::global()->bounded(0xFFFF);
            data.TICurTemperature = QRandomGenerator::global()->bounded(0xFFFF);
            data.COLUMNTemperature = QRandomGenerator::global()->bounded(0xFFFF);
            data.MicroPIDValue = QRandomGenerator::global()->bounded(500);
            data.EPCPressure = QRandomGenerator::global()->bounded(0xFFFF);
            qobject_cast<CXASingleStatusTableModel*>(m_model)->appendModelData(data);
        }
        m_tableView->scrollToBottom();
    });
    if(m_chartMode==ECM_TestData){
        m_timer.setInterval(200);
    }
}

void CXAChartWidget::addChannel(EDataChannel channel, QString name, int modelXCol, int modelYCol, QtCharts::QValueAxis *x, QtCharts::QValueAxis *y)
{
    //折线图
    QLineSeries* series = new QLineSeries;
    //QSplineSeries* series = new QSplineSeries;
    series->setName(name);
    series->setUseOpenGL(true);
    QVXYModelMapper *mapper = new QVXYModelMapper(this);
    mapper->setXColumn(modelXCol);
    mapper->setYColumn(modelYCol);
    mapper->setSeries(series);
    mapper->setModel(m_model);
    m_chart->addSeries(series);
    series->attachAxis(x);
    series->attachAxis(y);
    connect(series, &QScatterSeries::hovered, m_chartView, &CXACustomChartView::tooltip);
    //散点图
    QScatterSeries* scatterSeries = new QScatterSeries;
    scatterSeries->setName(name);
    scatterSeries->setMarkerSize(10);
    scatterSeries->setUseOpenGL(true);
    mapper = new QVXYModelMapper(this);
    mapper->setXColumn(modelXCol);
    mapper->setYColumn(modelYCol);
    mapper->setSeries(scatterSeries);
    mapper->setModel(m_model);
    m_chart->addSeries(scatterSeries);
    scatterSeries->attachAxis(x);
    scatterSeries->attachAxis(y);
    connect(scatterSeries, &QScatterSeries::hovered, m_chartView, &CXACustomChartView::tooltip);

    SSeriesInfo info;
    info.name = name;
    info.channel = channel;
    info.lineSeries = series;
    info.scatterSeries = scatterSeries;
    switch(channel){
    case EDataChannel::EDC_COLUMNTemper:
        series->setColor(QColor(QStringLiteral("#ee4848")));
        break;
    case EDataChannel::EDC_TDTemper:
        series->setColor(QColor(QStringLiteral("#e70592")));
        break;
    case EDataChannel::EDC_TITemper:
        series->setColor(QColor(QStringLiteral("#2ebff5")));
        break;
    case EDataChannel::EDC_MicroPID:
        series->setColor(QColor(QStringLiteral("#f4b92f")));
        break;
    case EDataChannel::EDC_EPCPressure:
        series->setColor(QColor(QStringLiteral("#01b761")));
        break;
    default:
        break;
    }
    m_seriesInfoList.append(info);
}

void CXAChartWidget::handleDataAppend()
{
    qreal curValue = m_model->data(m_model->index(m_model->rowCount()-1,0)).toUInt();
    updateXMax(curValue);
}

void CXAChartWidget::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
    switch (marker->type())
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
        // Toggle visibility of series
        QAbstractSeries* tempSeries = marker->series();
        setSeriesVisible(tempSeries,!tempSeries->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

void CXAChartWidget::setSeriesVisible(QtCharts::QAbstractSeries *obj, bool visible)
{
    if(!obj){
        return;
    }
    obj->setVisible(visible);
    for(auto&& item : m_seriesInfoList){
        if(item.lineSeries==obj){
            QtCharts::QAbstractSeries* scatterObj = item.scatterSeries;
            scatterObj->setVisible(b_showScatterSeries?visible:b_showScatterSeries);
            //始终隐藏markers
            m_chart->legend()->markers(scatterObj).first()->setVisible(false);
            break;
        }
    }
}

CXAChartWidget::SSeriesInfo *CXAChartWidget::channelInfo(EDataChannel channel)
{
    for(auto&& item : m_seriesInfoList){
        if(item.channel ==channel){
            return &item;
        }
    }
    return nullptr;
}

void CXAChartWidget::replaceChannelData(EDataChannel channel, const QList<QPointF> &data)
{
    SSeriesInfo* tempInfo = channelInfo(channel);
    if(!tempInfo){
        return;
    }
    QLineSeries* lineSeries = qobject_cast<QLineSeries*>(tempInfo->lineSeries);
    lineSeries->replace(data);
    QScatterSeries* scatterSeries = qobject_cast<QScatterSeries*>(tempInfo->scatterSeries);
    scatterSeries->replace(data);
}

void CXAChartWidget::updateXMax(qreal curValue,bool rightnow)
{
    qreal dwidth= m_chart->plotArea().width()*1.5/(m_axisX->tickCount()); //一次滚动多少宽度
    m_x = curValue>m_x?curValue:m_x;;

    switch(m_refreshMode){
    case ERM_Dynamic:
    {
        if(m_x>m_axisX->max())
            if(rightnow)
                m_chart->scroll(dwidth, 0);
    }
        break;
    case ERM_Fixed:
    {
        if(rightnow)
            m_axisX->setRange(0,m_x);
    }
        break;
    default:
        break;
    }
}

void CXAChartWidget::updateYMax(STestData curData,bool rightnow)
{
    qreal singleStepY = m_axisY->max()/m_axisY->tickCount()/2;
    qreal maxValueY = 0;
    maxValueY = qMax((qreal)curData.MicroPIDValue , maxValueY);
    if(maxValueY>m_axisY->max()-singleStepY){
        maxValueY = maxValueY+singleStepY;
        if(rightnow)
            m_axisY->setMax(maxValueY);
        m_y = maxValueY>m_y?maxValueY:m_y;
    }
}

void CXAChartWidget::updateY2Max(STestData curData,bool rightnow)
{
    qreal singleStepY2 = m_axisY2->max()/m_axisY2->tickCount()/2;
    qreal maxValueY2 = 0;
    maxValueY2 = qMax((qreal)curData.TDCurTemperature , maxValueY2);
    maxValueY2 = qMax((qreal)curData.TICurTemperature , maxValueY2);
    maxValueY2 = qMax((qreal)curData.COLUMNTemperature , maxValueY2);
    if(maxValueY2>m_axisY2->max()-singleStepY2){
        maxValueY2 = maxValueY2+singleStepY2;
        if(rightnow)
            m_axisY2->setMax(maxValueY2);
        m_y2 = maxValueY2>m_y2?maxValueY2:m_y2;
    }
}

void CXAChartWidget::updateYMax(SSingleStatus curData)
{
    qreal singleStepY = m_axisY->max()/m_axisY->tickCount()/2;
    qreal maxValueY = 0;
    maxValueY = qMax((qreal)curData.MicroPIDValue , maxValueY);
    if(maxValueY>m_axisY->max()-singleStepY){
        maxValueY = maxValueY+singleStepY;

        m_axisY->setMax(maxValueY);
        m_y = maxValueY>m_y?maxValueY:m_y;
    }
}

void CXAChartWidget::updateY2Max(SSingleStatus curData)
{
    qreal singleStepY2 = m_axisY2->max()/m_axisY2->tickCount()/2;
    qreal maxValueY2 = 0;
    maxValueY2 = qMax((qreal)curData.TDCurTemperature , maxValueY2);
    maxValueY2 = qMax((qreal)curData.TICurTemperature , maxValueY2);
    maxValueY2 = qMax((qreal)curData.COLUMNTemperature , maxValueY2);
    maxValueY2 = qMax((qreal)curData.EPCPressure , maxValueY2);
    if(maxValueY2>m_axisY2->max()-singleStepY2){
        maxValueY2 = maxValueY2+singleStepY2;

        m_axisY2->setMax(maxValueY2);
        m_y2 = maxValueY2>m_y2?maxValueY2:m_y2;
    }
}

quint16 CXAChartWidget::handle_TestData_Read_AllInfo(const QByteArray &data)
{
    CCETestDataPackage_ReadAllInfo pack(data);
    quint16 ret = pack.isValid();
    if(ret==CCEAPI::EResult::ER_Success){
        STestData&& data1 = pack.getInfo();
        static unsigned int s_index = 0;
        s_index+=2;
        data1.curTestRunTime = s_index;
        data1.TDCurTemperature = QRandomGenerator::global()->bounded(500);
        data1.TICurTemperature = QRandomGenerator::global()->bounded(500);
        data1.COLUMNTemperature = QRandomGenerator::global()->bounded(500);
        data1.MicroPIDValue = QRandomGenerator::global()->bounded(0xFFFFFFFF);
        qobject_cast<CXATestDataTableModel*>(m_model)->appendModelData(data1);
        m_tableView->scrollToBottom();

        updateYMax(data1);
        updateY2Max(data1);
    }
    return ret;
}

quint16 CXAChartWidget::handle_SingleStatus_Read_TDCurTemperature(const QByteArray &data)
{
    CCESingleStatusPackage_ReadTDCurTemperature pack(data);
    quint16 ret = pack.isValid();
    if(ret==CCEAPI::EResult::ER_Success){
        SSingleStatus data;
        data.TDCurTemperature = pack.getCurTemperature()/10;
        qobject_cast<CXASingleStatusTableModel*>(m_model)->appendModelData(data);
        m_tableView->scrollToBottom();

        updateY2Max(data);
    }
    return ret;
}

quint16 CXAChartWidget::handle_SingleStatus_Read_TICurTemperature(const QByteArray &data)
{
    CCESingleStatusPackage_ReadTICurTemperature pack(data);
    quint16 ret = pack.isValid();
    if(ret==CCEAPI::EResult::ER_Success){
        SSingleStatus data;
        data.TICurTemperature = pack.getCurTemperature()/10;
        qobject_cast<CXASingleStatusTableModel*>(m_model)->appendModelData(data);
        m_tableView->scrollToBottom();

        updateY2Max(data);
    }
    return ret;
}

quint16 CXAChartWidget::handle_SingleStatus_Read_COLUMNCurTemperature(const QByteArray &data)
{
    CCESingleStatusPackage_ReadCOLUMNTemperature pack(data);
    quint16 ret = pack.isValid();
    if(ret==CCEAPI::EResult::ER_Success){
        SSingleStatus data;
        data.COLUMNTemperature = pack.getCurTemperature()/10;
        qobject_cast<CXASingleStatusTableModel*>(m_model)->appendModelData(data);
        m_tableView->scrollToBottom();

        updateY2Max(data);
    }
    return ret;
}

quint16 CXAChartWidget::handle_SingleStatus_Read_MicroPIDValue(const QByteArray &data)
{
    CCESingleStatusPackage_ReadMicroPIDValue pack(data);
    quint16 ret = pack.isValid();
    if(ret==CCEAPI::EResult::ER_Success){
        SSingleStatus data;
        data.MicroPIDValue = pack.getValue();
        qobject_cast<CXASingleStatusTableModel*>(m_model)->appendModelData(data);
        m_tableView->scrollToBottom();

        updateYMax(data);
    }
    return ret;
}

quint16 CXAChartWidget::handle_SingleStatus_Read_EPCPressure(const QByteArray &data)
{
    CCESingleStatusPackage_ReadEPCPressure pack(data);
    quint16 ret = pack.isValid();
    if(ret==CCEAPI::EResult::ER_Success){
        SSingleStatus data;
        data.EPCPressure = pack.getValue();
        qobject_cast<CXASingleStatusTableModel*>(m_model)->appendModelData(data);
        m_tableView->scrollToBottom();

        updateY2Max(data);
    }
    return ret;
}

void CXAChartWidget::slot_scatterSeriesVisible(bool visible)
{
    b_showScatterSeries = visible;
    for(auto&& item:m_seriesInfoList){
        QtCharts::QAbstractSeries* scatterObj = item.scatterSeries;
        if(!visible)
            scatterObj->setVisible(visible);
        else{
            //确保折线图是可见的，再显示散点图
            if(item.lineSeries->isVisible()){
                scatterObj->setVisible(visible);
            }
        }
        //始终隐藏markers
        m_chart->legend()->markers(scatterObj).first()->setVisible(false);
    }
}

void CXAChartWidget::slot_fitInView()
{
    m_axisX->setRange(0, m_x!=0?m_x:m_defalutXMax);
    m_axisY->setRange(0, m_y);
    m_axisY2->setRange(0, m_y2);
}

void CXAChartWidget::slot_checkedDynamicMode(bool checked)
{
    m_refreshMode = checked?ERM_Dynamic:ERM_Fixed;
}

void CXAChartWidget::slot_clearChart()
{
    m_x = 0;
    slot_fitInView();
    if(m_chartMode==ECM_TestData){
        qobject_cast<CXATestDataTableModel*>(m_model)->clearModelData();
    }
    else{
        qobject_cast<CXASingleStatusTableModel*>(m_model)->clearModelData();
    }
}

#include <CSVHelper/cxacsvhelper.h>
void CXAChartWidget::slot_importCSV()
{
    CXACSVHelper test;
    test.setColumnCount(5);
    QList<QStringList> list;
    test.read(list);
    if(!list.isEmpty()){
        list.pop_front();
    }
    qobject_cast<CXATestDataTableModel*>(m_model)->clearModelData();
    QList<QPointF> TDCurTemperaturePoints;
    QList<QPointF> TICurTemperaturePoints;
    QList<QPointF> COLUMNTemperaturePoints;
    QList<QPointF> MicroPIDPoints;
    for(auto&&item:list){
        STestData data;
        if(item.count()>=1){
            data.curTestRunTime = item.at(0).toUInt();
        }
        if(item.count()>=3){
            data.TDCurTemperature = item.at(2).toDouble();
            TDCurTemperaturePoints.append(QPointF(data.curTestRunTime,data.TDCurTemperature));
        }
        if(item.count()>=4){
            data.TICurTemperature = item.at(3).toDouble();
            TICurTemperaturePoints.append(QPointF(data.curTestRunTime,data.TICurTemperature));
        }
        if(item.count()>=2){
            data.COLUMNTemperature = item.at(1).toDouble();
            COLUMNTemperaturePoints.append(QPointF(data.curTestRunTime,data.COLUMNTemperature));
        }
        if(item.count()>=5){
            data.MicroPIDValue = item.at(4).toUInt();
            MicroPIDPoints.append(QPointF(data.curTestRunTime,data.MicroPIDValue));
        }

        updateXMax(data.curTestRunTime,false);
        updateYMax(data,false);
        updateY2Max(data,false);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
    replaceChannelData(EDataChannel::EDC_TDTemper,TDCurTemperaturePoints);
    replaceChannelData(EDataChannel::EDC_TITemper,TICurTemperaturePoints);
    replaceChannelData(EDataChannel::EDC_COLUMNTemper,COLUMNTemperaturePoints);
    replaceChannelData(EDataChannel::EDC_MicroPID,MicroPIDPoints);
    slot_fitInView();
}
