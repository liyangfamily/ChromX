#include "cxacsvhelper.h"

#include "app.h"
#include "icore.h"

#include <CCE_CommunicatEngine/CCEPackageDispatcher>

#include <QFile>
#include <QFileDialog>

CXACSVHelper::CXACSVHelper()
{

}

CXACSVHelper::CXACSVHelper(int colCount, QString filePath):
    m_columnCount(colCount),
    m_filePath(filePath)
{

}

void CXACSVHelper::setColumnCount(int colCount)
{
    m_columnCount = colCount;
}

void CXACSVHelper::setFilePath(QString filePath)
{
    m_filePath = filePath;
}

bool CXACSVHelper::read(QList<QStringList>& list)
{
    if(!openFile()){
        return false;
    }
    QFile file(m_filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Open Excel file failed!";
        return false;
    }
    QStringList tempList;
    QTextStream in(&file);
    in.setCodec("UTF-8");
    //遍历行
    for(int i = 0; !in.atEnd(); ++i){
        QString fileLine = in.readLine() ;
        tempList = fileLine.split(QLatin1Char(','), QString::KeepEmptyParts);
        tempList = tempList.mid(0,m_columnCount);
        //if(i > 0){//这里的目的是第一行的表项不赋值
            list.append(tempList);
            //qDebug() << tempList;
        //}
    }

    file.close();
    return true;
}

bool CXACSVHelper::write(const QList<QStringList>& list)
{
    if(!saveFile()){
        return false;
    }
    QFile file(m_filePath);
    //以只写方式打开，完全重写数据
    if (file.open(QIODevice::WriteOnly)){
        for(auto&& item:list){
            QString tempStr;
            for (int i = 0; i < item.size(); ++i){
                tempStr.append(item.at(i));
                if(i<(item.size()-1)){
                    tempStr.append(',');
                }else{
                    tempStr.append('\n');
                }
            }
            file.write(tempStr.toStdString().c_str());//写入每一行数据到文件
        }
        file.close();
    }
    return true;
}

bool CXACSVHelper::read(QList<SUiTestData> &list)
{
    if(!openFile()){
        return false;
    }
    setColumnCount(5);
    QFile file(m_filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Open Excel file failed!";
        return false;
    }
    QStringList tempList;
    QTextStream in(&file);
    in.setCodec("UTF-8");
    //遍历行
    for(int i = 1; !in.atEnd(); ++i){
        QString fileLine = in.readLine() ;
        tempList = fileLine.split(QLatin1Char(','), QString::KeepEmptyParts);
        tempList = tempList.mid(0,m_columnCount);

        SUiTestData uiTestData;
        if(tempList.count()>=1){
            uiTestData.curTestRunTime = tempList.at(0).toUInt();
        }
        if(tempList.count()>=3){
            uiTestData.TDCurTemperature = tempList.at(2).toDouble();
        }
        if(tempList.count()>=4){
            uiTestData.TICurTemperature = tempList.at(3).toDouble();
        }
        if(tempList.count()>=2){
            uiTestData.COLUMNTemperature = tempList.at(1).toDouble();
        }
        if(tempList.count()>=5){
            uiTestData.MicroPIDValue = tempList.at(4).toDouble();
        }

        list.append(uiTestData);
    }

    file.close();
    return true;
}

bool CXACSVHelper::write(const QList<SUiTestData> &list)
{
    if(!saveFile()){
        return false;
    }
    setColumnCount(5);
    QFile file(m_filePath);
    //以只写方式打开，完全重写数据
    if (file.open(QIODevice::WriteOnly)){
        const QString header = QStringLiteral("TimeStamp,TempCol,TempTD,TempTI,PID/HDPID_ADC\n");
        file.write(header.toStdString().c_str());//写入每一行数据到文件

        for(auto&& item:list){
            QString tempStr;
            tempStr = QString("%1,%2,%3,%4,%5\n").arg(item.curTestRunTime)\
                    .arg(QString::number(item.COLUMNTemperature,'f',1))\
                    .arg(QString::number(item.TDCurTemperature,'f',1))\
                    .arg(QString::number(item.TICurTemperature,'f',1))\
                    .arg(QString::number(item.MicroPIDValue));
            file.write(tempStr.toStdString().c_str());//写入每一行数据到文件
        }
        file.close();
    }
    return true;
}

bool CXACSVHelper::openFile()
{
    if(m_filePath.isEmpty()){
        QString selectFilter;
        m_filePath = QFileDialog::getOpenFileName(ICore::mainWindow() ,QObject::tr("Excel file"),
                                                  App::lastOpenPath,
                                                  QString("%1").arg("Files (*.csv)"),
                                                  &selectFilter);
        if (m_filePath.isEmpty()) {
            return false;
        }
        App::lastOpenPath = m_filePath;
        App::writeConfig();
    }
    if(m_filePath.isEmpty()){
        qDebug() << "Excel file is empty!";
        return false;
    }
    return true;
}

bool CXACSVHelper::saveFile()
{
    if(m_filePath.isEmpty()){
        QString selectFilter;
        m_filePath = QFileDialog::getSaveFileName(ICore::mainWindow() ,
                                                          QObject::tr("Excel file"),
                                                          App::lastOpenPath,
                                                          QString("%1").arg("Files (*.csv)"),
                                                          &selectFilter);
        if (m_filePath.isEmpty()) {
            return false;
        }
        App::lastOpenPath = m_filePath;
        App::writeConfig();
    }
    if(m_filePath.isEmpty()){
        qDebug() << "Excel file is empty!";
        return false;
    }
    return true;
}
