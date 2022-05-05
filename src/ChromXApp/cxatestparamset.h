#ifndef CXATESTPARAMSET_H
#define CXATESTPARAMSET_H

#include <QWidget>
#include <JsonHelper/cxajsontestparamset.h>
#include <QScopedPointer>
namespace Ui {
class CXATestParamSet;
}
class QTableWidget;
class CXAJsonTestParamSet;
class CXATestParamSet : public QWidget
{
    Q_OBJECT

public:
    explicit CXATestParamSet(QWidget *parent = nullptr);
    ~CXATestParamSet();

private:
    enum ETableCol
        {
            ECol_PIDTimeValue = 0,
            ECol_PIDTemperValue,
            ECol_PIDTimeDifferenceValue,
            ECol_PIDTemperRate,
            ECol_TimeValue,
            ECol_TimePWMValue,
        };
    Ui::CXATestParamSet *ui;
    QScopedPointer<CXAJsonTestParamSet> m_jsonTestParamSet;

public slots:
    void on_btn_ToChart_clicked();
    void on_btn_ChartBack_clicked();
    void on_btn_PIDSetRead_clicked();
    void on_btn_SaveASDefault_clicked();
    void on_btn_ImportTemplate_clicked();
    void tableWidgetCellChanged(int row, int column);

private:
    void initUI();
    void initSignalAndSlot();
    bool importTemplateFile();
    bool exportTemplateFile();

    void updateUIFromJson();
    void updateJsonFromUI();

    void showTableValueFromJson(QTableWidget* table,STimeCtrl* timeCtrl ,SPIDCtrl* PIDCtrl,int rowSize,bool autoPID);
    void getTableValueToJson(QTableWidget* table,STimeCtrl* timeCtrl ,SPIDCtrl* PIDCtrl,int rowSize);
    void computTablePIDValue(QTableWidget* table,int rowIndex = -1);
    void changeTabelShowMode(QTableWidget* table,bool autoPID);
};


#include <QSpinBox>
#include <QItemDelegate>

class  ReadOnlyDelegate : public  QItemDelegate
{
    Q_OBJECT
public:
    ReadOnlyDelegate(QObject *parent = 0) : QItemDelegate(parent) { }
    QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,
        const  QModelIndex &index)  const
    {
        return  NULL;
    }
};

class  SpinBoxDelegate : public  QItemDelegate
{
    Q_OBJECT
public:
    SpinBoxDelegate(QObject *parent = 0) : QItemDelegate(parent) { }
    SpinBoxDelegate(int min,int max,QObject *parent = 0) :
        QItemDelegate(parent),m_min(min),m_max(max) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const  QModelIndex &index)  const
    {
        QSpinBox *editor = new  QSpinBox(parent);
        editor->setAlignment(Qt::AlignmentFlag::AlignHCenter);
        editor->setMinimum(m_min);
        editor->setMaximum(m_max);
        return  editor;
    }
    void  setEditorData(QWidget *editor, const  QModelIndex &index)  const
    {
        int  value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast <QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
        const  QModelIndex &index)  const
    {
        QSpinBox *spinBox = static_cast <QSpinBox*>(editor);
        spinBox->interpretText();
        int  value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor,
        const  QStyleOptionViewItem &option, const  QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }

private:
    int m_min = 0;
    int m_max = 0xFFFF;
};

class  DoubleSpinBoxDelegate : public  QItemDelegate
{
    Q_OBJECT
public:
    DoubleSpinBoxDelegate(QObject *parent = 0) : QItemDelegate(parent) { }
    DoubleSpinBoxDelegate(double min,double max,QObject *parent = 0) :
        QItemDelegate(parent),m_min(min),m_max(max) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const  QModelIndex &index)  const
    {
        QDoubleSpinBox *editor = new  QDoubleSpinBox(parent);
        editor->setDecimals(1);
        editor->setAlignment(Qt::AlignmentFlag::AlignHCenter);
        editor->setMinimum(m_min);
        editor->setMaximum(m_max);
        return  editor;
    }
    void  setEditorData(QWidget *editor, const  QModelIndex &index)  const
    {
        double  value = index.model()->data(index, Qt::EditRole).toDouble();
        QDoubleSpinBox *spinBox = static_cast <QDoubleSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
        const  QModelIndex &index)  const
    {
        QDoubleSpinBox *spinBox = static_cast <QDoubleSpinBox*>(editor);
        spinBox->interpretText();
        double  value = spinBox->value();
        model->setData(index, QString::number(value,'f',1), Qt::EditRole);
    }
    void  updateEditorGeometry(QWidget *editor,
        const  QStyleOptionViewItem &option, const  QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }

private:
    double m_min = 0;
    double m_max = 0xFFFF;
};
#endif // CXATESTPARAMSET_H
