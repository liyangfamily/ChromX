#ifndef CXATESTDATATABLEMODEL_H
#define CXATESTDATATABLEMODEL_H

#include <QAbstractTableModel>
#include <CCE_CommunicatEngine/CCEInteCtrlPackageStruct.h>
#include <TransUiValueConvertor/cxatestdataconvertor.h>

class CXATestDataTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CXATestDataTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertModelData(int row,const SUiTestData& data);
    bool appendModelData(const SUiTestData& data);

    void clearModelData();
    void setModelData(const QList<SUiTestData>& datas,bool blockSignal = false);
    QList<SUiTestData> modelData() const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void adjustModelData();
private:
    void setColData(const QModelIndex &index, const QVariant &value);
    QVariant colData(const QModelIndex &index) const;
private:
    //QList<STestData> m_data;
    QMap<quint32,SUiTestData> m_data;
    int m_columnCount;
    int m_rowCount;
};

#endif // CXATESTDATATABLEMODEL_H
