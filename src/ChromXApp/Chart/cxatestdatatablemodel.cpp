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
        return (*(m_data.begin()+index.row())).getModelData(index.column());
        //return m_data[index.row()].getModelData(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()].getModelData(index.column());
    } else if (role == Qt::BackgroundRole) {
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CXATestDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        (*(m_data.begin()+index.row())).setModelData(index.column(), value);
        //m_data[index.row()].setModelData(index.column(), value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool CXATestDataTableModel::insertModelData(int row, const STestData &data)
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

bool CXATestDataTableModel::appendModelData(const STestData &data)
{
    if(m_data.contains(data.curTestRunTime)){
        insertModelData(data.curTestRunTime, data);
        return true;
    }
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    //m_data.append(data);
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

void CXATestDataTableModel::setModelData(const QList<STestData> &datas)
{
    beginResetModel();
    //m_data = datas;
    endResetModel();
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
