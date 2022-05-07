#include "cxacustomchartview.h"
#include <QApplication>
#include <QValueAxis>
#include <QDebug>

CXACustomChartView::CXACustomChartView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent)
{
    m_isPress = false;
    m_ctrlPress = false;
    m_alreadySaveRange = false;
    m_xLine = new QGraphicsLineItem(this->chart());
    m_xLine->setZValue(15);
    m_yLine = new QGraphicsLineItem(this->chart());
    m_yLine->setZValue(15);
    m_coordItem = new QGraphicsSimpleTextItem(this->chart());
    m_coordItem->setZValue(5);
    m_coordItem->setPos(20, 20);
    m_tooltipItem = new QGraphicsSimpleTextItem(this->chart());
    m_tooltipItem->setZValue(11);
    m_tooltipItem->setFont(QFont("Microsoft YaHei",10,QFont::DemiBold));

    this->setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);
    setCursor(QCursor(Qt::PointingHandCursor)); //设置鼠标指针为手指形
}

CXACustomChartView::~CXACustomChartView()
{
}

void CXACustomChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_lastPoint = event->pos();
        m_isPress = true;
    }
    QChartView::mousePressEvent(event);
}

void CXACustomChartView::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint curPos = event->pos();
    QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
    QString coordStr = QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y());
    m_coordItem->setText(coordStr);

    m_tooltipItem->setPos(event->pos().x()+20,event->pos().y()+10);

    if (m_isPress)
    {
        QPoint offset = curPos - m_lastPoint;
        m_lastPoint = curPos;
        if (!m_alreadySaveRange)
        {
            this->saveAxisRange();
            m_alreadySaveRange = true;
        }
        this->chart()->scroll(-offset.x(), offset.y());
    }

    m_xLine->setLine(event->x(),0,event->x(),this->height());
    m_yLine->setLine(0,event->y(),this->width(),event->y());

    QChartView::mouseMoveEvent(event);
}

void CXACustomChartView::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
    if (event->button() == Qt::RightButton)
    {
        if (m_alreadySaveRange)
        {
            this->chart()->axisX()->setRange(m_xMin, m_xMax);
            this->chart()->axisY()->setRange(m_yMin, m_yMax);
        }
    }
    QChartView::mouseReleaseEvent(event);
}

void CXACustomChartView::wheelEvent(QWheelEvent *event)
{
    const QPointF curPos = event->position();
    QPointF curVal;
    if (!m_alreadySaveRange)
    {
        this->saveAxisRange();
        m_alreadySaveRange = true;
    }

    const double factor = 1.5;//缩放比例
    if (m_ctrlPress)
    {//Y轴
        QList<QAbstractAxis*> axisList = this->chart()->axes(Qt::Vertical);
        for(auto&& item:axisList){
            QPointF curVal;
            QList<QAbstractSeries *> seriesList = this->chart()->series();
            for(auto&& seriesItem:seriesList){
                if(seriesItem->attachedAxes().contains(item)){
                    curVal = this->chart()->mapToValue(curPos,seriesItem);
                }
            }
            if(curVal.isNull()){
                continue;
            }
            QValueAxis *axisY = dynamic_cast<QValueAxis*>(item);
            const double yMin = axisY->min();
            const double yMax = axisY->max();
            const double yCentral = curVal.y();

            double bottomOffset;
            double topOffset;
            if (event->delta() > 0)
            {//放大
                bottomOffset = 1.0 / factor * (yCentral - yMin);
                topOffset = 1.0 / factor * (yMax - yCentral);
            }
            else
            {//缩小
                bottomOffset = 1.0 * factor * (yCentral - yMin);
                topOffset = 1.0 * factor * (yMax - yCentral);
            }

            item->setRange(yCentral - bottomOffset, yCentral + topOffset);
        }
    }
    else
    {//X轴
        curVal = this->chart()->mapToValue(curPos);
        QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
        const double xMin = axisX->min();
        const double xMax = axisX->max();
        const double xCentral = curVal.x();

        double leftOffset;
        double rightOffset;
        if (event->delta() > 0)
        {//放大
            leftOffset = 1.0 / factor * (xCentral - xMin);
            rightOffset = 1.0 / factor * (xMax - xCentral);

        }
        else
        {//缩小
            leftOffset = 1.0 * factor * (xCentral - xMin);
            rightOffset = 1.0 * factor * (xMax - xCentral);

        }

        this->chart()->axisX()->setRange(xCentral - leftOffset, xCentral + rightOffset);
    }
}

void CXACustomChartView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        m_ctrlPress = true;
    }
}

void CXACustomChartView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        m_ctrlPress = false;
    }
}

void CXACustomChartView::enterEvent(QEvent *event)
{
    m_xLine->setVisible(true);
    m_yLine->setVisible(true);
    m_tooltipItem->setVisible(true);
    m_coordItem->setVisible(true);
}

void CXACustomChartView::leaveEvent(QEvent *event)
{
    m_xLine->setVisible(false);
    m_yLine->setVisible(false);
    m_tooltipItem->setVisible(false);
    m_coordItem->setVisible(false);
}

void CXACustomChartView::keepCallout()
{

}

void CXACustomChartView::tooltip(QPointF point, bool state)
{
    if(state){
        QAbstractSeries* senderObj = qobject_cast<QAbstractSeries*>(this->sender());
        QPointF curVal = this->chart()->mapToValue(point,senderObj);
        m_tooltipItem->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltipItem->show();
    }
    else{
        m_tooltipItem->hide();
    }
}

void CXACustomChartView::saveAxisRange()
{
    QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
    m_xMin = axisX->min();
    m_xMax = axisX->max();
    QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
    m_yMin = axisY->min();
    m_yMax = axisY->max();

}

