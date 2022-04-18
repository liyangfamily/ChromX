#include "app.h"

QString App::ConfigFile = "config.ini";

//UI Param
QString App::lastOpenPath = "";
QString App::lastLanguage = "";
//UI Control
bool App::enableLog = true;
bool App::enableCommunicatLog = false;
//Control Device
QString App::mainCOM   = "";
QString App::assistCOM = "";

void App::readConfig()
{
    if (!checkConfig()) {
        return;
    }

    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("UIParam");
    App::lastOpenPath = set.value("LastOpenPath").toString();
    App::lastLanguage = set.value("LastLanguage").toString();
    set.endGroup();

    set.beginGroup("UIControl");
    if(set.contains("EnableLog")){
        App::enableLog = set.value("EnableLog").toBool();
    }
    if(set.contains("EnableCommunicatLog")){
        App::enableCommunicatLog = set.value("EnableCommunicatLog").toBool();
    }
    set.endGroup();

    set.beginGroup("CtrlDevice");
    App::mainCOM = set.value("Main").toString();
    App::assistCOM = set.value("Assist").toString();
    set.endGroup();
}

void App::writeConfig()
{
    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("UIParam");
    set.setValue("LastOpenPath", App::lastOpenPath);
    set.setValue("LastLanguage", App::lastLanguage);
    set.endGroup();

    set.beginGroup("UIControl");
    set.setValue("EnableLog", App::enableLog);
    set.setValue("EnableCommunicatLog", App::enableCommunicatLog);
    set.endGroup();

    set.beginGroup("CtrlDevice");
    set.setValue("Main", App::mainCOM);
    set.setValue("Assist", App::assistCOM);
    set.endGroup();
}

void App::newConfig()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#endif
    writeConfig();
}

bool App::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(App::ConfigFile);
    if (file.size() == 0) {
        newConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly)) {
        bool ok = true;
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2) {
                if (list.at(0) != "LastOpenPath"&& list.at(1) == "") {
                    ok = false;
                    break;
                }
            }
        }
    file.close();
        if (!ok) {
            newConfig();
            return false;
        }
    } else {
        newConfig();
        return false;
    }

    return true;
}
