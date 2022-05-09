#include "cxasinglestatustablemodel.h"
#include <QColor>

CXASingleStatusTableModel::CXASingleStatusTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    m_columnCount = 6;
    m_rowCount = 0;
}

QVariant CXASingleStatusTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return "Index";
        else if(section == 1)
            return "TD Temper";
        else if(section == 2)
            return "TI Temper";
        else if(section == 3)
            return "COLUMN Temper";
        else if(section == 4)
            return "Dector Value";
        else
            return "EPC Value";
    } else {
        return QString("%1").arg(section + 1);
    }
}

int CXASingleStatusTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

int CXASingleStatusTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_columnCount;
}

QVariant CXASingleStatusTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if(index.column()==0){
            return index.row();
        }
        else{
            return m_data[index.row()].getModelData(index.column());
        }
    } else if (role == Qt::EditRole) {
        if(index.column()==0){
            return index.row();
        }
        else{
            return m_data[index.row()].getModelData(index.column());
        }
    } else if (role == Qt::BackgroundRole) {
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CXASingleStatusTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if(index.column()!=0){
            m_data[index.row()].setModelData(index.column(), value);
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool CXASingleStatusTableModel::insertModelData(int row, const SSingleStatus &data)
{
    if(row<0||row>rowCount()){
        return false;
    }
    beginInsertRows(QModelIndex(),row,row);
    m_data.insert(row,data);
    endInsertRows();
    return true;
}

bool CXASingleStatusTableModel::appendModelData(const SSingleStatus &data)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_data.append(data);
    endInsertRows();
    return true;
}

void CXASingleStatusTableModel::clearModelData()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void CXASingleStatusTableModel::setModelData(const QList<SSingleStatus> &datas)
{
    beginResetModel();
    m_data = datas;
    endResetModel();
}

Qt::ItemFlags CXASingleStatusTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
