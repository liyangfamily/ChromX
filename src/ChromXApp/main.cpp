#include "mainwindow.h"

#include <QApplication>
#include "app.h"
#include <CCE_Core/CCEUIHelper>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Startup Config.
    App::ConfigFile = QString("%1/%2.ini").arg(CCEUIHelper::appConfigDataLocation()).arg(CCEUIHelper::appName());
    App::readConfig();
    App::writeConfig();

    MainWindow w;
    w.show();
    return a.exec();
}
