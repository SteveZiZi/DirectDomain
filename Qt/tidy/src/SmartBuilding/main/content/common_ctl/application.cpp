#include "application.h"

#include <QTimer>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDateTime>
#include <QFile>
#include <QDebug>

#include "src/SmartBuilding/dataLevel/dataMgr.h"

#define SNAP_SCREEN_PATH                    "./snap/"
#define BRIGHTNESS_FILE                     "/sys/class/backlight/backlight/brightness"
#define MINETES                             10
#define LOOPTIMES                           5

Application::Application(int &argc, char **argv)
: QApplication(argc,argv)
, m_iBeat(0)
{
    init_();
    localConnect();
    openBackLight();
}

bool Application::notify(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        //qDebug() << "MouseButtonPress event******";
        if(m_pTimer->isActive())
        {
            //qDebug() << "Restart count****";
            m_iBeat = 0;
            m_pTimer->start();
        }
        else
        {
            //qDebug() << "OpenBackLight***";
            m_pTimer->start();
            int loop = LOOPTIMES;
            while(loop)
            {
                if(!openBackLight())
                    loop--;
                else
                    break;
            }
        }
    }
    // 双击保存图片到snap文件夹下    
//    else if(e->type() == QEvent::MouseButtonDblClick)
//    {
//        snapScreen();
//    }

    return QApplication::notify(obj,e);
}

void Application::onTimerOut()
{
    m_iBeat += 1;
    if(MINETES <= m_iBeat)
    {
        //qDebug() << "CloseBackLight***";
        m_pTimer->stop();
        m_iBeat = 0;
        int loop = LOOPTIMES;
        while(loop)
        {
            if(!closeBackLight())
                loop--;
            else
                break;
        }
    }
}

bool Application::closeBackLight()
{
    QFile file(BRIGHTNESS_FILE);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "open brightness fail";
        return false;
    }
    QTextStream out(&file);
    out << "0";
    file.close();

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open brightness fail";
        return false;
    }
    QTextStream in(&file);
    QString brightness = in.readAll();
    file.close();

    if(0 != brightness.toInt())
        return false;

    return true;
//    QString saverCmd = "echo 0 >> /sys/class/backlight/backlight/brightness";
//    system(saverCmd.toLatin1().data());
}

bool Application::openBackLight()
{
    QFile file(BRIGHTNESS_FILE);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "open brightness fail";
        return false;
    }
    QTextStream out(&file);
    out << "200";
    file.close();

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open brightness fail";
        return false;
    }
    QTextStream in(&file);
    QString brightness = in.readAll();
    file.close();

    if(200 != brightness.toInt())
    {
        return false;
    }

    return true;

//    QString saverCmd = "echo 200 >> /sys/class/backlight/backlight/brightness";
//    system(saverCmd.toLatin1().data());
}

void Application::snapScreen()
{
    QPixmap fullScreenPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    QString path(SNAP_SCREEN_PATH);
    QString snapScreenName;
    QString format(".png");
    QDateTime nowTime = QDateTime::currentDateTime();

    snapScreenName = path + nowTime.toString("yyyy-MM-dd hh:mm:ss") + format;
    fullScreenPixmap.save(snapScreenName,"PNG");
}

void Application::onQtTimer()
{
    CDataMgr::getInstance()->setQtTime();
}

void Application::init_()
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(60000);
    m_pTimer->start();

    m_pQtTimer = new QTimer(this);
    m_pQtTimer->setInterval(5000);
    m_pQtTimer->start();
}

void Application::localConnect()
{
    connect(m_pTimer, SIGNAL(timeout()),this,SLOT(onTimerOut()));
    connect(m_pQtTimer,SIGNAL(timeout()),this,SLOT(onQtTimer()));
}
