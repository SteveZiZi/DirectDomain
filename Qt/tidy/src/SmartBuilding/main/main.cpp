#include <QtGui/QApplication>
#include <QTextCodec>
#include <QWSServer>
#include <QSplashScreen>
#include <QFile>

#include "src/SmartBuilding/main/SmartBuilding.h"
#include "src/vKeyBoard/vKeyBoard.h"
#include "src/SmartBuilding/main/content/common_ctl/application.h"

#if defined WIN32
#include "vld.h"
#endif

QString getStyleSheetFromFile(QString fileName)
{
    QFile file(fileName);

    QString styleSheet("");
    if (file.open(QFile::ReadOnly))  {
        styleSheet = QLatin1String(file.readAll());
        file.close();
    }

    return styleSheet;
}

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    a.setWindowIcon(QIcon(":/light.png"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

#ifndef WIN32
    QWSServer::setBackground(QBrush(QColor(8,16,49)));                  // 防止出现绿屏现象
    QWSServer::setCursorVisible(false);                                 // 隐藏鼠标箭头
#endif

    QPixmap pixmap(":/start.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();

    // 为应用导入软键盘
    vKeyBoard *vkeyBoard = new vKeyBoard;
    a.setInputContext(vkeyBoard);

    SmartBuilding w;
    w.setStyleSheet(getStyleSheetFromFile(":/samrtBuilding.qss"));
    w.show();
    return a.exec();
}
