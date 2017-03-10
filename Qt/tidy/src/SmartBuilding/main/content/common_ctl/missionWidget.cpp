#include "missionWidget.h"

#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QDebug>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/inc/xtUtility.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/main/content/common_ctl/toolTip.h"
#include "src/SmartBuilding/main/content/common_ctl/reportWidget.h"
#include "src/SmartBuilding/main/content/funWidget/timeCtrl/timeTaskInfoW.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/lightTaskInfoW.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/lightTask.h"
#include "src/SmartBuilding/main/content/funWidget/timeCtrl/timeTask.h"

#define DELAY_TIME                  500

CMissionWidget::CMissionWidget(QWidget *parent)
: QWidget(parent)
, m_sMissionType(NONE_TYPE)
, m_pTaskInfoW(NULL)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
}

CMissionWidget::~CMissionWidget()
{

}

void CMissionWidget::addMission()
{
    if (TIME_TYPE == m_sMissionType){
        if (m_pMissionReport->rowCount() >= 12){
            popToolTip(m_pAddNewTaskBtn->mapToGlobal(QPoint(0, 0)),tr("已达到最大任务量!"));
            return;
        }
    }
    else if (LIGHT_TYPE == m_sMissionType){
        if (m_pMissionReport->rowCount() >= 2){
            popToolTip(m_pAddNewTaskBtn->mapToGlobal(QPoint(0, 0)),tr("已达到最大任务量!"));
            return;
        }
    }

    if (m_pTaskInfoW)
        m_pTaskInfoW->deleteLater();

    if (TIME_TYPE == m_sMissionType)
        m_pTaskInfoW = new CTimeTaskInfoW(this);
    else if (LIGHT_TYPE == m_sMissionType)
        m_pTaskInfoW = new CLightTaskInfoW(this);

    connect(m_pTaskInfoW,SIGNAL(taskUpdate()),this,SLOT(readMission()));
    m_pTaskInfoW->show();
}

void CMissionWidget::showTask(int row,int column)
{
    if (column != 4)
        return;

    if (m_pTaskInfoW)
        m_pTaskInfoW->deleteLater();

    if (TIME_TYPE == m_sMissionType)
        m_pTaskInfoW = new CTimeTaskInfoW(this,true,row);
    else if (LIGHT_TYPE == m_sMissionType)
        m_pTaskInfoW = new CLightTaskInfoW(this,true,row);

    connect(m_pTaskInfoW,SIGNAL(taskUpdate()),this,SLOT(readMission()));
    m_pTaskInfoW->show();
}

void CMissionWidget::delOneRow()
{
    if (TIME_TYPE == m_sMissionType)
        m_pDataMgr->delTimeTask(m_pMissionReport->currentRow());
    else if (LIGHT_TYPE == m_sMissionType)
        m_pDataMgr->delLightTask(m_pMissionReport->currentRow());

    xt_msleep(DELAY_TIME);

    readMission();
}

void CMissionWidget::readMission()
{
    int row = -1;
    if (TIME_TYPE == m_sMissionType)
        row = m_pDataMgr->getTimeTask();
    else if (LIGHT_TYPE == m_sMissionType)
        row = m_pDataMgr->getLightTask();

    xt_msleep(DELAY_TIME);

    m_pMissionReport->clearReportContent();

    if (row > 0)
        addRow(row);
}

void CMissionWidget::init_()
{
    m_pMissionReport = new CReportWidget(this);
    m_pMissionReport->setColumnCount(5);
    QStringList header;
    header << tr("任务ID") << tr("执行") << tr("开始时间") << tr("结束时间") <<tr("详细信息");
    m_pMissionReport->setHeaderContent(header);
    m_pMissionReport->horizontalHeader()->setStyleSheet("background-color: #4378ac;color: white;");  // bug 不明

    m_pAddNewTaskBtn = new QPushButton(tr("新增任务"),this);
    m_pUpdateTaskBtn = new QPushButton(tr("读取任务"),this);
    m_pDelOneTaskBtn = new QPushButton(tr("删除任务"),this);

    layout_();
}

void CMissionWidget::layout_()
{
    m_pMissionReport->setGeometry(TASK_CTRL_REPORT_L,TASK_CTRL_REPORT_T,
        TASK_CTRL_REPORT_W,TASK_CTRL_REPORT_H);

    m_pAddNewTaskBtn->setGeometry(TASK_CTRL_ADD_BTN_L,TASK_CTRL_ADD_BTN_T,
        TASK_CTRL_ADD_BTN_W,TASK_CTRL_ADD_BTN_H);
    m_pUpdateTaskBtn->setGeometry(TASK_CTRL_UPDATE_BTN_L,TASK_CTRL_UPDATE_BTN_T,
        TASK_CTRL_UPDATE_BTN_W,TASK_CTRL_UPDATE_BTN_H);
    m_pDelOneTaskBtn->setGeometry(TASK_CTRL_DEL_BTN_L,TASK_CTRL_DEL_BTN_T,
        TASK_CTRL_DEL_BTN_W,TASK_CTRL_DEL_BTN_H);
}

void CMissionWidget::localConnect()
{
    connect(m_pAddNewTaskBtn,SIGNAL(clicked()),this,SLOT(addMission()));
    connect(m_pMissionReport,SIGNAL(cellClicked(int,int)),this,SLOT(showTask(int,int)));
    connect(m_pDelOneTaskBtn,SIGNAL(clicked()),this,SLOT(delOneRow()));
    connect(m_pUpdateTaskBtn,SIGNAL(clicked()),this,SLOT(readMission()));
}

void CMissionWidget::addRow(int row)
{
    for (int i = 0; i < row;i++)
    {
        m_pMissionReport->addOneRow();
        if (TIME_TYPE == m_sMissionType)
        {
            CTimeTask *timeTask = m_pDataMgr->getTimeTask(i);
            m_pMissionReport->item(i,0)->setText(QString::number(timeTask->id()));
            if (timeTask->enable())
                m_pMissionReport->item(i,1)->setText(tr("是"));
            else
                m_pMissionReport->item(i,1)->setText(tr("否"));
            m_pMissionReport->item(i,2)->setText(timeTask->startTime().mid(0,10));
            m_pMissionReport->item(i,3)->setText(timeTask->endTime().mid(0,10));
        }
        else if (LIGHT_TYPE == m_sMissionType)
        {
            CLightTask *lightTask = m_pDataMgr->getLightTask(i);
            m_pMissionReport->item(i,0)->setText(QString::number(lightTask->id()));
            if (lightTask->enable())
                m_pMissionReport->item(i,1)->setText(tr("是"));
            else
                m_pMissionReport->item(i,1)->setText(tr("否"));
            m_pMissionReport->item(i,2)->setText(lightTask->startTime().mid(0,10));
            m_pMissionReport->item(i,3)->setText(lightTask->endTime().mid(0,10));
        }
        m_pMissionReport->item(i,4)->setText(tr("详细信息"));
    }

    m_pMissionReport->setColumnColor(4,"#6ad125");
}

