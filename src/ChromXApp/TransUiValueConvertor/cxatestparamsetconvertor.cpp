#include "cxatestparamsetconvertor.h"
#include <CCE_Core/CCEUIHelper>





void CXATestParamSetConvertor::convertTimeCtrlFromUi(const SUiTimeCtrl &m_uiTimeCtrl, STimeCtrl &m_timeCtrl)
{
    m_timeCtrl.timeValue = m_uiTimeCtrl.timeValue;
    m_timeCtrl.PWMValue = m_uiTimeCtrl.PWMValue;
}

void CXATestParamSetConvertor::convertTimeCtrlToUi(const STimeCtrl &m_timeCtrl, SUiTimeCtrl &m_uiTimeCtrl)
{
    m_uiTimeCtrl.timeValue = m_timeCtrl.timeValue;
    m_uiTimeCtrl.PWMValue = m_timeCtrl.PWMValue;
}

void CXATestParamSetConvertor::convertPIDCtrlParamFromUi(const SUiPIDCtrl &m_uiPIDCtrl, SPIDCtrl &m_PIDCtrl)
{
    m_PIDCtrl.temperatureValue = CCEUIHelper::temperToResistance(m_uiPIDCtrl.temperatureValue);

    m_PIDCtrl.timeValue = m_uiPIDCtrl.timeValue;
    m_PIDCtrl.PWMValue_Max = m_uiPIDCtrl.PWMValue_Max;
    m_PIDCtrl.PWMValue_Min = m_uiPIDCtrl.PWMValue_Min;
}

void CXATestParamSetConvertor::convertPIDCtrlParamToUi(const SPIDCtrl &m_PIDCtrl, SUiPIDCtrl &m_uiPIDCtrl)
{
    m_uiPIDCtrl.temperatureValue = CCEUIHelper::resistanceToTemper(m_PIDCtrl.temperatureValue);

    m_uiPIDCtrl.timeValue = m_PIDCtrl.timeValue;
    m_uiPIDCtrl.PWMValue_Max = m_PIDCtrl.PWMValue_Max;
    m_uiPIDCtrl.PWMValue_Min = m_PIDCtrl.PWMValue_Min;
}

void CXATestParamSetConvertor::convertTDCtrlParamFromUi(const SUiTDCtrl &m_uiTDCtrl, STDCtrl &m_TDCtrl)
{
    m_TDCtrl.BeforeTDStartup_TITemperature_Max = CCEUIHelper::temperToResistance(m_uiTDCtrl.BeforeTDStartup_TITemperature_Max);

    m_TDCtrl.CounterBlowingTime = m_uiTDCtrl.CounterBlowingTime;
    m_TDCtrl.BeforeTDStartup_TestPCG = m_uiTDCtrl.BeforeTDStartup_TestPCG;
    m_TDCtrl.TDStart_CarrierPressure_UpLimit = m_uiTDCtrl.TDStart_CarrierPressure_UpLimit;
    m_TDCtrl.TDStart_CarrierPressure_LowLimit = m_uiTDCtrl.TDStart_CarrierPressure_LowLimit;
    m_TDCtrl.BeforeTDStartup_TestTITemperature = m_uiTDCtrl.BeforeTDStartup_TestTITemperature;
    m_TDCtrl.controlMode = m_uiTDCtrl.controlMode;

    for(int i = 0; i < (int)(sizeof(m_uiTDCtrl.timeCtrlArray) / sizeof(m_uiTDCtrl.timeCtrlArray[0])); ++i) {
        convertTimeCtrlFromUi(m_uiTDCtrl.timeCtrlArray[i], m_TDCtrl.timeCtrlArray[i]);
        convertPIDCtrlParamFromUi(m_uiTDCtrl.PIDCtrlArray[i], m_TDCtrl.PIDCtrlArray[i]);
    }
}

void CXATestParamSetConvertor::convertTDCtrlParamToUi(const STDCtrl &m_TDCtrl, SUiTDCtrl &m_uiTDCtrl)
{
    m_uiTDCtrl.BeforeTDStartup_TITemperature_Max = CCEUIHelper::resistanceToTemper(m_TDCtrl.BeforeTDStartup_TITemperature_Max);

    m_uiTDCtrl.CounterBlowingTime = m_TDCtrl.CounterBlowingTime;
    m_uiTDCtrl.BeforeTDStartup_TestPCG = m_TDCtrl.BeforeTDStartup_TestPCG;
    m_uiTDCtrl.TDStart_CarrierPressure_UpLimit = m_TDCtrl.TDStart_CarrierPressure_UpLimit;
    m_uiTDCtrl.TDStart_CarrierPressure_LowLimit = m_TDCtrl.TDStart_CarrierPressure_LowLimit;
    m_uiTDCtrl.BeforeTDStartup_TestTITemperature = m_TDCtrl.BeforeTDStartup_TestTITemperature;
    m_uiTDCtrl.controlMode = m_TDCtrl.controlMode;

    for(int i = 0; i < (int)(sizeof(m_TDCtrl.timeCtrlArray) / sizeof(m_TDCtrl.timeCtrlArray[0])); ++i) {
        convertTimeCtrlToUi(m_TDCtrl.timeCtrlArray[i], m_uiTDCtrl.timeCtrlArray[i]);
        convertPIDCtrlParamToUi(m_TDCtrl.PIDCtrlArray[i], m_uiTDCtrl.PIDCtrlArray[i]);
    }
}

void CXATestParamSetConvertor::convertTICtrlParamFromUi(const SUiTICtrl &m_uiTICtrl, STICtrl &m_TICtrl)
{
    m_TICtrl.BeforeTIStartup_TestMicroPID = m_uiTICtrl.BeforeTIStartup_TestMicroPID;
    m_TICtrl.BeforeTIStartup_MicroPIDValue_Min = m_uiTICtrl.BeforeTIStartup_MicroPIDValue_Min;
    m_TICtrl.controlMode = m_uiTICtrl.controlMode;

    for(int i = 0; i < (int)(sizeof(m_uiTICtrl.timeCtrlArray) / sizeof(m_uiTICtrl.timeCtrlArray[0])); ++i) {
        convertTimeCtrlFromUi(m_uiTICtrl.timeCtrlArray[i], m_TICtrl.timeCtrlArray[i]);
        convertPIDCtrlParamFromUi(m_uiTICtrl.PIDCtrlArray[i], m_TICtrl.PIDCtrlArray[i]);
    }
}

void CXATestParamSetConvertor::convertTICtrlParamToUi(const STICtrl &m_TICtrl, SUiTICtrl &m_uiTICtrl)
{
    m_uiTICtrl.BeforeTIStartup_TestMicroPID = m_TICtrl.BeforeTIStartup_TestMicroPID;
    m_uiTICtrl.BeforeTIStartup_MicroPIDValue_Min = m_TICtrl.BeforeTIStartup_MicroPIDValue_Min;
    m_uiTICtrl.controlMode = m_TICtrl.controlMode;

    for(int i = 0; i < (int)(sizeof(m_TICtrl.timeCtrlArray) / sizeof(m_TICtrl.timeCtrlArray[0])); ++i) {
        convertTimeCtrlToUi(m_TICtrl.timeCtrlArray[i], m_uiTICtrl.timeCtrlArray[i]);
        convertPIDCtrlParamToUi(m_TICtrl.PIDCtrlArray[i], m_uiTICtrl.PIDCtrlArray[i]);
    }
}

void CXATestParamSetConvertor::convertCOLUMNCtrlParamFromUi(const SUiCOLUMNCtrl &m_uiCOLUMNCtrl, SCOLUMNCtrl &m_COLUMNCtrl)
{
    m_COLUMNCtrl.controlMode = m_uiCOLUMNCtrl.controlMode;

    for(int i = 0; i < (int)(sizeof(m_uiCOLUMNCtrl.timeCtrlArray) / sizeof(m_uiCOLUMNCtrl.timeCtrlArray[0])); ++i) {
        convertTimeCtrlFromUi(m_uiCOLUMNCtrl.timeCtrlArray[i], m_COLUMNCtrl.timeCtrlArray[i]);
        convertPIDCtrlParamFromUi(m_uiCOLUMNCtrl.PIDCtrlArray[i], m_COLUMNCtrl.PIDCtrlArray[i]);
    }
}

void CXATestParamSetConvertor::convertCOLUMNCtrlParamToUi(const SCOLUMNCtrl &m_COLUMNCtrl, SUiCOLUMNCtrl &m_uiCOLUMNCtrl)
{
    m_uiCOLUMNCtrl.controlMode = m_COLUMNCtrl.controlMode;

    for(int i = 0; i < (int)(sizeof(m_COLUMNCtrl.timeCtrlArray) / sizeof(m_COLUMNCtrl.timeCtrlArray[0])); ++i) {
        convertTimeCtrlToUi(m_COLUMNCtrl.timeCtrlArray[i], m_uiCOLUMNCtrl.timeCtrlArray[i]);
        convertPIDCtrlParamToUi(m_COLUMNCtrl.PIDCtrlArray[i], m_uiCOLUMNCtrl.PIDCtrlArray[i]);
    }
}

void CXATestParamSetConvertor::convertMicroPIDCtrlParamFromUi(const SUiMicroPIDCtrl &m_uiMicroPIDCtrl, SMicroPIDCtrl &m_MicroPIDCtrl)
{
    m_MicroPIDCtrl.startTime = m_uiMicroPIDCtrl.startTime;
    m_MicroPIDCtrl.samplingFreq = m_uiMicroPIDCtrl.samplingFreq;
    m_MicroPIDCtrl.plasmaFreq = m_uiMicroPIDCtrl.plasmaFreq;
    m_MicroPIDCtrl.baseLineVoltage = m_uiMicroPIDCtrl.baseLineVoltage;
}

void CXATestParamSetConvertor::convertMicroPIDCtrlParamToUi(const SMicroPIDCtrl &m_MicroPIDCtrl, SUiMicroPIDCtrl &m_uiMicroPIDCtrl)
{
    m_uiMicroPIDCtrl.startTime = m_MicroPIDCtrl.startTime;
    m_uiMicroPIDCtrl.samplingFreq = m_MicroPIDCtrl.samplingFreq;
    m_uiMicroPIDCtrl.plasmaFreq = m_MicroPIDCtrl.plasmaFreq;
    m_uiMicroPIDCtrl.baseLineVoltage = m_MicroPIDCtrl.baseLineVoltage;
}

void CXATestParamSetConvertor::convertPIDAllParamFromUi(const SUiPIDAll &m_uiPIDparam, SPIDAll &m_PIDparam)
{
    m_PIDparam.TD_PID_P_Parma = m_uiPIDparam.TD_PID_P_Parma * 1000000;
    m_PIDparam.TD_PID_I_Parma = m_uiPIDparam.TD_PID_I_Parma * 1000000;
    m_PIDparam.TD_PID_D_Parma = m_uiPIDparam.TD_PID_D_Parma * 1000000;

    m_PIDparam.TI_PID_P_Parma = m_uiPIDparam.TI_PID_P_Parma * 1000000;
    m_PIDparam.TI_PID_I_Parma = m_uiPIDparam.TI_PID_I_Parma * 1000000;
    m_PIDparam.TI_PID_D_Parma = m_uiPIDparam.TI_PID_D_Parma * 1000000;

    m_PIDparam.COLUMN_PID_P_Parma = m_uiPIDparam.COLUMN_PID_P_Parma * 1000000;
    m_PIDparam.COLUMN_PID_I_Parma = m_uiPIDparam.COLUMN_PID_I_Parma * 1000000;
    m_PIDparam.COLUMN_PID_D_Parma = m_uiPIDparam.COLUMN_PID_D_Parma * 1000000;
}

void CXATestParamSetConvertor::convertPIDAllParamToUi(const SPIDAll &m_PIDparam, SUiPIDAll &m_uiPIDparam)
{
    m_uiPIDparam.TD_PID_P_Parma = static_cast<double>(m_PIDparam.TD_PID_P_Parma) / 1000000;
    m_uiPIDparam.TD_PID_I_Parma = static_cast<double>(m_PIDparam.TD_PID_I_Parma) / 1000000;
    m_uiPIDparam.TD_PID_D_Parma = static_cast<double>(m_PIDparam.TD_PID_D_Parma) / 1000000;

    m_uiPIDparam.TI_PID_P_Parma = static_cast<double>(m_PIDparam.TI_PID_P_Parma) / 1000000;
    m_uiPIDparam.TI_PID_I_Parma = static_cast<double>(m_PIDparam.TI_PID_I_Parma) / 1000000;
    m_uiPIDparam.TI_PID_D_Parma = static_cast<double>(m_PIDparam.TI_PID_D_Parma) / 1000000;

    m_uiPIDparam.COLUMN_PID_P_Parma = static_cast<double>(m_PIDparam.COLUMN_PID_P_Parma) / 1000000;
    m_uiPIDparam.COLUMN_PID_I_Parma = static_cast<double>(m_PIDparam.COLUMN_PID_I_Parma) / 1000000;
    m_uiPIDparam.COLUMN_PID_D_Parma = static_cast<double>(m_PIDparam.COLUMN_PID_D_Parma) / 1000000;
}

void CXATestParamSetConvertor::convertRunParamSetFromUi(const SUiRunParamSet &m_uiRunParamSet, SRunParamSet &m_runParamSet)
{
    m_runParamSet.COLUMNFanCloseTemperature = CCEUIHelper::temperToResistance(m_uiRunParamSet.COLUMNFanCloseTemperature);

    m_runParamSet.cleaningTime = m_uiRunParamSet.cleaningTime;
    m_runParamSet.samplingTime = m_uiRunParamSet.samplingTime;
    m_runParamSet.samplingPumpVoltage = m_uiRunParamSet.samplingPumpVoltage;
    m_runParamSet.EPCControlVoltage = m_uiRunParamSet.EPCControlVoltage;

    convertTDCtrlParamFromUi(m_uiRunParamSet.TDCtrl, m_runParamSet.TDCtrl);
    convertTICtrlParamFromUi(m_uiRunParamSet.TICtrl, m_runParamSet.TICtrl);
    convertCOLUMNCtrlParamFromUi(m_uiRunParamSet.COLUMNCtrl, m_runParamSet.COLUMNCtrl);
    convertMicroPIDCtrlParamFromUi(m_uiRunParamSet.microPIDCtrl, m_runParamSet.microPIDCtrl);

    m_runParamSet.testData_AutoRepo = m_uiRunParamSet.testData_AutoRepo;
}

void CXATestParamSetConvertor::convertRunParamSetToUi(const SRunParamSet &m_runParamSet, SUiRunParamSet &m_uiRunParamSet)
{
    m_uiRunParamSet.COLUMNFanCloseTemperature = CCEUIHelper::resistanceToTemper(m_runParamSet.COLUMNFanCloseTemperature);

    m_uiRunParamSet.cleaningTime = m_runParamSet.cleaningTime;
    m_uiRunParamSet.samplingTime = m_runParamSet.samplingTime;
    m_uiRunParamSet.samplingPumpVoltage = m_runParamSet.samplingPumpVoltage;
    m_uiRunParamSet.EPCControlVoltage = m_runParamSet.EPCControlVoltage;

    convertTDCtrlParamToUi(m_runParamSet.TDCtrl, m_uiRunParamSet.TDCtrl);
    convertTICtrlParamToUi(m_runParamSet.TICtrl, m_uiRunParamSet.TICtrl);
    convertCOLUMNCtrlParamToUi(m_runParamSet.COLUMNCtrl, m_uiRunParamSet.COLUMNCtrl);
    convertMicroPIDCtrlParamToUi(m_runParamSet.microPIDCtrl, m_uiRunParamSet.microPIDCtrl);

    m_uiRunParamSet.testData_AutoRepo = m_runParamSet.testData_AutoRepo;
}

void CXATestParamSetConvertor::convertTestParamSetFromUi(const SUiTestParamSet &m_uiTestParamSet, STestParamSet &m_testParamSet)
{
    convertPIDAllParamFromUi(m_uiTestParamSet.PIDAll, m_testParamSet.PIDAll);
    convertRunParamSetFromUi(m_uiTestParamSet.runParamSet, m_testParamSet.runParamSet);

    m_testParamSet.testStatus = m_uiTestParamSet.testStatus;
}

void CXATestParamSetConvertor::convertTestParamSetToUi(const STestParamSet &m_testParamSet, SUiTestParamSet &m_uiTestParamSet)
{
    convertPIDAllParamToUi(m_testParamSet.PIDAll, m_uiTestParamSet.PIDAll);
    convertRunParamSetToUi(m_testParamSet.runParamSet, m_uiTestParamSet.runParamSet);

    m_uiTestParamSet.testStatus = m_testParamSet.testStatus;
}


