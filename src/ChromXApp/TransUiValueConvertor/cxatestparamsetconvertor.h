#ifndef CXATESTPARAMSETCONVERTOR_H
#define CXATESTPARAMSETCONVERTOR_H

#include <CCE_CommunicatEngine/CCEInteCtrlPackageStruct.h>

typedef struct tagUiTimeCtrl{
    quint16 timeValue;
    quint8 PWMValue;
    tagUiTimeCtrl()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiTimeCtrl));
    }
}SUiTimeCtrl;

typedef struct tagUiPIDCtrl{
    quint16 timeValue;
    double temperatureValue;
    quint8 PWMValue_Min;
    quint8 PWMValue_Max;
    tagUiPIDCtrl()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiPIDCtrl));
    }
}SUiPIDCtrl;

typedef struct tagUiTDCtrl{
    quint8  CounterBlowingTime;
    bool  BeforeTDStartup_TestPCG;
    quint16 TDStart_CarrierPressure_UpLimit;
    quint16 TDStart_CarrierPressure_LowLimit;
    bool  BeforeTDStartup_TestTITemperature;
    double BeforeTDStartup_TITemperature_Max;
    bool  controlMode;

    SUiTimeCtrl timeCtrlArray[5];
    SUiPIDCtrl PIDCtrlArray[5];

    tagUiTDCtrl()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiTDCtrl));
    }
}SUiTDCtrl;

typedef struct tagUiTICtrl{
    bool  BeforeTIStartup_TestMicroPID;
    double BeforeTIStartup_MicroPIDValue_Min;
    bool  controlMode;

    SUiTimeCtrl timeCtrlArray[5];
    SUiPIDCtrl PIDCtrlArray[5];

    tagUiTICtrl()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiTICtrl));
    }
}SUiTICtrl;

typedef struct tagUiCOLUMNCtrl{
    bool controlMode;

    SUiTimeCtrl timeCtrlArray[8];
    SUiPIDCtrl PIDCtrlArray[8];
    tagUiCOLUMNCtrl()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiCOLUMNCtrl));
    }
}SUiCOLUMNCtrl;

typedef struct tagUiMicroPIDCtrl{
    quint16 startTime;
    quint8  samplingFreq;
    quint16 plasmaFreq;
    quint16 baseLineVoltage;
    tagUiMicroPIDCtrl()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiMicroPIDCtrl));
    }
}SUiMicroPIDCtrl;

typedef struct tagUiPIDAll {
    double TD_PID_P_Parma;                 //0x0000-0x0003	R/W	TD  PID 参数 ， P 数值 ， (定点，6位小数)
    double TD_PID_I_Parma;                 //0x0004-0x0007	R/W	TD  PID 参数 ， I 数值 ， (定点，6位小数)
    double TD_PID_D_Parma;                 //0x0008-0x000b	R/W	TD  PID 参数 ，D 数值 ，(定点，6位小数)

    double TI_PID_P_Parma;                 //0x000c-0x000f	R/W	TI PID 参数 ， P 数值 ， (定点，6位小数)
    double TI_PID_I_Parma;                 //0x0010-0x0013	R/W	TI PID 参数 ， I 数值 ， (定点，6位小数)
    double TI_PID_D_Parma;                 //0x0014-0x0017	R/W	TI PID 参数 ，D 数值 ，(定点，6位小数)

    double COLUMN_PID_P_Parma;             //0x0018-0x001b	R/W	COLUMN PID 参数 ， P 数值 ， (定点，6位小数)
    double COLUMN_PID_I_Parma;             //0x001c-0x001f	R/W	COLUMN PID 参数 ， I 数值 ， (定点，6位小数)
    double COLUMN_PID_D_Parma;             //0x0020-0x0023	R/W	COLUMN PID 参数 ，D 数值 ，(定点，6位小数)

    tagUiPIDAll()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiPIDAll));
    }
}SUiPIDAll;

typedef struct tagUiRunParamSet{
    double COLUMNFanCloseTemperature;      //0x0024-0x0025	R/W	column风扇关闭条件： column 温度值：0-65536

    quint8 reserved1[10];

    quint16 cleaningTime;                   //0x0030-0x0031	R/W	清洗时间 (s)  (0-65535)
    quint16 samplingTime;                   //0x0032-0x0033	R/W	采样时间 (s)  (0-65535)
    quint8  samplingPumpVoltage;            //0x0034	R/W	采样泵电压 （0-100）
    quint16 EPCControlVoltage;              //0x0035-0x0036	R/W	EPC控制电压

    quint8  reserved2[4];

    SUiTDCtrl TDCtrl;                         //TD 控制
    SUiTICtrl TICtrl;                         //TI 控制
    SUiCOLUMNCtrl COLUMNCtrl;                 //COLUMN 控制

    quint8  reserved3[3];

    SUiMicroPIDCtrl microPIDCtrl;             //microPID 控制
    quint8 testData_AutoRepo;               //0x00f7	R/W	测试数据自动上传  0 不自动上传， 1自动上传
    tagUiRunParamSet()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiRunParamSet));
    }
}SUiRunParamSet;

typedef struct tagUiTestParamSet{
    SUiPIDAll PIDAll;
    SUiRunParamSet runParamSet;

    quint8 testStatus;                      //0x00f8	R/W	测试运行/停止  0 停止，1 为运行

    tagUiTestParamSet()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiTestParamSet));
    }
}SUiTestParamSet;

class CXATestParamSetConvertor
{
public:
    void convertPIDAllParamFromUi(const SUiPIDAll& m_uiPIDparam, SPIDAll& m_PIDparam);
    void convertPIDAllParamToUi(const SPIDAll& m_PIDparam, SUiPIDAll& m_uiPIDparam);

    void convertRunParamSetFromUi(const SUiRunParamSet& m_uiRunParamSet, SRunParamSet& m_runParamSet);
    void convertRunParamSetToUi(const SRunParamSet& m_runParamSet, SUiRunParamSet& m_uiRunParamSet);

    void convertTestParamSetFromUi(const SUiTestParamSet& m_uiTestParamSet, STestParamSet& m_testParamSet);
    void convertTestParamSetToUi(const STestParamSet& m_testParamSet, SUiTestParamSet& m_uiTestParamSet);

private:
    void convertTimeCtrlFromUi(const SUiTimeCtrl& m_uiTimeCtrl, STimeCtrl& m_timeCtrl);
    void convertTimeCtrlToUi(const STimeCtrl& m_timeCtrl, SUiTimeCtrl& m_uiTimeCtrl);

    void convertPIDCtrlParamFromUi(const SUiPIDCtrl& m_uiPIDCtrl, SPIDCtrl& m_PIDCtrl,bool rtd = true);
    void convertPIDCtrlParamToUi(const SPIDCtrl& m_PIDCtrl, SUiPIDCtrl& m_uiPIDCtrl,bool rtd = true);

    void convertTDCtrlParamFromUi(const SUiTDCtrl& m_uiTDCtrl, STDCtrl& m_TDCtrl);
    void convertTDCtrlParamToUi(const STDCtrl& m_TDCtrl, SUiTDCtrl& m_uiTDCtrl);

    void convertTICtrlParamFromUi(const SUiTICtrl& m_uiTICtrl, STICtrl& m_TICtrl);
    void convertTICtrlParamToUi(const STICtrl& m_TICtrl, SUiTICtrl& m_uiTICtrl);

    void convertCOLUMNCtrlParamFromUi(const SUiCOLUMNCtrl& m_uiCOLUMNCtrl, SCOLUMNCtrl& m_COLUMNCtrl);
    void convertCOLUMNCtrlParamToUi(const SCOLUMNCtrl& m_COLUMNCtrl, SUiCOLUMNCtrl& m_uiCOLUMNCtrl);

    void convertMicroPIDCtrlParamFromUi(const SUiMicroPIDCtrl& m_uiMicroPIDCtrl, SMicroPIDCtrl& m_MicroPIDCtrl);
    void convertMicroPIDCtrlParamToUi(const SMicroPIDCtrl& m_MicroPIDCtrl, SUiMicroPIDCtrl& m_uiMicroPIDCtrl);
};

#endif // CXATESTPARAMSETCONVERTOR_H
