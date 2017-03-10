#include "monitorWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QFrame>
#include <QTimer>
#include <QDebug>
#include <QHeaderView>
#include <QIntValidator>
#include <QCoreApplication>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/inc/xtUtility.h"
#include "src/SmartBuilding/main/content/common_ctl/popSureWidget.h"
#include "src/SmartBuilding/main/content/common_ctl/reportWidget.h"
#include "src/SmartBuilding/main/topBar/topBarEvent.h"
#include "src/SmartBuilding/main/content/common_ctl/toolTip.h"

#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/dataLevel/cabinet.h"
#include "src/SmartBuilding/dataLevel/circuit.h"
#include "src/SmartBuilding/dataLevel/rectifier.h"

#define UPDATE_TIME                     500
#define DELAY_TIME                      500
#define MIN_VOL                         100
#define CIRCUIT_COUNT                   16

inline void printTime()
{
    QDateTime d = QDateTime::currentDateTime();

    qDebug() << d.time().hour() << " "
        << d.time().minute() << " "
        << d.time().second() << " "
        << d.time().msec();
}

CMonitorW::CMonitorW(QWidget *parent)
: QWidget(parent)
, m_bTurning(false)
, m_bSwitched(false)
, m_bShowRectifer(false)
, m_iCurrentRow(-1)
, m_iCurrentColumn(-1)
, m_pDataMgr(CDataMgr::getInstance())
{
    m_sCpcNo = m_pDataMgr->getCPCNo();
    init_();
    localConnect();
}

CMonitorW::~CMonitorW()
{

}

void CMonitorW::changeState(int state)
{
    bool flag;
    if (Qt::Checked == state){
        m_pRightFrame->setEnabled(true);
        flag = false;
    }
    else if (Qt::Unchecked == state){
        m_pRightFrame->setEnabled(false);
        flag = true;
    }

    QCoreApplication::postEvent(this->parent()->parent()->parent(), new CTopBarEvent(flag));
}

void CMonitorW::addTenPrecent()
{
    if (m_pTimeLiness->text().toInt() <= 0){
        popToolTip(m_pTimeLiness->mapToGlobal(QPoint(0, 0)),tr("时效必须大于0分钟!"));
        return;
    }
    setAging();                                                         // 设置时效

    int precent = m_pDimValue->text().remove(" %").toInt();
    precent += 10;
    if(precent > 100)
        precent = 100;

    m_pDimValue->setText(QString("%1 %").arg(QString::number(precent)));

    dimming(11 - precent / 10,m_pDimComBox->currentIndex() + 1);
}

void CMonitorW::subTenPrecent()
{
    if (m_pTimeLiness->text().toInt() <= 0){
        popToolTip(m_pTimeLiness->mapToGlobal(QPoint(0, 0)),tr("时效必须大于0分钟!"));
        return;
    }
    setAging();                                                         // 设置时效

    int precent = m_pDimValue->text().remove(" %").toInt();
    precent -= 10;
    if(precent < 0)
        precent = 0;

    m_pDimValue->setText(QString("%1 %").arg(QString::number(precent)));

    dimming(11 - precent / 10,m_pDimComBox->currentIndex() + 1);
}

void CMonitorW::turnOnOff()
{
    if (m_bTurning)
        return;

    if (m_pTimeLiness->text().toInt() <= 0){
        popToolTip(m_pTimeLiness->mapToGlobal(QPoint(0, 0)),tr("时效必须大于0分钟!"));
        return;
    }

    m_bTurning = true;

    if (m_pDataMgr->getCircuitWarnState(m_pDimComBox->currentIndex() + 1)){
        popToolTip(m_pSwitchBtn->mapToGlobal(QPoint(0, 0)),tr("此回路发生漏电，请勿操作!"));
        m_bTurning = false;
        return;
    }

    setAging();                                                         // 设置时效

    INT8U dataBuf[4] = {0};
    if (m_bSwitched)
    {
        // 关闭回路
        if (popSwitchCtrlDlg(tr("关闭回路？"), tr("是否关闭选中回路")) != QMessageBox::Cancel){
            m_pDataMgr->sendCommands(TURN_OFF_ORDER_CMD,dataBuf,m_pDimComBox->currentIndex() + 1);
            m_bSwitched = false;
            setSwitchState(m_bSwitched);
        }
    }
    else
    {
        // 打开回路
        if (popSwitchCtrlDlg(tr("打开回路?"), tr("是否打开选中回路")) != QMessageBox::Cancel){
            m_pDataMgr->sendCommands(TURN_ON_ORDER_CMD,dataBuf,m_pDimComBox->currentIndex() + 1);
            m_bSwitched = true;
            setSwitchState(m_bSwitched);
        }
    }
    m_bTurning = false;
}

void CMonitorW::getCircuitInfo(int circuitNo)
{
    CCabinet* cabinet = m_pDataMgr->getIpcData();
    CCircuit* circuit = cabinet->findCircuit(circuitNo + 1);
    if (circuit)
    {
        int precent = m_pDataMgr->getCloseFunNo(circuit->circuitNo(),(int)circuit->volgeta()) * 10;

        m_pDimValue->setText(QString("%1 %").arg(QString::number(precent)));

        if (circuit->volgeta() < MIN_VOL)
            setSwitchState(false);
        else
            setSwitchState(true);
    }
    else
    {
        m_pDimValue->setText("0 %");
        setSwitchState(false);
    }
}

void CMonitorW::backSlot()
{
    m_bShowRectifer = false;
    connect(m_pDataReport,SIGNAL(cellClicked(int,int)),this,SLOT(showRectifierData(int,int)));

    m_pBackBtn->hide();
}

void CMonitorW::updateShow()
{
    CCabinet *cabinet = m_pDataMgr->getIpcData();
    setCabinetData(cabinet);

    if (!m_bShowRectifer)
    {
        QStringList header;
        header << tr("C-L") << tr("电压(V)") << tr("电流(A)") << tr("功率(W)") << tr("温度(℃)") << tr("状态");
        m_pDataReport->setHorizontalHeaderLabels(header);
        if (m_pDataReport->rowCount() != cabinet->circuitCount())
        {
            m_pDataReport->clearReportContent();
            for (int rowCount = 0;rowCount < cabinet->circuitCount();rowCount++)
            {
                m_pDataReport->addOneRow();
                CCircuit* circuit = cabinet->findCircuit(rowCount);
                setCircuitData(rowCount,circuit);
            }
        }
        else
        {
            for (int rowCount = 0;rowCount < cabinet->circuitCount();rowCount++)
            {
                CCircuit* circuit = cabinet->findCircuit(rowCount);
                setCircuitData(rowCount,circuit);
            }
        }
        m_pDataReport->setColumnColor(0,"#6ad125");
    }
    else
    {
        showRectifierData(m_iCurrentRow,m_iCurrentColumn);
    }

    m_pTotalPower->setText(m_pDataMgr->getTotalData());
}

void CMonitorW::showRectifierData(int row,int column)
{
    if (column != 0)
        return;

    if (!m_bShowRectifer)
        disconnect(m_pDataReport,SIGNAL(cellClicked(int,int)),this,SLOT(showRectifierData(int,int)));

    m_bShowRectifer = true;
    m_iCurrentRow = row;
    m_iCurrentColumn = column;
    m_pBackBtn->show();

    QStringList header;
    header << tr("L-R") << tr("电压(V)") << tr("电流(A)") << tr("功率(W)") << tr("温度(℃)") << tr("状态");
    m_pDataReport->setHorizontalHeaderLabels(header);
    CCabinet *cabinet = m_pDataMgr->getIpcData();
    CCircuit* circuit = cabinet->findCircuit(row);

    if (m_pDataReport->rowCount() != circuit->rectifierCount())
    {
        m_pDataReport->clearReportContent();
        for (int i = 0;i < circuit->rectifierCount();i++)
        {
            m_pDataReport->addOneRow();
            CRectifier* rectifier = circuit->findRectifier(i);
            setRectifierData(i,rectifier);
        }
    }
    else
    {
        for (int i = 0;i < circuit->rectifierCount();i++)
        {
            CRectifier* rectifier = circuit->findRectifier(i);
            setRectifierData(i,rectifier);
        }
    }
}

void CMonitorW::showWarnDetail(QTableWidgetItem* item)
{
    if (!m_bShowRectifer || item->column() != 5)
        return;

    QString warning = item->text();
    popToolTip(QCursor::pos(),warning);
}

void CMonitorW::init_()
{
    m_pCabinetReport = new CReportWidget(this);
    m_pCabinetReport->setObjectName("cabinetReport");
    m_pCabinetReport->createReportWidget(1,6);
    m_pCabinetReport->showHorizontalHeader(false);
    m_pCabinetReport->setSelectionMode(QAbstractItemView::NoSelection);

    m_pDataReport = new CReportWidget(this);
    m_pDataReport->setColumnCount(6);
    QStringList header;
    header << "C-L" << tr("电压(V)") << tr("电流(A)")
           << tr("功率(W)") <<tr("温度(℃)") << tr("状态");
    m_pDataReport->setHeaderContent(header);
    m_pDataReport->horizontalHeader()->setStyleSheet("background-color: #4378ac;color: white;");  // bug 不明

    m_pManualCheck = new QCheckBox(tr("手动控制"),this);
    m_pManualCheck->setObjectName("manualCheck");
    m_pRightFrame = new QFrame(this);
    m_pRightFrame->setEnabled(false);
    m_pDimComBox = new QComboBox(m_pRightFrame);
    QStringList dimPrecent;
    for (int i = 0;i < CIRCUIT_COUNT;i++)
    {
        m_pDimComBox->addItem(QString("L#%1%2")
                            .arg(m_sCpcNo)
                            .arg(i + 1, 2, 10, QChar('0')));
    }
    m_pTimeLiness = new QLineEdit("3",m_pRightFrame);
    m_pTimeLiness->setValidator(new QIntValidator(0,5000,this));
    m_pDimValue = new QLineEdit("0 %",m_pRightFrame);
    m_pDimValue->setEnabled(false);
    m_pTenAddBtn = new QPushButton(m_pRightFrame);
    m_pTenAddBtn->setObjectName("tenAddBtn");
    m_pTenSubBtn = new QPushButton(m_pRightFrame);
    m_pTenSubBtn->setObjectName("tenSubBtn");
    m_pSwitchBtn = new QPushButton(m_pRightFrame);
    m_pSwitchBtn->setObjectName("switchBtn");

    m_pTotalPower = new QLineEdit(this);
    m_pTotalPower->setEnabled(false);
    m_pBackBtn = new QPushButton(tr("返回"),this);
    m_pBackBtn->hide();

    m_pUpdateTimer = new QTimer(this);
    m_pUpdateTimer->start(UPDATE_TIME);

    m_pDataMgr->getWarnList(m_lWarnList);                               // 获取告警信息

    layout_();

    getCircuitInfo(0);
}

void CMonitorW::layout_()
{
    m_pCabinetReport->setGeometry(CABINET_REPORT_L,CABINET_REPORT_T,
                                  CABINET_REPORT_W,CABINET_REPORT_H);
    m_pDataReport->setGeometry(DATA_REPORT_L,DATA_REPORT_T,
                               DATA_REPORT_W,DATA_REPORT_H);

    m_pManualCheck->setGeometry(MANUAL_CHECK_L,MANUAL_CHECK_T,
                                MANUAL_CHECK_W,MANUAL_CHECK_H);
    m_pRightFrame->setGeometry(RIGHT_FRAME_L,RIGHT_FRAME_T,
                               RIGHT_FRAME_W,RIGHT_FRAME_H);

    QLabel* timeLabel = new QLabel(tr("时效(分钟)"),m_pRightFrame);
    timeLabel->setGeometry(TIME_LABEL_L,TIME_LABEL_T,
                           TIME_LABEL_W,TIME_LABEL_H);
    m_pTimeLiness->setGeometry(TIME_LINEEDIT_L,TIME_LINEEDIT_T,
                               TIME_LINEEDIT_W,TIME_LINEEDIT_H);

    QLabel* dimLabel = new QLabel(tr("调光"),m_pRightFrame);
    dimLabel->setGeometry(DIM_LABEL_L,DIM_LABEL_T,
                          DIM_LABEL_W,DIM_LABEL_H);
    m_pDimComBox->setGeometry(DIM_COMBOX_L,DIM_COMBOX_T,
                              DIM_COMBOX_W,DIM_COMBOX_H);
    m_pDimValue->setGeometry(DIM_LINEEDIT_L,DIM_LINEEDIT_T,
                             DIM_LINEEDIT_W,DIM_LINEEDIT_H);
    m_pTenAddBtn->setGeometry(TEN_ADD_BTN_L,TEN_ADD_BTN_T,
                              TEN_ADD_BTN_W,TEN_ADD_BTN_H);
    m_pTenSubBtn->setGeometry(TEN_SUB_BTN_L,TEN_SUB_BTN_T,
                              TEN_SUB_BTN_W,TEN_SUB_BTN_H);
    m_pSwitchBtn->setGeometry(SWITCH_BTN_L,SWITCH_BTN_T,
                              SWITCH_BTN_W,SWITCH_BTN_H);

    QLabel* totalLabel = new QLabel(tr("累计用电量(kWh)"),this);
    totalLabel->setGeometry(TOTAL_LABEL_L,TOTAL_LABEL_T,
                            TOTAL_LABEL_W,TOTAL_LABEL_H);
    m_pTotalPower->setGeometry(TOTAL_LINEEDIT_L,TOTAL_LINEEDIT_T,
                               TOTAL_LINEEDIT_W,TOTAL_LINEEDIT_H);

    m_pBackBtn->setGeometry(DATA_BACK_BTN_L,DATA_BACK_BTN_T,
                            DATA_BACK_BTN_W,DATA_BACK_BTN_H);
}

void CMonitorW::localConnect()
{
    connect(m_pManualCheck,SIGNAL(stateChanged(int)),this,SLOT(changeState(int)));
    connect(m_pTenAddBtn,SIGNAL(clicked()),this,SLOT(addTenPrecent()));
    connect(m_pTenSubBtn,SIGNAL(clicked()),this,SLOT(subTenPrecent()));
    connect(m_pSwitchBtn,SIGNAL(clicked()),this,SLOT(turnOnOff()));
    connect(m_pBackBtn,SIGNAL(clicked()),this,SLOT(backSlot()));
    connect(m_pDimComBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getCircuitInfo(int)));
    connect(m_pUpdateTimer,SIGNAL(timeout()),this,SLOT(updateShow()));
    connect(m_pDataReport,SIGNAL(cellClicked(int,int)),this,SLOT(showRectifierData(int,int)));
    connect(m_pDataReport,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(showWarnDetail(QTableWidgetItem*)));
}

void CMonitorW::setAging()
{
    unsigned long minutes = m_pTimeLiness->text().toInt();
    time_t minValue = QDateTime::currentDateTime().toTime_t() + minutes * 60;
    INT8U dataBuf[4] = {0};
    for (int i = 0;i < 4;i++)
    {
        dataBuf[i] = (INT8U)(minValue >> (8 * (3 - i)));
    }
    m_pDataMgr->sendCommands(SET_TIMING_CMD,dataBuf);

    xt_msleep(DELAY_TIME);
}

void CMonitorW::dimming(int precent,int circuitNo)
{
    QString vol = m_pDataMgr->getPrecentVol(precent,circuitNo);
    INT8U dataBuf[4] = {0};
    bool ok;
    dataBuf[0] = vol.left(2).toInt(&ok,16);
    dataBuf[1] = vol.right(2).toInt(&ok,16);
    m_pDataMgr->sendCommands(ADJUST_ORDER_CMD,dataBuf);
}

void CMonitorW::setSwitchState(bool state)
{
    if (!state)
        m_pSwitchBtn->setStyleSheet("background-image: url(:/turnOff.png);");
    else
        m_pSwitchBtn->setStyleSheet("background-image: url(:/turnOn.png);");
}

void CMonitorW::setCabinetData(CCabinet* cabinet)
{
    m_pCabinetReport->item(0,0)->setText(QString("C#%1").arg(cabinet->cabinetNo(), 2, 10, QChar('0')));
    m_pCabinetReport->item(0,1)->setText(QString::number(cabinet->volgeta(),'f',1));
    m_pCabinetReport->item(0,2)->setText(QString::number(cabinet->current(),'f',1));
    m_pCabinetReport->item(0,3)->setText(QString::number(cabinet->power(),'f',2));
    if (cabinet->temp() < 0)
        m_pCabinetReport->item(0,4)->setText("--");
    else
        m_pCabinetReport->item(0,4)->setText(QString::number(cabinet->temp(),'f',1));

    if (cabinet->state())
        m_pCabinetReport->item(0,5)->setText(tr("异常"));
    else
        m_pCabinetReport->item(0,5)->setText(tr("正常"));
}

void CMonitorW::setCircuitData(int row,CCircuit* circuit)
{
    m_pDataReport->item(row,0)->setText(QString("L#%1%2").arg(m_sCpcNo).arg(circuit->circuitNo(), 2, 10, QChar('0')));
    m_pDataReport->item(row,1)->setText(QString::number(circuit->volgeta(),'f',1));
    m_pDataReport->item(row,2)->setText(QString::number(circuit->current(),'f',1));
    m_pDataReport->item(row,3)->setText(QString::number(circuit->power(),'f',2));
    if (circuit->temp() < 0)
        m_pDataReport->item(row,4)->setText("--");
    else
        m_pDataReport->item(row,4)->setText(QString::number(circuit->temp(),'f',1));

    if (circuit->state())
        m_pDataReport->item(row,5)->setText(tr("异常"));
    else
        m_pDataReport->item(row,5)->setText(tr("正常"));
}

void CMonitorW::setRectifierData(int row,CRectifier* rectifier)
{
    m_pDataReport->item(row,0)->setText(QString("R#%1%2%3").arg(m_sCpcNo)
                               .arg(rectifier->circuitNo(), 2, 10, QChar('0'))
                               .arg(rectifier->deviceNo(), 2, 10, QChar('0')));
    m_pDataReport->item(row,1)->setText(QString::number(rectifier->volgeta(),'f',1));
    m_pDataReport->item(row,2)->setText(QString::number(rectifier->current(),'f',1));
    m_pDataReport->item(row,3)->setText(QString::number(rectifier->power(),'f',2));
    if (rectifier->temp() < 0)
        m_pDataReport->item(row,4)->setText("--");
    else
        m_pDataReport->item(row,4)->setText(QString::number(rectifier->temp(),'f',1));

    m_pDataReport->item(row,5)->setText(getRectifierState(rectifier->state()));
}

QString CMonitorW::getRectifierState(INT16S state)
{
    QString warnStr;
    if(0 == state)
        warnStr = tr("正常");
    if(0x0001 & state)
        warnStr = m_lWarnList[0];
    if(0x0002 & state)
        warnStr += m_lWarnList[1];
    if(0x0004 & state)
        warnStr += m_lWarnList[2];
    if(0x0008 & state)
        warnStr += m_lWarnList[3];
    if(0x0010 & state)
        warnStr += m_lWarnList[4];
    if(0x0020 & state)
        warnStr += m_lWarnList[5];
    if(0x0040 & state)
        warnStr += m_lWarnList[6];
    if(0x0080 & state)
        warnStr += m_lWarnList[7];
    if(0x0100 & state)
        warnStr = m_lWarnList[8];
    if(0x0200 & state)
        warnStr += m_lWarnList[9];
    if(0x0400 & state)
        warnStr += m_lWarnList[10];
    if(0x0800 & state)
        warnStr += m_lWarnList[11];
    if(0x1000 & state)
        warnStr += m_lWarnList[12];
    if(0x2000 & state)
        warnStr += m_lWarnList[13];
    if(0x4000 & state)
        warnStr += m_lWarnList[14];
    if(0x8000 & state)
        warnStr += m_lWarnList[15];

    return warnStr;
}