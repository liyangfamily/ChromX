#include "cxachartwidget.h"
#include "ui_cxatestparamsetchart.h"

#include "cxatestdatatablemodel.h"
#include "cxasinglestatustablemodel.h"
#include "cxacustomchartview.h"

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

CXAChartWidget::CXAChartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CXATestParamSetChart),
    m_axisX(new QValueAxis()),
    m_axisY2(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_x(0),
    m_y(1)
{
    ui->setupUi(this);
    initUI();
}

CXAChartWidget::~CXAChartWidget()
{
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
}

void CXAChartWidget::initChart()
{
    m_seriesPairMap.clear();
    m_seriesInfoList.clear();
    //init axis
    m_axisX->setTickCount(10);
    m_axisX->setRange(0, 100);
    m_axisY2->setTickCount(10);
    m_axisY2->setRange(0, 500);
    m_axisY->setTickCount(10);
    m_axisY->setRange(0, 65535);

    //init chart
    m_chart = new QChart();
    m_chart->setTitle("Simple line chart example");
    m_chart->addAxis(m_axisX,Qt::AlignBottom);
    m_chart->addAxis(m_axisY,Qt::AlignLeft);
    m_chart->addAxis(m_axisY2,Qt::AlignRight);

    //init chartView
    m_chartView = new CXACustomChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    //add channel
    //channel 1
    addChannel(EDC_TDTemper,"TD Temper",0,1,m_axisX,m_axisY);
    //channel 2
    addChannel(EDC_TITemper,"TI Temper",0,2,m_axisX,m_axisY);
    //channel 3
    addChannel(EDC_COLUMNTemper,"COLUMN Temper",0,3,m_axisX,m_axisY);
    //channel 4
    addChannel(EDC_MicroPID,"Micro PID",0,4,m_axisX,m_axisY2);
    //channel 5
    addChannel(EDC_EPCPressure,"EPC Pressure",0,5,m_axisX,m_axisY);

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
    //m_model = new CXASingleStatusTableModel;
    m_model = new CXATestDataTableModel;
    m_tableView = new QTableView;
    m_tableView->setModel(m_model);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(&m_timer,&QTimer::timeout,this,[=]{
        STestData data;
        static unsigned int s_index = 0;
        data.curTestRunTime = s_index++;
        data.TDCurTemperature = QRandomGenerator::global()->bounded(0xFFFF);
        data.TICurTemperature = QRandomGenerator::global()->bounded(0xFFFF);
        data.COLUMNTemperature = QRandomGenerator::global()->bounded(0xFFFF);
        data.MicroPIDValue = QRandomGenerator::global()->bounded(500);
//        SSingleStatus data;
//        data.TDCurTemperature = QRandomGenerator::global()->bounded(0xFFFF);
//        data.TICurTemperature = QRandomGenerator::global()->bounded(0xFFFF);
//        data.COLUMNTemperature = QRandomGenerator::global()->bounded(0xFFFF);
//        data.MicroPIDValue = QRandomGenerator::global()->bounded(500);
//        data.EPCPressure = QRandomGenerator::global()->bounded(0xFFFF);
        qobject_cast<CXATestDataTableModel*>(m_model)->appendModelData(data);
        m_tableView->scrollToBottom();
    });
    m_timer.start(100);
}

void CXAChartWidget::addChannel(EDataChannel channel, QString name, int modelXCol, int modelYCol, QtCharts::QValueAxis *x, QtCharts::QValueAxis *y)
{
    //折线图
    QLineSeries* series = new QLineSeries;
    series->setName(name);
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
    mapper = new QVXYModelMapper(this);
    mapper->setXColumn(modelXCol);
    mapper->setYColumn(modelYCol);
    mapper->setSeries(scatterSeries);
    mapper->setModel(m_model);
    m_chart->addSeries(scatterSeries);
    scatterSeries->attachAxis(x);
    scatterSeries->attachAxis(y);
    connect(scatterSeries, &QScatterSeries::hovered, m_chartView, &CXACustomChartView::tooltip);
    //添加到pair
    m_seriesPairMap.insert(series,scatterSeries);
    SSeriesInfo info;
    info.name = name;
    info.channel = channel;
    info.lineSeries = series;
    info.scatterSeries = scatterSeries;
    m_seriesInfoList.append(info);
}

void CXAChartWidget::handleDataAppend()
{
    qreal dwidth= m_chart->plotArea().width()/(m_axisX->tickCount()); //一次滚动多少宽度
    qreal dx= 10/(m_axisX->tickCount()); //横坐标偏移量

    m_x += dx;

    switch(m_refreshMode){
    case ERM_Dynamic:
    {
        if(m_x>m_axisX->max())
            m_chart->scroll(dwidth, 0);
    }
        break;
    case ERM_Fixed:
    {
        m_axisX->setRange(0,m_x);
    }
        break;
    default:
        break;
    }

    if(m_model->rowCount()>50){
        m_timer.stop();
        setChannelVisible(EDataChannel::EDC_TDTemper,false);
        setChannelVisible(EDataChannel::EDC_TITemper,false);
        setChannelVisible(EDataChannel::EDC_COLUMNTemper,false);
        setChannelVisible(EDataChannel::EDC_MicroPID,false);
    }
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
    if(m_seriesPairMap.contains(obj)){
        QtCharts::QAbstractSeries* scatterObj = m_seriesPairMap.value(obj);
        scatterObj->setVisible(b_showScatterSeries?visible:b_showScatterSeries);
        m_chart->legend()->markers(scatterObj).first()->setVisible(false);
    }
}

void CXAChartWidget::slot_scatterSeriesVisible(bool visible)
{
    b_showScatterSeries = visible;
    for(auto&& item:m_seriesPairMap.keys()){
        QtCharts::QAbstractSeries* scatterObj = m_seriesPairMap.value(item);
        if(!visible)
            scatterObj->setVisible(visible);
        else{
            //确保折线图是可见的，再显示散点图
            if(item->isVisible()){
                scatterObj->setVisible(visible);
            }
        }
        m_chart->legend()->markers(scatterObj).first()->setVisible(false);
    }
}

void CXAChartWidget::slot_fitInView()
{
    m_axisX->setRange(0, m_x);
    m_axisY2->setRange(0, 500);
    m_axisY->setRange(0, 65535);
}

void CXAChartWidget::slot_checkedDynamicMode(bool checked)
{
    m_refreshMode = checked?ERM_Dynamic:ERM_Fixed;
}
