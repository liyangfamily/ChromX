#ifndef CXACSVHELPER_H
#define CXACSVHELPER_H

#include <QString>
#include <QList>

class CXACSVHelper
{
public:
    CXACSVHelper();
    CXACSVHelper(int colCount, QString filePath);

    void setColumnCount(int colCount);
    void setFilePath(QString filePath);

    bool read(QList<QStringList>& list);
    bool write(const QList<QStringList>& list);

private:
    int m_columnCount = 0;
    QString m_filePath;
    QList<QStringList> m_data;
};

#endif // CXACSVHELPER_H
