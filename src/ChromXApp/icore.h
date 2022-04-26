#pragma once

#include <QList>
#include <QMainWindow>
#include <QObject>
#include <QRect>
#include <QSettings>

#include <functional>
#include <CCE_Core/CCEAPIDef>
QT_BEGIN_NAMESPACE
class QStatusBar;
class QWidget;
QT_END_NAMESPACE

class ICore : public QObject
{
    Q_OBJECT

    friend class MainWindow;

    explicit ICore(MainWindow *mw);
    ~ICore()  override;

public:
    static ICore *instance();
    static QMainWindow *mainWindow();
    static QStatusBar *statusBar();
    static void restart();
    static QWidget *dialogParent();
    static void raiseWindow(QWidget *widget);

    static void showRaise(QWidget *widget);
    static void showCenter(QWidget *widget);
    //about
    static QString versionString();


    static void showMessageCCEAPIResult(quint16 ret, int msec = 2000);
    static void showMessage(const QString& message,int msec=2000);

public:
    /* internal use */

    static QString buildCompatibilityString();
};

