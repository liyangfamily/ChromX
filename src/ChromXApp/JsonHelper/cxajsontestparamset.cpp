#include "cxajsontestparamset.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

CXAJsonTimeCtrl::CXAJsonTimeCtrl(const STimeCtrl &value, int index):
    m_data(value),m_index(index)
{

}

void CXAJsonTimeCtrl::setTimeCtrl(const STimeCtrl &value, int index)
{
    m_data = value;
    m_index = index;
}

void CXAJsonTimeCtrl::resetJsonData()
{
    m_index = 0;
    m_data.clear();
}

bool CXAJsonTimeCtrl::buildJson(QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject TimeCtrlObj;
    TimeCtrlObj["index"]        = m_index;
    TimeCtrlObj["TimeValue"]    = m_data.timeValue;
    TimeCtrlObj["PWMValue"]     = m_data.PWMValue;

    //设置doc
    doc.setObject(TimeCtrlObj);
    return true;
}

bool CXAJsonTimeCtrl::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& TimeCtrlObj = doc.object();
    m_index             = TimeCtrlObj["index"].toInt();
    m_data.timeValue    = TimeCtrlObj["TimeValue"].toInt();
    m_data.PWMValue     = TimeCtrlObj["PWMValue"].toInt();
    return true;
}

CXAJsonPIDCtrl::CXAJsonPIDCtrl(const SPIDCtrl &value, int index):
    m_data(value),m_index(index)

{

}

void CXAJsonPIDCtrl::setPIDTimeCtrl(const SPIDCtrl &value, int index)
{
    m_data = value;
    m_index = index;
}

void CXAJsonPIDCtrl::resetJsonData()
{
    m_index = 0;
    m_data.clear();
}

bool CXAJsonPIDCtrl::buildJson(QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject PIDCtrlObj;
    PIDCtrlObj["index"]             = m_index;
    PIDCtrlObj["TimeValue"]         = m_data.timeValue;
    PIDCtrlObj["TemperatureValue"]  = m_data.temperatureValue;
    PIDCtrlObj["PWMValue_Max"]      = m_data.PWMValue_Max;
    PIDCtrlObj["PWMValue_Min"]      = m_data.PWMValue_Min;

    //设置doc
    doc.setObject(PIDCtrlObj);
    return true;
}

bool CXAJsonPIDCtrl::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& PIDCtrlObj = doc.object();
    m_index                     = PIDCtrlObj["index"].toInt();
    m_data.timeValue            = PIDCtrlObj["TimeValue"].toInt();
    m_data.temperatureValue     = PIDCtrlObj["TemperatureValue"].toInt();
    m_data.PWMValue_Max         = PIDCtrlObj["PWMValue_Max"].toInt();
    m_data.PWMValue_Min         = PIDCtrlObj["PWMValue_Min"].toInt();
    return true;
}


CXAJsonTDCtrl::CXAJsonTDCtrl(const STDCtrl &value):
    m_data(value)
{

}

void CXAJsonTDCtrl::resetJsonData()
{
    m_data.clear();
}

bool CXAJsonTDCtrl::buildJson(QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject jsonTDCtrlObj;
    jsonTDCtrlObj["BeforeTDStartup_TestPCG"] = m_data.BeforeTDStartup_TestPCG;
    jsonTDCtrlObj["TDStart_CarrierPressure_UpLimit"] = m_data.TDStart_CarrierPressure_UpLimit;
    jsonTDCtrlObj["TDStart_CarrierPressure_LowLimit"] = m_data.TDStart_CarrierPressure_LowLimit;
    jsonTDCtrlObj["BeforeTDStartup_TestTITemperature"] = m_data.BeforeTDStartup_TestTITemperature;
    jsonTDCtrlObj["BeforeTDStartup_TITemperature_Max"] = m_data.BeforeTDStartup_TITemperature_Max;
    jsonTDCtrlObj["ControlMode"] = m_data.controlMode;

    // build TimeCtrl
    QJsonArray jsonTimeCtrlArray;
    int index = 0;
    for (auto &&item : m_data.timeCtrlArray){
        QJsonDocument tempDoc;
        CXAJsonTimeCtrl jsonTimeCtrl(item,index++);
        if (jsonTimeCtrl.buildJson(tempDoc)){
            if (tempDoc.isObject()){
                jsonTimeCtrlArray.append(tempDoc.object());
            }
        }
    }
    jsonTDCtrlObj["TimeCtrl"] = jsonTimeCtrlArray;

    // build PIDTimeCtrl
    QJsonArray jsonPIDCtrlArray;
    index = 0;
    for (auto &&item : m_data.PIDCtrlArray){
        QJsonDocument tempDoc;
        CXAJsonPIDCtrl jsonPIDCtrl(item,index++);
        if (jsonPIDCtrl.buildJson(tempDoc)){
            if (tempDoc.isObject()){
                jsonPIDCtrlArray.append(tempDoc.object());
            }
        }
    }
    jsonTDCtrlObj["PIDCtrl"] = jsonPIDCtrlArray;

    //设置doc
    doc.setObject(jsonTDCtrlObj);
    return true;
}

bool CXAJsonTDCtrl::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& jsonTDCtrlObj = doc.object();

    m_data.BeforeTDStartup_TestPCG             =       jsonTDCtrlObj["BeforeTDStartup_TestPCG"].toInt();
    m_data.TDStart_CarrierPressure_UpLimit     =       jsonTDCtrlObj["TDStart_CarrierPressure_UpLimit"].toInt();
    m_data.TDStart_CarrierPressure_LowLimit    =       jsonTDCtrlObj["TDStart_CarrierPressure_LowLimit"].toInt();
    m_data.BeforeTDStartup_TestTITemperature   =       jsonTDCtrlObj["BeforeTDStartup_TestTITemperature"].toInt();
    m_data.BeforeTDStartup_TITemperature_Max   =       jsonTDCtrlObj["BeforeTDStartup_TITemperature_Max"].toInt();
    m_data.controlMode                         =       jsonTDCtrlObj["ControlMode"].toInt();

    QJsonArray jsonTimeCtrlArray = jsonTDCtrlObj["TimeCtrl"].toArray();
    int index = 0;
    for (auto &&item : jsonTimeCtrlArray){
        QJsonDocument tempDoc;
        tempDoc.setObject(item.toObject());
        CXAJsonTimeCtrl jsonTimeCtrl;
        jsonTimeCtrl.parseJson(tempDoc,version);
        index = jsonTimeCtrl.index();
        if(index<sizeof(m_data.timeCtrlArray)/sizeof(m_data.timeCtrlArray[0])){
            m_data.timeCtrlArray[index] = jsonTimeCtrl.TimeCtrl();
        }
    }

    QJsonArray jsonPIDCtrlArray = jsonTDCtrlObj["PIDCtrl"].toArray();
    for (auto &&item : jsonPIDCtrlArray){
        QJsonDocument tempDoc;
        tempDoc.setObject(item.toObject());
        CXAJsonPIDCtrl jsonPIDCtrl;
        jsonPIDCtrl.parseJson(tempDoc,version);
        index = jsonPIDCtrl.index();
        if(index<sizeof(m_data.PIDCtrlArray)/sizeof(m_data.PIDCtrlArray[0])){
            m_data.PIDCtrlArray[index] = jsonPIDCtrl.PIDTimeCtrl();
        }
    }
    return true;
}

CXAJsonTICtrl::CXAJsonTICtrl(const STICtrl &value):
    m_data(value)
{

}

void CXAJsonTICtrl::setTICtrl(const STICtrl &value)
{
    m_data = value;
}

void CXAJsonTICtrl::resetJsonData()
{
    m_data.clear();
}

bool CXAJsonTICtrl::buildJson(QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject jsonTICtrlObj;
    jsonTICtrlObj["BeforeTIStartup_TestMicroPID"]         = m_data.BeforeTIStartup_TestMicroPID;
    jsonTICtrlObj["BeforeTIStartup_MicroPIDValue_Min"]    = (double)m_data.BeforeTIStartup_MicroPIDValue_Min;
    jsonTICtrlObj["ControlMode"] = m_data.controlMode;

    // build TimeCtrl
    QJsonArray jsonTimeCtrlArray;
    int index = 0;
    for (auto &&item : m_data.timeCtrlArray){
        QJsonDocument tempDoc;
        CXAJsonTimeCtrl jsonTimeCtrl(item,index++);
        if (jsonTimeCtrl.buildJson(tempDoc)){
            if (tempDoc.isObject()){
                jsonTimeCtrlArray.append(tempDoc.object());
            }
        }
    }
    jsonTICtrlObj["TimeCtrl"] = jsonTimeCtrlArray;

    // build PIDTimeCtrl
    QJsonArray jsonPIDCtrlArray;
    index = 0;
    for (auto &&item : m_data.PIDCtrlArray){
        QJsonDocument tempDoc;
        CXAJsonPIDCtrl jsonPIDCtrl(item,index++);
        if (jsonPIDCtrl.buildJson(tempDoc)){
            if (tempDoc.isObject()){
                jsonPIDCtrlArray.append(tempDoc.object());
            }
        }
    }
    jsonTICtrlObj["PIDCtrl"] = jsonPIDCtrlArray;

    //设置doc
    doc.setObject(jsonTICtrlObj);
    return true;
}

bool CXAJsonTICtrl::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& jsonTICtrlObj = doc.object();

    m_data.BeforeTIStartup_TestMicroPID        =       jsonTICtrlObj["BeforeTIStartup_TestMicroPID"].toInt();
    m_data.BeforeTIStartup_MicroPIDValue_Min   =       jsonTICtrlObj["BeforeTIStartup_MicroPIDValue_Min"].toDouble();
    m_data.controlMode                         =       jsonTICtrlObj["ControlMode"].toInt();

    QJsonArray jsonTimeCtrlArray = jsonTICtrlObj["TimeCtrl"].toArray();
    int index = 0;
    for (auto &&item : jsonTimeCtrlArray){
        QJsonDocument tempDoc;
        tempDoc.setObject(item.toObject());
        CXAJsonTimeCtrl jsonTimeCtrl;
        jsonTimeCtrl.parseJson(tempDoc,version);
        index = jsonTimeCtrl.index();
        if(index<sizeof(m_data.timeCtrlArray)/sizeof(m_data.timeCtrlArray[0])){
            m_data.timeCtrlArray[index] = jsonTimeCtrl.TimeCtrl();
        }
    }

    QJsonArray jsonPIDCtrlArray = jsonTICtrlObj["PIDCtrl"].toArray();
    for (auto &&item : jsonPIDCtrlArray){
        QJsonDocument tempDoc;
        tempDoc.setObject(item.toObject());
        CXAJsonPIDCtrl jsonPIDCtrl;
        jsonPIDCtrl.parseJson(tempDoc,version);
        index = jsonPIDCtrl.index();
        if(index<sizeof(m_data.PIDCtrlArray)/sizeof(m_data.PIDCtrlArray[0])){
            m_data.PIDCtrlArray[index] = jsonPIDCtrl.PIDTimeCtrl();
        }
    }
    return true;
}

CXAJsonCOLUMNCtrl::CXAJsonCOLUMNCtrl(const SCOLUMNCtrl &value):
    m_data(value)
{

}

void CXAJsonCOLUMNCtrl::setCOLUMNCtrl(const SCOLUMNCtrl &value)
{
    m_data = value;
}

SCOLUMNCtrl CXAJsonCOLUMNCtrl::COLUMNCtrl() const
{
    return m_data;
}


void CXAJsonCOLUMNCtrl::resetJsonData()
{
    m_data.clear();
}

bool CXAJsonCOLUMNCtrl::buildJson(QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject jsonCOLUMNCtrlObj;
    jsonCOLUMNCtrlObj["ControlMode"] = m_data.controlMode;

    // build TimeCtrl
    QJsonArray jsonTimeCtrlArray;
    int index = 0;
    for (auto &&item : m_data.timeCtrlArray){
        QJsonDocument tempDoc;
        CXAJsonTimeCtrl jsonTimeCtrl(item,index++);
        if (jsonTimeCtrl.buildJson(tempDoc)){
            if (tempDoc.isObject()){
                jsonTimeCtrlArray.append(tempDoc.object());
            }
        }
    }
    jsonCOLUMNCtrlObj["TimeCtrl"] = jsonTimeCtrlArray;

    // build PIDTimeCtrl
    QJsonArray jsonPIDCtrlArray;
    index = 0;
    for (auto &&item : m_data.PIDCtrlArray){
        QJsonDocument tempDoc;
        CXAJsonPIDCtrl jsonPIDCtrl(item,index++);
        if (jsonPIDCtrl.buildJson(tempDoc)){
            if (tempDoc.isObject()){
                jsonPIDCtrlArray.append(tempDoc.object());
            }
        }
    }
    jsonCOLUMNCtrlObj["PIDCtrl"] = jsonPIDCtrlArray;

    //设置doc
    doc.setObject(jsonCOLUMNCtrlObj);
    return true;
}

bool CXAJsonCOLUMNCtrl::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& jsonCOLUMNCtrlObj = doc.object();
    m_data.controlMode                         =       jsonCOLUMNCtrlObj["ControlMode"].toInt();

    QJsonArray jsonTimeCtrlArray = jsonCOLUMNCtrlObj["TimeCtrl"].toArray();
    int index = 0;
    for (auto &&item : jsonTimeCtrlArray){
        QJsonDocument tempDoc;
        tempDoc.setObject(item.toObject());
        CXAJsonTimeCtrl jsonTimeCtrl;
        jsonTimeCtrl.parseJson(tempDoc,version);
        index = jsonTimeCtrl.index();
        if(index<sizeof(m_data.timeCtrlArray)/sizeof(m_data.timeCtrlArray[0])){
            m_data.timeCtrlArray[index] = jsonTimeCtrl.TimeCtrl();
        }
    }

    QJsonArray jsonPIDCtrlArray = jsonCOLUMNCtrlObj["PIDCtrl"].toArray();
    for (auto &&item : jsonPIDCtrlArray){
        QJsonDocument tempDoc;
        tempDoc.setObject(item.toObject());
        CXAJsonPIDCtrl jsonPIDCtrl;
        jsonPIDCtrl.parseJson(tempDoc,version);
        index = jsonPIDCtrl.index();
        if(index<sizeof(m_data.PIDCtrlArray)/sizeof(m_data.PIDCtrlArray[0])){
            m_data.PIDCtrlArray[index] = jsonPIDCtrl.PIDTimeCtrl();
        }
    }
    return true;
}



CXAJsonMicroPIDCtrl::CXAJsonMicroPIDCtrl(const SMicroPIDCtrl &value):
    m_data(value)
{

}

void CXAJsonMicroPIDCtrl::setMicroPIDCtrl(const SMicroPIDCtrl &value)
{
    m_data = value;
}

SMicroPIDCtrl CXAJsonMicroPIDCtrl::microPIDCtrl() const
{
    return m_data;
}

void CXAJsonMicroPIDCtrl::resetJsonData()
{
    m_data.clear();
}

bool CXAJsonMicroPIDCtrl::buildJson(QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject jsonMicroPIDCtrlObj;
    jsonMicroPIDCtrlObj["StartTime"]            = m_data.startTime;
    jsonMicroPIDCtrlObj["SamplingFreq"]         = m_data.samplingFreq;
    jsonMicroPIDCtrlObj["PlasmaFreq"]           = m_data.plasmaFreq;
    jsonMicroPIDCtrlObj["BaseLineVoltage"]      = m_data.baseLineVoltage;

    //设置doc
    doc.setObject(jsonMicroPIDCtrlObj);
    return true;
}

bool CXAJsonMicroPIDCtrl::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& jsonMicroPIDCtrlObj = doc.object();

    m_data.startTime            =       jsonMicroPIDCtrlObj["StartTime"].toInt();
    m_data.samplingFreq         =       jsonMicroPIDCtrlObj["SamplingFreq"].toDouble();
    m_data.plasmaFreq           =       jsonMicroPIDCtrlObj["PlasmaFreq"].toInt();
    m_data.baseLineVoltage      =       jsonMicroPIDCtrlObj["BaseLineVoltage"].toInt();

    return true;
}


CXAJsonRunParamSet::CXAJsonRunParamSet(const SRunParamSet &value):
    m_data(value)
{

}

void CXAJsonRunParamSet::setRunParam(const SRunParamSet &value)
{
    m_data=value;
}

SRunParamSet CXAJsonRunParamSet::runParam() const
{
    return m_data;
}

void CXAJsonRunParamSet::resetJsonData()
{
    m_data.clear();
}

#include <QDateTime>
bool CXAJsonRunParamSet::buildJson(QJsonDocument &doc, const qint8 version)
{
    QJsonObject jsonRunParamSetObj;

    // 其他信息
    QJsonObject jsonOtherSetObj;
    jsonOtherSetObj["COLUMNFanCloseTemperature"]    = m_data.COLUMNFanCloseTemperature;
    jsonOtherSetObj["CleaningTime"]                 = m_data.cleaningTime;
    jsonOtherSetObj["SamplingTime"]                 = m_data.samplingTime;
    jsonOtherSetObj["SamplingPumpVoltage"]          = m_data.samplingPumpVoltage;
    jsonOtherSetObj["EPCControlVoltage"]            = m_data.EPCControlVoltage;
    jsonOtherSetObj["TestDataAutoRepo"]             = m_data.testData_AutoRepo;

    jsonRunParamSetObj["OtherSet"] = jsonOtherSetObj;

    QJsonDocument tempDoc;
    // build TDCtrl
    CXAJsonTDCtrl jsonTDCtrl(m_data.TDCtrl);
    jsonTDCtrl.buildJson(tempDoc,version);
    jsonRunParamSetObj["TDCtrl"] = tempDoc.object();

    // build TICtrl
    CXAJsonTICtrl jsonTICtrl;
    jsonTICtrl.setTICtrl(m_data.TICtrl);
    jsonTICtrl.buildJson(tempDoc,version);
    jsonRunParamSetObj["TICtrl"] = tempDoc.object();

    // build COLUMNCtrl
    CXAJsonCOLUMNCtrl jsonCOLUMNCtrl;
    jsonCOLUMNCtrl.setCOLUMNCtrl(m_data.COLUMNCtrl);
    jsonCOLUMNCtrl.buildJson(tempDoc,version);
    jsonRunParamSetObj["COLUMNCtrl"] = tempDoc.object();

    // build MicroPIDCtrl
    CXAJsonMicroPIDCtrl jsonMicroPIDCtrl;
    jsonMicroPIDCtrl.setMicroPIDCtrl(m_data.microPIDCtrl);
    jsonMicroPIDCtrl.buildJson(tempDoc,version);
    jsonRunParamSetObj["MicroPIDCtrl"] = tempDoc.object();

    //设置doc
    doc.setObject(jsonRunParamSetObj);
    return true;
}

bool CXAJsonRunParamSet::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& jsonRunParamSetObj = doc.object();

    // 其他信息
    QJsonObject jsonOtherSetObj = jsonRunParamSetObj["OtherSet"].toObject();
    m_data.COLUMNFanCloseTemperature    =   jsonOtherSetObj["COLUMNFanCloseTemperature"].toInt();
    m_data.cleaningTime                 =   jsonOtherSetObj["CleaningTime"].toInt();
    m_data.samplingTime                 =   jsonOtherSetObj["SamplingTime"].toInt();
    m_data.samplingPumpVoltage          =   jsonOtherSetObj["SamplingPumpVoltage"].toInt();
    m_data.EPCControlVoltage            =   jsonOtherSetObj["EPCControlVoltage"].toInt();

    m_data.testData_AutoRepo            =   jsonOtherSetObj["TestDataAutoRepo"].toInt();


    QJsonDocument tempDoc;
    // parse TDCtrl
    tempDoc.setObject(jsonRunParamSetObj["TDCtrl"].toObject());
    CXAJsonTDCtrl jsonTDCtrl;
    jsonTDCtrl.parseJson(tempDoc,version);
    m_data.TDCtrl = jsonTDCtrl.TDCtrl();

    // parse TDCtrl
    tempDoc.setObject(jsonRunParamSetObj["TICtrl"].toObject());
    CXAJsonTICtrl jsonTICtrl;
    jsonTICtrl.parseJson(tempDoc,version);
    m_data.TICtrl = jsonTICtrl.TICtrl();

    // parse TDCtrl
    tempDoc.setObject(jsonRunParamSetObj["COLUMNCtrl"].toObject());
    CXAJsonCOLUMNCtrl jsonCOLUMNCtrl;
    jsonCOLUMNCtrl.parseJson(tempDoc,version);
    m_data.COLUMNCtrl = jsonCOLUMNCtrl.COLUMNCtrl();

    // parse TDCtrl
    tempDoc.setObject(jsonRunParamSetObj["MicroPIDCtrl"].toObject());
    CXAJsonMicroPIDCtrl jsonMicroPIDCtrl;
    jsonMicroPIDCtrl.parseJson(tempDoc,version);
    m_data.microPIDCtrl = jsonMicroPIDCtrl.microPIDCtrl();

    return true;
}


CXAJsonPIDAll::CXAJsonPIDAll(const SPIDAll &value):
    m_data(value)
{

}

void CXAJsonPIDAll::setPIDAll(const SPIDAll &value)
{
    m_data = value;
}

void CXAJsonPIDAll::resetJsonData()
{
    m_data.clear();
}

bool CXAJsonPIDAll::buildJson(QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject jsonPIDAllObj;

    QJsonObject jsonTDPIDObj;
    jsonTDPIDObj["P"]            = (double)m_data.TD_PID_P_Parma;
    jsonTDPIDObj["I"]            = (double)m_data.TD_PID_I_Parma;
    jsonTDPIDObj["D"]            = (double)m_data.TD_PID_D_Parma;
    jsonPIDAllObj["TD_PID"] = jsonTDPIDObj;

    QJsonObject jsonTIPIDObj;
    jsonTIPIDObj["P"]            = (double)m_data.TI_PID_P_Parma;
    jsonTIPIDObj["I"]            = (double)m_data.TI_PID_I_Parma;
    jsonTIPIDObj["D"]            = (double)m_data.TI_PID_D_Parma;
    jsonPIDAllObj["TI_PID"] = jsonTIPIDObj;

    QJsonObject jsonCOLUMNPIDObj;
    jsonCOLUMNPIDObj["P"]            = (double)m_data.COLUMN_PID_P_Parma;
    jsonCOLUMNPIDObj["I"]            = (double)m_data.COLUMN_PID_I_Parma;
    jsonCOLUMNPIDObj["D"]            = (double)m_data.COLUMN_PID_D_Parma;
    jsonPIDAllObj["COLUMN_PID"] = jsonCOLUMNPIDObj;

    //设置doc
    doc.setObject(jsonPIDAllObj);
    return true;
}

bool CXAJsonPIDAll::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& jsonPIDAllObj = doc.object();

    QJsonObject jsonTDPIDObj = jsonPIDAllObj["TD_PID"].toObject();
    m_data.TD_PID_P_Parma    =       jsonTDPIDObj["P"].toDouble();
    m_data.TD_PID_I_Parma    =       jsonTDPIDObj["I"].toDouble();
    m_data.TD_PID_D_Parma    =       jsonTDPIDObj["D"].toDouble();


    QJsonObject jsonTIPIDObj = jsonPIDAllObj["TI_PID"].toObject();
    m_data.TI_PID_P_Parma    =       jsonTIPIDObj["P"].toDouble();
    m_data.TI_PID_I_Parma    =       jsonTIPIDObj["I"].toDouble();
    m_data.TI_PID_D_Parma    =       jsonTIPIDObj["D"].toDouble();

    QJsonObject jsonCOLUMNPIDObj = jsonPIDAllObj["COLUMN_PID"].toObject();
    m_data.COLUMN_PID_P_Parma    =       jsonCOLUMNPIDObj["P"].toDouble();
    m_data.COLUMN_PID_I_Parma    =       jsonCOLUMNPIDObj["I"].toDouble();
    m_data.COLUMN_PID_D_Parma    =       jsonCOLUMNPIDObj["D"].toDouble();

    return true;
}


CXAJsonTestParamSet::CXAJsonTestParamSet(const STestParamSet &value):
    m_data(value)
{

}

void CXAJsonTestParamSet::setTestParam(const STestParamSet &value)
{
    m_data = value;
}

void CXAJsonTestParamSet::resetJsonData()
{
    m_data.clear();
}

bool CXAJsonTestParamSet::buildJson(QJsonDocument &doc, const qint8 version)
{
    QJsonObject jsonTestParamSetObj;

    // 基本信息
    QJsonObject jsonBaseInfoObj;
    jsonBaseInfoObj["Name"]                 =  m_name;
    jsonBaseInfoObj["Version"]              =  m_version;
    if(m_creatDataTime.isEmpty()){
        jsonBaseInfoObj["CreatDataTime"]    =  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    }
    else{
        jsonBaseInfoObj["CreatDataTime"] = m_creatDataTime;
    }
    jsonBaseInfoObj["ModifDataTime"]        =  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    jsonTestParamSetObj["BaseInfo"] = jsonBaseInfoObj;

    QJsonDocument tempDoc;
    // build PIDAll
    CXAJsonPIDAll jsonPIDAllCtrl;
    jsonPIDAllCtrl.setPIDAll(m_data.PIDAll);
    jsonPIDAllCtrl.buildJson(tempDoc,version);
    jsonTestParamSetObj["PIDAll"] = tempDoc.object();

    // build RunParamSet
    CXAJsonRunParamSet jsonRunParamSet(m_data.runParamSet);
    jsonRunParamSet.buildJson(tempDoc,version);
    jsonTestParamSetObj["RunParam"] = tempDoc.object();

    //设置doc
    doc.setObject(jsonTestParamSetObj);
    return true;
}

bool CXAJsonTestParamSet::parseJson(const QJsonDocument &doc, const qint8 version)
{
    Q_UNUSED(version);
    QJsonObject&& jsonTestParamSetObj = doc.object();

    // 基本信息
    QJsonObject jsonBaseInfoObj = jsonTestParamSetObj["BaseInfo"].toObject();
    m_name              =   jsonBaseInfoObj["Name"].toString();
    m_version           =   (EVersion)jsonBaseInfoObj["Name"].toInt();
    m_creatDataTime     =   jsonBaseInfoObj["CreatDataTime"].toString();
    m_modifDataTime     =   jsonBaseInfoObj["ModifDataTime"].toString();

    QJsonDocument tempDoc;
    // parse PIDAll
    tempDoc.setObject(jsonTestParamSetObj["PIDAll"].toObject());
    CXAJsonPIDAll jsonPIDAllCtrl;
    jsonPIDAllCtrl.parseJson(tempDoc,version);
    m_data.PIDAll = jsonPIDAllCtrl.PIDAll();

    // parse RunParamSet
    tempDoc.setObject(jsonTestParamSetObj["RunParam"].toObject());
    CXAJsonRunParamSet jsonRunParamSet;
    jsonRunParamSet.parseJson(tempDoc,version);
    m_data.runParamSet = jsonRunParamSet.runParam();

    return true;
}
