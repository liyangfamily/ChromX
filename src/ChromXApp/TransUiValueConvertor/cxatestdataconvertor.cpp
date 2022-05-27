#include "cxatestdataconvertor.h"

CXATestDataConvertor::CXATestDataConvertor()
{

}

void CXATestDataConvertor::convertTestDataToUi(const STestData &m_testData, SUiTestData &m_uiTestData)
{
    convertTDTemperToUi(m_testData.TDCurTemperature, m_uiTestData.TDCurTemperature,false);
    convertTITemperToUi(m_testData.TICurTemperature, m_uiTestData.TICurTemperature);
    m_uiTestData.curTestRunTime = m_testData.curTestRunTime / 1000.0;
    convertCOLUMNTemperToUi(m_testData.COLUMNTemperature, m_uiTestData.COLUMNTemperature);
    convertMicroPIDValueToUi(m_testData.MicroPIDValue, m_uiTestData.MicroPIDValue);
}

void CXATestDataConvertor::convertTDTemperFromUi(const double &m_uiTDTemper, quint16 &m_TDTemper,bool rtd)
{
    if(rtd){
        m_TDTemper = CCEUIHelper::temperToResistance(m_uiTDTemper);
    }
    else{
        m_TDTemper = m_uiTDTemper * 10;
    }
}

void CXATestDataConvertor::convertTDTemperToUi(const quint16 &m_TDTemper, double &m_uiTDTemper,bool rtd)
{
    if(rtd){
        m_uiTDTemper = CCEUIHelper::resistanceToTemper(m_TDTemper);
    }
    else{
        m_uiTDTemper = m_TDTemper / 10.0;
    }
}

void CXATestDataConvertor::convertTITemperFromUi(const double &m_uiTITemper, quint16 &m_TITemper)
{
    m_TITemper = CCEUIHelper::temperToResistance(m_uiTITemper);
}

void CXATestDataConvertor::convertTITemperToUi(const quint16 &m_TITemper, double &m_uiTITemper)
{
    m_uiTITemper = CCEUIHelper::resistanceToTemper(m_TITemper);
}

void CXATestDataConvertor::convertCOLUMNTemperFromUi(const double &m_uiCOLUMNTemper, quint16 &m_COLUMNTemper)
{
    m_COLUMNTemper = CCEUIHelper::temperToResistance(m_uiCOLUMNTemper);
}

void CXATestDataConvertor::convertCOLUMNTemperToUi(const quint16 &m_COLUMNTemper, double &m_uiCOLUMNTemper)
{
    m_uiCOLUMNTemper = CCEUIHelper::resistanceToTemper(m_COLUMNTemper);
}

void CXATestDataConvertor::convertMicroPIDValueToUi(const quint32 &m_MicroPIDValue, double &m_uiMicroPIDValue)
{
    m_uiMicroPIDValue = CCEUIHelper::getMicroPIDUiValue(m_MicroPIDValue);
}

