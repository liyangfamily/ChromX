#ifndef CXACHARTWIDGET_H
#define CXACHARTWIDGET_H

#include <QTimer>
#include <QWidget>
#include <QtCharts/QChart>

namespace Ui {
class CXATestParamSetChart;
}
class QAbstractTableModel;
class CXACustomChartView;
class QTableView;
class QHBoxLayout;
QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QSplineSeries;
class QScatterSeries;
class QValueAxis;
class QChartView;
QT_CHARTS_END_NAMESPACE
class CXAChartWidget : public QWidget
{
    Q_OBJECT

public:
    enum EChartMode{
        ECM_TestData,
        ECM_SingleStatus,
    };
    enum ERefreshMode{
        ERM_Dynamic,
        ERM_Fixed,
    };
    enum EDataChannel{
        EDC_TDTemper,
        EDC_TITemper,
        EDC_COLUMNTemper,
        EDC_MicroPID,
        EDC_EPCPressure,
        EDC_Null = 0xFF,
    };
    explicit CXAChartWidget(EChartMode mode = ECM_TestData, QWidget *parent = nullptr);
    ~CXAChartWidget();
public:
    void setRefreshMode(ERefreshMode mode){
        m_refreshMode = mode;
    };
    void setChannelVisible(EDataChannel channel,bool visible);
protected:
    bool event(QEvent* e) override;
private:
    Ui::CXATestParamSetChart *ui;
    QHBoxLayout *m_toolBtnLayout = nullptr;
    QtCharts::QChart *m_chart = nullptr;
    CXACustomChartView *m_chartView = nullptr;
    QMap<QtCharts::QAbstractSeries*,QtCharts::QAbstractSeries*> m_seriesPairMap;

    QTableView *m_tableView = nullptr;
    QAbstractTableModel *m_model = nullptr;

    QtCharts::QValueAxis *m_axisX;
    QtCharts::QValueAxis *m_axisY2;
    QtCharts::QValueAxis *m_axisY;
    qreal m_x;
    qreal m_y;
    qreal m_y2;
    QTimer m_timer;

    bool b_showScatterSeries = false;
    EChartMode m_chartMode = ECM_TestData;
    ERefreshMode m_refreshMode = ERM_Dynamic;
    struct SSeriesInfo{
        QString name;
        EDataChannel channel = EDC_Null;
        QtCharts::QAbstractSeries* lineSeries = nullptr;
        QtCharts::QAbstractSeries* scatterSeries = nullptr;
    };
    QList<SSeriesInfo> m_seriesInfoList;
private:
    void initUI();
    void initToolButton();
    void initChart();
    void initModel();

    void addChannel(EDataChannel channel, QString name,int modelXCol,int modelYCol,QtCharts::QValueAxis* x,QtCharts::QValueAxis* y);
    void handleDataAppend();
    void handleMarkerClicked();

    void setSeriesVisible(QtCharts::QAbstractSeries* obj,bool visible);

    quint16 handle_SingleStatus_Read_TDCurTemperature(const QByteArray & data);
    quint16 handle_SingleStatus_Read_TICurTemperature(const QByteArray & data);
    quint16 handle_SingleStatus_Read_COLUMNCurTemperature(const QByteArray & data);
    quint16 handle_SingleStatus_Read_MicroPIDValue(const QByteArray & data);
    quint16 handle_SingleStatus_Read_EPCPressure(const QByteArray & data);
public slots:
    void slot_scatterSeriesVisible(bool visible);
    void slot_fitInView();
    void slot_checkedDynamicMode(bool checked);
    void slot_clearChart();
};

#endif // CXACHARTWIDGET_H
