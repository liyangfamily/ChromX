#include "cxatestdatatablemodel.h"
#include <QColor>

CXATestDataTableModel::CXATestDataTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_columnCount = 5;
    m_rowCount = 0;
}

QVariant CXATestDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return "Run Time";
        else if(section == 1)
            return "TD Temper";
        else if(section == 2)
            return "TI Temper";
        else if(section == 3)
            return "COLUMN Temper";
        else
            return "Dector Value";
    } else {
        return QString("%1").arg(section + 1);
    }
}

int CXATestDataTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

int CXATestDataTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_columnCount;
}

QVariant CXATestDataTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return colData(index);
    } else if (role == Qt::EditRole) {
        return colData(index);
    } else if (role == Qt::BackgroundRole) {
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CXATestDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        setColData(index,value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool CXATestDataTableModel::insertModelData(int row, const SUiTestData &data)
{
    if(row<0||row>rowCount()){
        return false;
    }
    bool alredyExist = m_data.contains(row);
    auto it = m_data.insert(row,data);
    ulong pos = 0;
    while(it!=m_data.begin()){
        pos++;
        it--;
    }
    if(!alredyExist){
        beginInsertRows(QModelIndex(),pos,pos);
        endInsertRows();
    }
    emit dataChanged(this->index(pos,0,QModelIndex()),this->index(pos,m_columnCount-1,QModelIndex()));
    return true;
}

bool CXATestDataTableModel::appendModelData(const SUiTestData &data)
{
    if(m_data.contains(data.curTestRunTime)){
        insertModelData(data.curTestRunTime, data);
        return true;
    }
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_data.insert(data.curTestRunTime,data);
    endInsertRows();
    return true;
}

void CXATestDataTableModel::clearModelData()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void CXATestDataTableModel::setModelData(const QList<SUiTestData> &datas,bool blockSignal)
{
    m_data.clear();
    for(auto&& item:datas){
        m_data.insert(item.curTestRunTime,item);
    }
    if(!blockSignal){
        beginResetModel();
        endResetModel();
    }
}

QList<SUiTestData> CXATestDataTableModel::modelData() const
{
    return m_data.values();
}

Qt::ItemFlags CXATestDataTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CXATestDataTableModel::adjustModelData()
{
    beginResetModel();
    endResetModel();
}

void CXATestDataTableModel::setColData(const QModelIndex &index, const QVariant &value)
{
    switch(index.column()){
    case 0:
        (*(m_data.begin()+index.row())).curTestRunTime = value.toInt();
        break;
    case 1:
        (*(m_data.begin()+index.row())).TDCurTemperature = value.toDouble();
        break;
    case 2:
        (*(m_data.begin()+index.row())).TICurTemperature = value.toDouble();
        break;
    case 3:
        (*(m_data.begin()+index.row())).COLUMNTemperature = value.toDouble();
        break;
    case 4:
        (*(m_data.begin()+index.row())).MicroPIDValue = value.toDouble();
        break;
    default:
        break;
    }
}

QVariant CXATestDataTableModel::colData(const QModelIndex &index) const
{
    switch(index.column()){
    case 0:
        return (*(m_data.begin()+index.row())).curTestRunTime;
    case 1:
        return (*(m_data.begin()+index.row())).TDCurTemperature;
    case 2:
        return (*(m_data.begin()+index.row())).TICurTemperature;
    case 3:
        return (*(m_data.begin()+index.row())).COLUMNTemperature;
    case 4:
        return (*(m_data.begin()+index.row())).MicroPIDValue;
    default:
        return QVariant();
    }
}
