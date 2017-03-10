#include "logReportWidget.h"

#include <QLabel>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/inc/xtComAlg.h"
#include "src/SmartBuilding/main/content/common_ctl/reportWidget.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"


#ifndef WIN32
#define RECORD_CONFIG_FILE          "./dat/logconfig.dat"
#define BLACKBOX_CONFIG_FILE        "./dat/blackboxconfig.dat"
#define RECORD_FILE                 "/mnt/data/alarm/alarm.dat"
#define BLACKBOX_FILE               "/mnt/data/alarm/impalarm.dat"
#else 
#define RECORD_CONFIG_FILE          "F:\\dat\\logconfig.dat"
#define BLACKBOX_CONFIG_FILE        "F:\\dat\\blackboxconfig.dat"
#define RECORD_FILE                 "F:\\dat\\alarm.dat"
#define BLACKBOX_FILE               "F:\\dat\\impalarm.dat"
#endif
#define PER_UNIT_SIZE               32
#define MAX_LOG_COUNT               120

enum
{
    LEAKAGE = 0x0101,                                                   // 漏电流超限告警
    PARAMZERO = 0x0200,                                                 // 档案参数清理
    PARAMRESET = 0x0201,                                                // 档案参数变更
    RECTIFIERVSET = 0x0202,                                             // 整流器电压设置
    LOOPVSET = 0x0203,                                                  // 回路电压设置
    RECTIFIERSWITCHSET = 0x0208,                                        // 整流器开关控制
    LOOPSET = 0x020a,                                                   // 回路控制
    SUBLOOPCONTROL = 0x020d,                                            // 子回路控制
    CONNECTFALE = 0x020f,                                               // 通讯失败
    TASKCONTROLLOOPV = 0x0215,                                          // 时控任务控制回路电压
    TASKCONTROLSWITCH = 0x0216,                                         // 时控任务控制回路开关
    LIGHTCONTROLLOOPV = 0x0301,                                         // 光控任务控制回路电压
    LIGHTCONTROLLOOPOVER = 0x0302,                                      // 光控任务结束
    LIGHTCONTROLCLOSE = 0x0303,                                         // 光控任务关闭回路
    CONSTLUXCONTROLSTART = 0x0310,                                      // 恒照任务开始
    CONSTLUXCONTROLEND = 0x0311                                         // 恒照任务结束
};

bool compareLogContent(CLogContent &log1,CLogContent &log2)
{
    if(log1.time().toTime_t() >= log2.time().toTime_t()) // 将时间转化为秒进行转换
        return true;
    else
        return false;
}

CLogReportW::CLogReportW(S_LOGTYPE type,QWidget *parent)
: QWidget(parent)
, m_sLogType(type)
, m_iLogCount(0)
, m_pDataMgr(CDataMgr::getInstance())
{
    m_sCpcNo = m_pDataMgr->getCPCNo();
    init_();
    localConnect();

    parseConfig();
}

CLogReportW::~CLogReportW()
{

}

void CLogReportW::clearLogContent()
{
    m_pLogWidget->clearReportContent();
}

void CLogReportW::changePage(int action)
{
    int nCurScroller = m_pLogWidget->verticalScrollBar()->value();      // 获得当前scroller值

    if(QAbstractSlider::SliderSingleStepAdd == action)
    {
        int maxValue = m_pLogWidget->verticalScrollBar()->maximum();    // 当前SCROLLER最大显示值

        if(nCurScroller < maxValue)
            m_pLogWidget->verticalScrollBar()->setSliderPosition(8 + nCurScroller);
        else
            m_pLogWidget->verticalScrollBar()->setSliderPosition(maxValue);
    }
    else if(QAbstractSlider::SliderSingleStepSub == action)
    {
        if(nCurScroller > 0)
            m_pLogWidget->verticalScrollBar()->setSliderPosition(nCurScroller - 8);
        else
            m_pLogWidget->verticalScrollBar()->setSliderPosition(0);
    }
}

void CLogReportW::updateCount(int row,int,int,int)
{
    m_pPageLabel->setText(QString("%1/120").arg(QString::number(row + 1)));
}

void CLogReportW::readLog()
{
    QFile file;
    if (RECORD_TYPE == m_sLogType)
        file.setFileName(RECORD_FILE);
    else if (BLACKBOX_TYPE == m_sLogType)
        file.setFileName(BLACKBOX_FILE);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "open log.dat fail";
        return;
    }
    QByteArray arr = file.readAll();
    parselogContent(arr);

    qSort(m_lLogList.begin(),m_lLogList.end(),compareLogContent);       //  按时间进行排序
    showLogContent(m_lLogList);
    file.close();

    m_pLogWidget->verticalScrollBar()->setSliderPosition(0);
    m_pLogWidget->selectRow(0);
    m_pLogWidget->setFocus(Qt::MouseFocusReason);
}

void CLogReportW::init_()
{
    m_pLogWidget = new CReportWidget(this);
    m_pLogWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    m_pLogWidget->setColumnCount(3);
    m_pLogWidget->setColumnWidth(0,LOG_REPORT_DATA_W);
    m_pLogWidget->setColumnWidth(1,LOG_REPORT_TYPE_W);
    m_pLogWidget->setColumnWidth(2,LOG_REPORT_CONNECT_W - 17);
    QStringList header;
    header << tr("日志时间") << tr("类型") << tr("日志内容");
    m_pLogWidget->setHeaderContent(header);
    m_pLogWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为，以行为单位
    m_pLogWidget->horizontalHeader()->setStyleSheet("background-color: #4378ac;color: white;");  // bug 不明

    m_pPageLabel = new QLabel("0/120",this);
    m_pPageLabel->setObjectName("pageLabel");
    m_pFlashBtn = new QPushButton(tr("刷新"),this);

    layout_();
}

void CLogReportW::layout_()
{
    m_pLogWidget->setGeometry(LOG_REPORT_L,LOG_REPORT_T,
                              LOG_REPORT_W,LOG_REPORT_H);
    m_pPageLabel->setGeometry(LOG_PAGE_LABEL_L,LOG_PAGE_LABEL_T,
                              LOG_PAGE_LABEL_W,LOG_PAGE_LABEL_H);
    m_pFlashBtn->setGeometry(LOG_FLUSH_BTN_L,LOG_FLUSH_BTN_T,
                             LOG_FLUSH_BTN_W,LOG_FLUSH_BTN_H);
}

void CLogReportW::localConnect()
{
    connect(m_pLogWidget->verticalScrollBar(),SIGNAL(actionTriggered(int)),this,SLOT(changePage(int)));
    connect(m_pLogWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(updateCount(int,int,int,int)));
    connect(m_pFlashBtn,SIGNAL(clicked()),this,SLOT(readLog()));
}

void CLogReportW::parseConfig()
{
    QFile file;
    if (RECORD_TYPE == m_sLogType)
        file.setFileName(RECORD_CONFIG_FILE);
    else if (BLACKBOX_TYPE == m_sLogType)
        file.setFileName(BLACKBOX_CONFIG_FILE);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "open config.dat fail";
        return;
    }
    QTextStream in(&file);
    QString temp = in.readLine();
    while (!temp.isNull())
    {
        QStringList configList = temp.split(" : ");
        QString type = configList[0];
        QString content = configList[1];
        m_mConifgMap.insert(type,content);
        temp = in.readLine();
    }
    file.close();
}

void CLogReportW::parselogContent(QByteArray& content)
{
    m_iLogCount = ((int)content[0] << 8 ) | content[1];                 // 获取实际的日志条数
    if(m_iLogCount <= 0)
        return;

    m_lLogList.clear();

    // 报文的第一个字节、第二字节日志长度，所以真正的日志从第五个字节开始
    for(int i = 4;i < PER_UNIT_SIZE * m_iLogCount + 2;i = i + PER_UNIT_SIZE)
    {
        CLogContent logContent;
        QString year = QDate::currentDate().toString("yyyy-MM-dd");
        year = year.left(year.length() - 8);                            // 设置年份 因为只有几几年 前面的20要加上
        //日志内容的第八个字节为年，接下来依次为月，日，时，分
        QString yearForm = QString("%1%2-%3-%4 %5:%6:%7").arg(year)
                           .arg(content.at(i + 7), 2, 16, QChar('0'))
                           .arg(content.at(i + 8), 2, 16, QChar('0'))
                           .arg(content.at(i + 9), 2, 16, QChar('0'))
                           .arg(content.at(i + 10), 2, 16, QChar('0'))
                           .arg(content.at(i + 11), 2, 16, QChar('0'))
                           .arg("00");                                  // 00为秒

        logContent.setTime(QDateTime::fromString(yearForm,"yyyy-MM-dd hh:mm:ss"));

        QString typeValue = QString("%1%2").arg(content.at(i + 13), 2, 16, QChar('0'))
                                           .arg(content.at(i + 12), 2, 16, QChar('0'));

        QMap<QString,QString>::const_iterator cit = m_mConifgMap.find(typeValue);
        if(cit != m_mConifgMap.end())
        {
            QByteArray contextArr = content.mid(i + 14, i + 24);        // 获取日志内容
            logContent.setType(cit.value());                            // 获取类型
            if(logContent.type() == tr("时控任务#") || logContent.type() == tr("光控任务#")
               || logContent.type() == tr("恒照任务#"))
                logContent.setType(logContent.type() + QString::number(contextArr[1]));

            QString content = analyzeContent(typeValue,contextArr);
            logContent.setContent(content);                             // 获取内容

            m_lLogList.append(logContent);
        }
    }
}

QString CLogReportW::analyzeContent(QString& typeValue,QByteArray& contentArr)
{
    bool ok;
    QString content;
    QString tempVol;
    int precentNo = 0;
    unsigned short keepTime = 0;
    switch(typeValue.toLong(&ok,16))
    {
    case LEAKAGE:
        content = QString(tr("回路L#%1%2")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0')));
        if(contentArr[2])
            content += tr("漏电流超限");
        else
            content += tr("漏电流超限恢复");
        break;
    case PARAMZERO:                                                     // 档案参数清理
        content = tr("删除档案");
        break;
    case PARAMRESET:                                                    // 档案参数变更
        content = tr("加载档案");
        break;
    case RECTIFIERVSET:                                                 // 整流器电压设置
        break;
    case LOOPVSET:                                                      // 回路电压设置
        tempVol = QString("%1%2").arg(contentArr[2], 2, 10, QChar('0'))
                                 .arg(contentArr[3], 2, 10, QChar('0'));
        precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
        content = QString(tr("设置L#%1%2亮度至%3%,时效%4分钟")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                     .arg(QString::number(precentNo))
                     .arg(QString::number(contentArr[4])));
        break;
    case RECTIFIERSWITCHSET:                                            // 整流器开关控制
        content = "test1";
        break;
    case LOOPSET:                                                       // 回路控制
        if (contentArr[2]){
            tempVol = QString("%1%2").arg(contentArr[4], 2, 10, QChar('0'))
                                     .arg(contentArr[5], 2, 10, QChar('0'));
            precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
            content = QString(tr("设置L#%1%2打开亮度至%3%,时效%4分钟")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                         .arg(QString::number(precentNo))
                         .arg(QString::number(contentArr[3])));
        }
        else 
            content = QString(tr("设置L#%1%2关闭,时效%3分钟")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                         .arg(QString::number(contentArr[3])));
        break;
    case SUBLOOPCONTROL:                                                // 子回路控制
        content = "test2";
        break;
    case CONNECTFALE:                                                   // 通讯失败
        content = QString(tr("R#%1%2%3")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0')));
        if(contentArr[3])
            content += tr("掉线");
        else
            content += tr("上线");
        break;
    case TASKCONTROLLOOPV:                                              // 任务控制回路电压
        tempVol = QString("%1%2").arg(contentArr[3], 2, 10, QChar('0'))
                                 .arg(contentArr[4], 2, 10, QChar('0'));
        precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
        content = QString(tr("设置L#%1%2亮度至%3%")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                     .arg(QString::number(precentNo)));
    case TASKCONTROLSWITCH:                                             // 任务控制回路开关
        content = QString(tr("设置L#%1%2")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0')));
        if(contentArr[3])
            content += tr("打开");
        else
            content += tr("关闭");
        break;
    case LIGHTCONTROLLOOPV:                                             // 光控任务控制回路
        tempVol = QString("%1%2").arg(contentArr[3], 2, 10, QChar('0'))
                                 .arg(contentArr[4], 2, 10, QChar('0'));
        precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
        keepTime = (contentArr[5] << 8) | contentArr[6];
        content = QString(tr("设置L#%1%2亮度至%3%,时长%4分钟")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                     .arg(QString::number(precentNo))
                     .arg(QString::number(keepTime)));
        break;
    case LIGHTCONTROLLOOPOVER:
        content = QString(tr("光控任务#%1结束").arg(QString::number(contentArr[1])));
        break;
    case LIGHTCONTROLCLOSE:
        keepTime = (contentArr[4] << 8) | contentArr[5];
        if (contentArr[3])
            content = QString(tr("设置L#%1%2打开,时长%3分钟")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                         .arg(QString::number(keepTime)));
        else
            content = QString(tr("设置L#%1%2关闭,时长%3分钟")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                         .arg(QString::number(keepTime)));
        break;
    case CONSTLUXCONTROLSTART:
        content = tr("恒照任务开始");
        break;
    case CONSTLUXCONTROLEND:
        content = tr("恒照任务结束");
        break;
    default:
        break;
    }
    return content;
}


void CLogReportW::showLogContent(QList<CLogContent>& contentList)
{
    if(m_pLogWidget->rowCount() != MAX_LOG_COUNT)
    {
        m_pLogWidget->clearReportContent();
        int towCount = m_pLogWidget->rowCount();
        for(int i = 0;i < contentList.size();i++)
        {
            towCount += 1;
            m_pLogWidget->setRowCount(towCount);                        // 总行数增加1
            QString str;
            str = contentList[i].time().toString("yyyy-MM-dd hh:mm:ss");
            str = str.left(str.length() - 3);
            QTableWidgetItem *item0 = new QTableWidgetItem(str);
            item0->setTextAlignment(Qt::AlignLeft);
            m_pLogWidget->setItem(towCount - 1,0,item0);

            str = contentList[i].type();
            QTableWidgetItem *item1 = new QTableWidgetItem(str);
            item1->setTextAlignment(Qt::AlignLeft);
            m_pLogWidget->setItem(towCount - 1,1,item1);

            str = contentList[i].content();
            QTableWidgetItem *item2 = new QTableWidgetItem(str);
            item2->setTextAlignment(Qt::AlignLeft);
            m_pLogWidget->setItem(towCount - 1,2,item2);
        }
    }
    else
    {
        for(int i = 0;i < MAX_LOG_COUNT;i++)
        {
            QString str;
            str = contentList[i].time().toString("yyyy-MM-dd hh:mm:ss");
            str = str.left(str.length() - 3);
            m_pLogWidget->item(i,0)->setText(str);

            str = contentList[i].type();
            m_pLogWidget->item(i,1)->setText(str);

            str = contentList[i].content();
            m_pLogWidget->item(i,2)->setText(str);
        }
    }
}