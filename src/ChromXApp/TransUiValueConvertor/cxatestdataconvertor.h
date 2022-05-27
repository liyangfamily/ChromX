#ifndef CXATESTDATACONVERTOR_H
#define CXATESTDATACONVERTOR_H

#include <CCE_CommunicatEngine/CCEInteCtrlPackageStruct.h>

typedef struct tagUiTestData {
    double TDCurTemperature;
    double TICurTemperature;
    double curTestRunTime;
    double COLUMNTemperature;
    double MicroPIDValue;

    tagUiTestData()
    {
        clear();
    }
    void clear()
    {
        memset(this, 0, sizeof(tagUiTestData));
    }
}SUiTestData;

class CXATestDataConvertor
{
public:
    CXATestDataConvertor();
    void convertTestDataToUi(const STestData& m_testData, SUiTestData& m_uiTestData);

    void convertTDTemperFromUi(const double& m_uiTDTemper, quint16& m_TDTemper,bool rtd=true);
    void convertTDTemperToUi(const quint16& m_TDTemper, double& m_uiTDTemper,bool rtd=true);

    void convertTITemperFromUi(const double& m_uiTITemper, quint16& m_TITemper);
    void convertTITemperToUi(const quint16& m_TITemper, double& m_uiTITemper);

    void convertCOLUMNTemperFromUi(const double& m_uiCOLUMNTemper, quint16& m_COLUMNTemper);
    void convertCOLUMNTemperToUi(const quint16& m_COLUMNTemper, double& m_uiCOLUMNTemper);


    void convertMicroPIDValueToUi(const quint32& m_MicroPIDValue, double& m_uiMicroPIDValue);
};

#endif // CXATESTDATACONVERTOR_H
