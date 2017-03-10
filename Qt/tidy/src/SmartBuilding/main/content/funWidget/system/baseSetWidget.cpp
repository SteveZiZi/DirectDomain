#include "baseSetWidget.h"

#include <QPushButton>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QLabel>
#include <QFrame>
#include <QRegExpValidator>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/main/content/common_ctl/toolTip.h"

CBaseSetW::CBaseSetW(QWidget *parent)
: QWidget(parent)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
    getTelNum();
    readInfo();
}

CBaseSetW::~CBaseSetW()
{

}

void CBaseSetW::timing()
{
    m_pDataMgr->syncTime(m_pDateTime->dateTime());
}

void CBaseSetW::getTelNum()
{
    m_pTelNumEdit->setText(m_pDataMgr->getTelNo());
}

void CBaseSetW::setTelNum()
{
    if (m_pTelNumEdit->text().length() != 11)
    {
        popToolTip(m_pTelNumBtn->mapToGlobal(QPoint(0, 0)),tr("号码位数错误!"));
        return;
    }

    m_pDataMgr->setTelNo(m_pTelNumEdit->text());
}

void CBaseSetW::readInfo()
{
    QStringList strList;
    m_pDataMgr->getIpInfo(strList);

    if (strList.size() != 5)
        return;

    m_pIpAddEdit->setText(strList[0]);
    m_pMaskAddEdit->setText(strList[1]);
    m_pGateWayEdit->setText(strList[2]);
    m_pServerAddEdit->setText(strList[3]);
    m_pPortEdit->setText(strList[4]);
}

void CBaseSetW::saveInfo()
{
    QStringList strList;
    strList.append(m_pIpAddEdit->text());
    strList.append(m_pMaskAddEdit->text());
    strList.append(m_pGateWayEdit->text());
    strList.append(m_pServerAddEdit->text());
    strList.append(m_pPortEdit->text());

    if (m_pDataMgr->setIpInfo(strList))
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("号码位数错误!"));
}

void CBaseSetW::clearTotalData()
{

}

void CBaseSetW::init_()
{
    m_pFrame = new QFrame(this);
    m_pDateTime = new QDateTimeEdit(QDateTime::currentDateTime(),m_pFrame);
    m_pDateTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    m_pTimingBtn = new QPushButton(tr("校 时"),m_pFrame);
    m_pTelNumBtn = new QPushButton(tr("管 理 员 电 话"),m_pFrame);
    m_pSetTelNumBtn = new QPushButton(tr("更 改"),m_pFrame);
    m_pClearTotalBtn = new QPushButton(tr("清空累计用电量"),this);
    m_pReadBtn = new QPushButton(tr("读取"),this);
    m_pSaveBtn = new QPushButton(tr("保存"),this);

    m_pTelNumEdit = new QLineEdit(m_pFrame);
    QRegExp regExp("[0-9]{0,11}");
    m_pTelNumEdit->setValidator(new QRegExpValidator(regExp,this));
    QRegExp regExpip("\\b(([01]?\\d?\\d|2[0-4]\\d|25[0-5])\\.){3}([01]?\\d?\\d|2[0-4]\\d|25[0-5])\\b");
    QRegExpValidator* ipValid = new QRegExpValidator(regExpip,this);
    m_pIpAddEdit = new QLineEdit(m_pFrame);
    m_pMaskAddEdit = new QLineEdit(m_pFrame);
    m_pGateWayEdit = new QLineEdit(m_pFrame);
    m_pServerAddEdit = new QLineEdit(m_pFrame);
    m_pPortEdit = new QLineEdit(m_pFrame);
    m_pLogicAddLabel = new QLabel(m_pFrame);
    m_pLogicAddLabel->setText(m_pDataMgr->getLogicAddr());
    m_pSoftVerLabel = new QLabel(m_pFrame);
    m_pSoftVerLabel->setText(QString(tr("软 件 版 本 : V%1")).arg(m_pDataMgr->getSoftVersion()));

    m_pIpAddEdit->setValidator(ipValid);
    m_pMaskAddEdit->setValidator(ipValid);
    m_pMaskAddEdit->setValidator(ipValid);
    m_pGateWayEdit->setValidator(ipValid);
    m_pServerAddEdit->setValidator(ipValid);
    m_pPortEdit->setValidator(new QIntValidator(0,60000,this));

    layout_();
}

void CBaseSetW::layout_()
{
    m_pFrame->setGeometry(SYSTEM_FRAME_L,SYSTEM_FRAME_T,
                          SYSTEM_FRAME_W,SYSTEM_FRAME_H);
    m_pReadBtn->setGeometry(SYSTEM_READ_BTN_L,SYSTEM_READ_BTN_T,
                            SYSTEM_READ_BTN_W,SYSTEM_READ_BTN_H);
    m_pSaveBtn->setGeometry(SYSTEM_SAVE_BTN_L,SYSTEM_SAVE_BTN_T,
                            SYSTEM_SAVE_BTN_W,SYSTEM_SAVE_BTN_H);
    m_pClearTotalBtn->setGeometry(SYSTEM_CLEAR_BTN_L,SYSTEM_CLEAR_BTN_T,
                                  SYSTEM_CLEAR_BTN_W,SYSTEM_CLEAR_BTN_H);

    m_pTimingBtn->setGeometry(TIMING_BTN_L,TIMING_BTN_T,
                              TIMING_BTN_W,TIMING_BTN_H);
    m_pDateTime->setGeometry(TIMING_EDIT_L,TIMING_EDIT_T,
                             TIMING_EDIT_W,TIMING_EDIT_H);
    m_pSoftVerLabel->setGeometry(SOFT_VER_LABEL_L,SOFT_VER_LABEL_T,
                                 SOFT_VER_LABEL_W,SOFT_VER_LABEL_H);

    m_pTelNumBtn->setGeometry(TEL_NUM_BTN_L,TEL_NUM_BTN_T,
                              TEL_NUM_BTN_W,TEL_NUM_BTN_H);
    m_pTelNumEdit->setGeometry(TEL_NUM_EDIT_L,TEL_NUM_EDIT_T,
                               TEL_NUM_EDIT_W,TEL_NUM_EDIT_H);
    m_pSetTelNumBtn->setGeometry(SET_TEL_BTN_L,SET_TEL_BTN_T,
                                 SET_TEL_BTN_W,SET_TEL_BTN_H);
    QLabel* ipAddLabel = new QLabel(tr("IP 地 址 : "),m_pFrame);
    ipAddLabel->setGeometry(IP_ADD_LABEL_L,IP_ADD_LABEL_T,
                            IP_ADD_LABEL_W,IP_ADD_LABEL_H);
    m_pIpAddEdit->setGeometry(IP_ADD_EDIT_L,IP_ADD_EDIT_T,
                              IP_ADD_EDIT_W,IP_ADD_EDIT_H);

    QLabel* maskLabel = new QLabel(tr("子 网 掩 码 : "),m_pFrame);
    maskLabel->setGeometry(MASK_LABEL_L,MASK_LABEL_T,
                           MASK_LABEL_W,MASK_LABEL_H);
    m_pMaskAddEdit->setGeometry(MASK_EDIT_L,MASK_EDIT_T,
                                MASK_EDIT_W,MASK_EDIT_H);

    QLabel* getwayLabel = new QLabel(tr("默 认 网 关 : "),m_pFrame);
    getwayLabel->setGeometry(GATEWAY_LABEL_L,GATEWAY_LABEL_T,
                             GATEWAY_LABEL_W,GATEWAY_LABEL_H);
    m_pGateWayEdit->setGeometry(GATEWAY_EDIT_L,GATEWAY_EDIT_T,
                                GATEWAY_EDIT_W,GATEWAY_EDIT_H);

    QLabel* serverLabel = new QLabel(tr("云 端 IP : "),m_pFrame);
    serverLabel->setGeometry(SERVER_LABEL_L,SERVER_LABEL_T,
                             SERVER_LABEL_W,SERVER_LABEL_H);
    m_pServerAddEdit->setGeometry(SERVER_EDIT_L,SERVER_EDIT_T,
                                  SERVER_EDIT_W,SERVER_EDIT_H);
    QLabel* portLabel = new QLabel(tr("端 口 : "),m_pFrame);
    portLabel->setGeometry(PORT_LABEL_L,PORT_LABEL_T,
                           PORT_LABEL_W,PORT_LABEL_H);
    m_pPortEdit->setGeometry(PORT_EDIT_L,PORT_EDIT_T,
                             PORT_EDIT_W,PORT_EDIT_H);

    QLabel* logicLabel = new QLabel(tr("逻 辑 地 址 : "),m_pFrame);
    logicLabel->setGeometry(LOGIC_LABEL_L,LOGIC_LABEL_T,
                            LOGIC_LABEL_W,LOGIC_LABEL_H);
    m_pLogicAddLabel->setGeometry(LOGIC_L,LOGIC_T,LOGIC_W,LOGIC_H);
}

void CBaseSetW::localConnect()
{
    connect(m_pTimingBtn,SIGNAL(clicked()),this,SLOT(timing()));
    connect(m_pTelNumBtn,SIGNAL(clicked()),this,SLOT(getTelNum()));
    connect(m_pSetTelNumBtn,SIGNAL(clicked()),this,SLOT(setTelNum()));
    connect(m_pClearTotalBtn,SIGNAL(clicked()),this,SLOT(clearTotalData()));
    connect(m_pReadBtn,SIGNAL(clicked()),this,SLOT(readInfo()));
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(saveInfo()));
}
