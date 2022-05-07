#ifndef CXASINGLESTATUSTABLEMODEL_H
#define CXASINGLESTATUSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include <CCE_CommunicatEngine/CCEInteCtrlPackageStruct.h>

class CXASingleStatusTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CXASingleStatusTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertModelData(int row,const SSingleStatus& data);
    bool appendModelData(const SSingleStatus& data);

    void clearModelData();
    void setModelData(const QList<SSingleStatus>& datas);

    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QList<SSingleStatus> m_data;
    int m_columnCount;
    int m_rowCount;
};

#endif // CXASINGLESTATUSTABLEMODEL_H
