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
    LEAKAGE = 0x0101,                                                   // ©�������޸澯
    PARAMZERO = 0x0200,                                                 // ������������
    PARAMRESET = 0x0201,                                                // �����������
    RECTIFIERVSET = 0x0202,                                             // ��������ѹ����
    LOOPVSET = 0x0203,                                                  // ��·��ѹ����
    RECTIFIERSWITCHSET = 0x0208,                                        // ���������ؿ���
    LOOPSET = 0x020a,                                                   // ��·����
    SUBLOOPCONTROL = 0x020d,                                            // �ӻ�·����
    CONNECTFALE = 0x020f,                                               // ͨѶʧ��
    TASKCONTROLLOOPV = 0x0215,                                          // ʱ��������ƻ�·��ѹ
    TASKCONTROLSWITCH = 0x0216,                                         // ʱ��������ƻ�·����
    LIGHTCONTROLLOOPV = 0x0301,                                         // ���������ƻ�·��ѹ
    LIGHTCONTROLLOOPOVER = 0x0302,                                      // ����������
    LIGHTCONTROLCLOSE = 0x0303,                                         // �������رջ�·
    CONSTLUXCONTROLSTART = 0x0310,                                      // ��������ʼ
    CONSTLUXCONTROLEND = 0x0311                                         // �����������
};

bool compareLogContent(CLogContent &log1,CLogContent &log2)
{
    if(log1.time().toTime_t() >= log2.time().toTime_t()) // ��ʱ��ת��Ϊ�����ת��
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
    int nCurScroller = m_pLogWidget->verticalScrollBar()->value();      // ��õ�ǰscrollerֵ

    if(QAbstractSlider::SliderSingleStepAdd == action)
    {
        int maxValue = m_pLogWidget->verticalScrollBar()->maximum();    // ��ǰSCROLLER�����ʾֵ

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

    qSort(m_lLogList.begin(),m_lLogList.end(),compareLogContent);       //  ��ʱ���������
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
    header << tr("��־ʱ��") << tr("����") << tr("��־����");
    m_pLogWidget->setHeaderContent(header);
    m_pLogWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊ������Ϊ��λ
    m_pLogWidget->horizontalHeader()->setStyleSheet("background-color: #4378ac;color: white;");  // bug ����

    m_pPageLabel = new QLabel("0/120",this);
    m_pPageLabel->setObjectName("pageLabel");
    m_pFlashBtn = new QPushButton(tr("ˢ��"),this);

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
    m_iLogCount = ((int)content[0] << 8 ) | content[1];                 // ��ȡʵ�ʵ���־����
    if(m_iLogCount <= 0)
        return;

    m_lLogList.clear();

    // ���ĵĵ�һ���ֽڡ��ڶ��ֽ���־���ȣ�������������־�ӵ�����ֽڿ�ʼ
    for(int i = 4;i < PER_UNIT_SIZE * m_iLogCount + 2;i = i + PER_UNIT_SIZE)
    {
        CLogContent logContent;
        QString year = QDate::currentDate().toString("yyyy-MM-dd");
        year = year.left(year.length() - 8);                            // ������� ��Ϊֻ�м����� ǰ���20Ҫ����
        //��־���ݵĵڰ˸��ֽ�Ϊ�꣬����������Ϊ�£��գ�ʱ����
        QString yearForm = QString("%1%2-%3-%4 %5:%6:%7").arg(year)
                           .arg(content.at(i + 7), 2, 16, QChar('0'))
                           .arg(content.at(i + 8), 2, 16, QChar('0'))
                           .arg(content.at(i + 9), 2, 16, QChar('0'))
                           .arg(content.at(i + 10), 2, 16, QChar('0'))
                           .arg(content.at(i + 11), 2, 16, QChar('0'))
                           .arg("00");                                  // 00Ϊ��

        logContent.setTime(QDateTime::fromString(yearForm,"yyyy-MM-dd hh:mm:ss"));

        QString typeValue = QString("%1%2").arg(content.at(i + 13), 2, 16, QChar('0'))
                                           .arg(content.at(i + 12), 2, 16, QChar('0'));

        QMap<QString,QString>::const_iterator cit = m_mConifgMap.find(typeValue);
        if(cit != m_mConifgMap.end())
        {
            QByteArray contextArr = content.mid(i + 14, i + 24);        // ��ȡ��־����
            logContent.setType(cit.value());                            // ��ȡ����
            if(logContent.type() == tr("ʱ������#") || logContent.type() == tr("�������#")
               || logContent.type() == tr("��������#"))
                logContent.setType(logContent.type() + QString::number(contextArr[1]));

            QString content = analyzeContent(typeValue,contextArr);
            logContent.setContent(content);                             // ��ȡ����

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
        content = QString(tr("��·L#%1%2")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0')));
        if(contentArr[2])
            content += tr("©��������");
        else
            content += tr("©�������޻ָ�");
        break;
    case PARAMZERO:                                                     // ������������
        content = tr("ɾ������");
        break;
    case PARAMRESET:                                                    // �����������
        content = tr("���ص���");
        break;
    case RECTIFIERVSET:                                                 // ��������ѹ����
        break;
    case LOOPVSET:                                                      // ��·��ѹ����
        tempVol = QString("%1%2").arg(contentArr[2], 2, 10, QChar('0'))
                                 .arg(contentArr[3], 2, 10, QChar('0'));
        precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
        content = QString(tr("����L#%1%2������%3%,ʱЧ%4����")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                     .arg(QString::number(precentNo))
                     .arg(QString::number(contentArr[4])));
        break;
    case RECTIFIERSWITCHSET:                                            // ���������ؿ���
        content = "test1";
        break;
    case LOOPSET:                                                       // ��·����
        if (contentArr[2]){
            tempVol = QString("%1%2").arg(contentArr[4], 2, 10, QChar('0'))
                                     .arg(contentArr[5], 2, 10, QChar('0'));
            precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
            content = QString(tr("����L#%1%2��������%3%,ʱЧ%4����")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                         .arg(QString::number(precentNo))
                         .arg(QString::number(contentArr[3])));
        }
        else 
            content = QString(tr("����L#%1%2�ر�,ʱЧ%3����")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                         .arg(QString::number(contentArr[3])));
        break;
    case SUBLOOPCONTROL:                                                // �ӻ�·����
        content = "test2";
        break;
    case CONNECTFALE:                                                   // ͨѶʧ��
        content = QString(tr("R#%1%2%3")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[1]), 2, 10, QChar('0'))
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0')));
        if(contentArr[3])
            content += tr("����");
        else
            content += tr("����");
        break;
    case TASKCONTROLLOOPV:                                              // ������ƻ�·��ѹ
        tempVol = QString("%1%2").arg(contentArr[3], 2, 10, QChar('0'))
                                 .arg(contentArr[4], 2, 10, QChar('0'));
        precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
        content = QString(tr("����L#%1%2������%3%")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                     .arg(QString::number(precentNo)));
    case TASKCONTROLSWITCH:                                             // ������ƻ�·����
        content = QString(tr("����L#%1%2")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0')));
        if(contentArr[3])
            content += tr("��");
        else
            content += tr("�ر�");
        break;
    case LIGHTCONTROLLOOPV:                                             // ���������ƻ�·
        tempVol = QString("%1%2").arg(contentArr[3], 2, 10, QChar('0'))
                                 .arg(contentArr[4], 2, 10, QChar('0'));
        precentNo = m_pDataMgr->getCloseFunNo(contentArr[1],tempVol.toInt() / 10);
        keepTime = (contentArr[5] << 8) | contentArr[6];
        content = QString(tr("����L#%1%2������%3%,ʱ��%4����")
                     .arg(m_sCpcNo)
                     .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                     .arg(QString::number(precentNo))
                     .arg(QString::number(keepTime)));
        break;
    case LIGHTCONTROLLOOPOVER:
        content = QString(tr("�������#%1����").arg(QString::number(contentArr[1])));
        break;
    case LIGHTCONTROLCLOSE:
        keepTime = (contentArr[4] << 8) | contentArr[5];
        if (contentArr[3])
            content = QString(tr("����L#%1%2��,ʱ��%3����")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                         .arg(QString::number(keepTime)));
        else
            content = QString(tr("����L#%1%2�ر�,ʱ��%3����")
                         .arg(m_sCpcNo)
                         .arg(DectoBCD(contentArr[2]), 2, 10, QChar('0'))
                         .arg(QString::number(keepTime)));
        break;
    case CONSTLUXCONTROLSTART:
        content = tr("��������ʼ");
        break;
    case CONSTLUXCONTROLEND:
        content = tr("�����������");
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
            m_pLogWidget->setRowCount(towCount);                        // ����������1
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