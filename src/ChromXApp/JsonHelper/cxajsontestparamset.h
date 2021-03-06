/*********************************************************************************************************
** This file is part of the CCE_Core module of the CCE Toolkit.
*********************************************************************************************************/
#ifndef CVTE_H_CXAJSONCONNECTION
#define CVTE_H_CXAJSONCONNECTION
#pragma once

#include <CCE_Core/CCEJsonHelper.h>
#include <CCE_CommunicatEngine/CCEInteCtrlPackageStruct.h>
#include <QObject>
#include <QMap>
#include <QRectF>
#include <QUuid>

class CXAJsonTimeCtrl : public CCEJsonHelper
{
public:
    CXAJsonTimeCtrl(){};
    CXAJsonTimeCtrl(const STimeCtrl& value, int index);

    void setTimeCtrl(const STimeCtrl& value, int index);
    STimeCtrl TimeCtrl() const{return m_data;}
    int index(){return m_index;}
    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    STimeCtrl m_data;
    int m_index = 0;
};


class CXAJsonPIDCtrl : public CCEJsonHelper
{
public:
    CXAJsonPIDCtrl(){};
    CXAJsonPIDCtrl(const SPIDCtrl& value, int index);

    void setPIDTimeCtrl(const SPIDCtrl& value, int index);
    SPIDCtrl PIDTimeCtrl() const{return m_data;}
    int index(){return m_index;}
    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    SPIDCtrl m_data;
    int m_index;
};

class CXAJsonPressureCtrl : public CCEJsonHelper
{
public:
    CXAJsonPressureCtrl(){};
    CXAJsonPressureCtrl(const SPressureCtrl& value, int index);

    void setPressureCtrl(const SPressureCtrl& value, int index);
    SPressureCtrl PressureCtrl() const{return m_data;}
    int index(){return m_index;}
    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    SPressureCtrl m_data;
    int m_index;
};

class CXAJsonTDCtrl : public CCEJsonHelper
{
public:
    CXAJsonTDCtrl(){};
    CXAJsonTDCtrl(const STDCtrl& value);

    void setTDCtrl(const STDCtrl& value);
    STDCtrl TDCtrl() const{return m_data;}

    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    STDCtrl m_data;
};

class CXAJsonTICtrl : public CCEJsonHelper
{
public:
    CXAJsonTICtrl(){};
    CXAJsonTICtrl(const STICtrl& value);

    void setTICtrl(const STICtrl& value);
    STICtrl TICtrl() const{return m_data;}

    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    STICtrl m_data;
};

class CXAJsonCOLUMNCtrl : public CCEJsonHelper
{
public:
    CXAJsonCOLUMNCtrl(){};
    CXAJsonCOLUMNCtrl(const SCOLUMNCtrl& value);

    void setCOLUMNCtrl(const SCOLUMNCtrl& value);
    SCOLUMNCtrl COLUMNCtrl() const;

    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    SCOLUMNCtrl m_data;
};

class CXAJsonMicroPIDCtrl : public CCEJsonHelper
{
public:
    CXAJsonMicroPIDCtrl(){};
    CXAJsonMicroPIDCtrl(const SMicroPIDCtrl& value);

    void setMicroPIDCtrl(const SMicroPIDCtrl& value);
    SMicroPIDCtrl microPIDCtrl() const;

    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    SMicroPIDCtrl m_data;;
};


class CXAJsonRunParamSet : public CCEJsonHelper
{
public:
    CXAJsonRunParamSet(){};
    CXAJsonRunParamSet(const SRunParamSet& value);
    void setRunParam(const SRunParamSet& value);
    SRunParamSet runParam() const;
public:
    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    SRunParamSet m_data;
};

class CXAJsonPIDAll : public CCEJsonHelper
{
public:
    CXAJsonPIDAll(){};
    CXAJsonPIDAll(const SPIDAll& value);

    void setPIDAll(const SPIDAll& value);
    SPIDAll PIDAll() const{
        return m_data;
    }

    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    SPIDAll m_data;;
};

class CXAJsonPressureMode : public CCEJsonHelper
{
public:
    CXAJsonPressureMode(){};
    CXAJsonPressureMode(const SPressureMode& value);

    void setPressureMode(const SPressureMode& value);
    SPressureMode PressureMode() const{
        return m_data;
    }

    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    SPressureMode m_data;;
};

class CXAJsonTestParamSet : public CCEJsonHelper
{
public:
    enum EVersion
    {
        Unknownversion = 0,
        StdV1 = 1,
        StdV2 = 2,
    };
    CXAJsonTestParamSet(){};
    CXAJsonTestParamSet(const STestParamSet& value);
    void setName(QString name){m_name = name;}
    QString name(){return m_name;}
    EVersion versin(){return m_version;}
    QString creatDataTime(){return m_creatDataTime;}
    QString modifyDataTime(){return m_modifDataTime;}

    void setTestParam(const STestParamSet& value);
    STestParamSet testParam() const{
        return m_data;
    };
    static QString defaultFilePath();
public:
    void resetJsonData();
public:
    bool buildJson(QJsonDocument &doc, const qint8 version = 1);
    bool parseJson(const QJsonDocument &doc, const qint8 version = 1);

private:
    QString m_name;
    EVersion m_version = EVersion::StdV1;
    QString m_creatDataTime;
    QString m_modifDataTime;
    STestParamSet m_data;
};
#endif
