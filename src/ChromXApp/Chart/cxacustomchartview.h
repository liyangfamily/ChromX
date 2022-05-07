#ifndef CXACUSTOMCHARTVIEW_H
#define CXACUSTOMCHARTVIEW_H

#include <QChartView>
QT_CHARTS_USE_NAMESPACE

class CXACustomChartView : public QChartView
{
public:
    CXACustomChartView(QChart *chart, QWidget *parent = nullptr);
    ~CXACustomChartView();
    // 保存坐标区域，用于复位
    void saveAxisRange();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);


private:
    QPoint m_lastPoint;
    bool m_isPress;
    bool m_ctrlPress;
    bool m_alreadySaveRange;
    double m_xMin, m_xMax, m_yMin, m_yMax;
    QGraphicsSimpleTextItem* m_coordItem;
    QGraphicsSimpleTextItem* m_tooltipItem =nullptr;
    QGraphicsLineItem* m_xLine = nullptr;
    QGraphicsLineItem* m_yLine = nullptr;

public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);
};

#endif // CXACUSTOMCHARTVIEW_H
