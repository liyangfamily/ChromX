#include "icore.h"

#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QStatusBar>

#ifdef Q_CC_MSVC
#pragma execution_character_set("utf-8")
#endif

// The Core Singleton
static ICore *m_instance = nullptr;
static MainWindow *m_mainwindow = nullptr;


ICore::ICore(MainWindow *mw)
{
    m_instance = this;
    m_mainwindow = mw;
}

ICore::~ICore()
{
    m_instance = nullptr;
    m_mainwindow = nullptr;
}

ICore * ICore::instance()
{
    return m_instance;
}

QMainWindow * ICore::mainWindow()
{
    return m_mainwindow;
}

QStatusBar * ICore::statusBar()
{
    return m_mainwindow->statusBar();
}

void ICore::restart()
{
    //m_mainwindow->restart();
}

/*!
        Returns a widget pointer suitable to use as parent for QDialogs.
    */
QWidget *ICore::dialogParent()
{
    QWidget *active = QApplication::activeModalWidget();
    if (!active)
        active = QApplication::activeWindow();
    if (!active)
        active = m_mainwindow;
    return active;
}

/*!
        Raises and activates the window for \a widget. This contains workarounds
        for X11.
    */
void ICore::raiseWindow(QWidget *widget)
{
    if (!widget)
        return;
    QWidget *window = widget->window();
    if (window && window == m_mainwindow) {
        m_mainwindow->raise();
    } else {
        window->raise();
        window->activateWindow();
    }
}

void ICore::showRaise(QWidget *widget)
{
    if (!widget)
        return;
    widget->setWindowState(widget->windowState() & ~Qt::WindowMinimized);
    widget->raise();
    widget->activateWindow();
    widget->show();
}

void ICore::showCenter(QWidget *widget)
{
    if (!widget)
        return;
    QWidget* parent=qobject_cast<QWidget*>(widget->parent());
    if(!parent){
        parent = m_mainwindow;
    }
    QRect screenGeometry = QApplication::desktop()->screenGeometry(parent);
    int x = screenGeometry.left() + (screenGeometry.width()-widget->width()) / 2;
    int y = screenGeometry.top() + (screenGeometry.height()-widget->height()) / 2;
    widget->move(x, y);
    showRaise(widget);
}


/*!
        \internal
    */
static QString compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang " ) + QString::number(__clang_major__) + QLatin1Char('.')
            + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC " ) + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER > 1999)
        return QLatin1String("MSVC <unknown>");
    if (_MSC_VER >= 1920)
        return QLatin1String("MSVC 2019");
    if (_MSC_VER >= 1910)
        return QLatin1String("MSVC 2017");
    if (_MSC_VER >= 1900)
        return QLatin1String("MSVC 2015");
#endif
    return QLatin1String("<unknown compiler>");
}

void ICore::showMessageCCEAPIResult(quint16 ret, int msec)
{
    QString tip;
    switch((CCEAPI::EResult)ret){
    case CCEAPI::EResult::ER_Success:
        tip=tr("Successful operation.");
    break;
    case CCEAPI::EResult::ER_UnitAddrErr:
        tip=tr("Unit Address Error.");
    break;
    case CCEAPI::EResult::ER_CtrlAddrErr:
        tip=tr("Ctrl Address Error.");
    break;
    case CCEAPI::EResult::ER_FrameTypeErr:
        tip=tr("Frame Type Error.");
    break;
    case CCEAPI::EResult::ER_CRCErr:
        tip=tr("CRC Error.");
    break;
    case CCEAPI::EResult::ER_Fail:
        tip=tr("There is no reason for failure.");
    break;
    default:
        tip=tr("Error Code: 0x%1").arg(QString::number(ret,16).toUpper());
        break;
    }
    ICore::statusBar()->showMessage(tip, msec);
}

void ICore::showMessage(const QString &message, int msec)
{
    ICore::statusBar()->showMessage(message, msec);
}

