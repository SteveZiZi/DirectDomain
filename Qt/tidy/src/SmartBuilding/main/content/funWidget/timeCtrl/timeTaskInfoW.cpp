#include "timeTaskInfoW.h"

#include <QLabel>
#include <QDebug>
#include <QDateTime>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QListWidget>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/main/content/common_ctl/weekSelection.h"
#include "src/SmartBuilding/main/content/common_ctl/toolTip.h"
#include "src/SmartBuilding/main/content/funWidget/timeCtrl/circuitSelection.h"
#include "src/SmartBuilding/main/content/funWidget/timeCtrl/timeTaskOrder.h"

#define MAX_TASK_COUNT              12

CTimeTaskInfoW::CTimeTaskInfoW(QWidget *parent,bool exit,int index)
: CTaskBaseInfoW(parent,exit)
, m_iIndex(index)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
    updateTaskInfo();
}

CTimeTaskInfoW::~CTimeTaskInfoW()
{

}

void CTimeTaskInfoW::saveTaskInfo()
{
//     if (!isExit())
//         emit taskUpdate();

    if (saveTask()){
        emit taskUpdate();
        this->close();
    }
}

void CTimeTaskInfoW::addAction()
{
    if (m_pActionWidget->count() >= 12){
        popToolTip(m_pAddActionBtn->mapToGlobal(QPoint(0, 0)),tr("已达到最大指令数!"));
        return;
    }

    QListWidgetItem* item = new QListWidgetItem;
    CBaseOrderWidget* order = new CTimeTaskOrder;
    m_pActionWidget->addItem(item);
    m_pActionWidget->setItemWidget(item,order);
    item->setSizeHint(QSize(ACTION_REPORT_W - 30,REPORT_ROW_HEIGHT + 10)); // 设置每行的行高
    connect(order,SIGNAL(orderDeled()),this,SLOT(delAction()));
}

void CTimeTaskInfoW::delAction()
{
    QPoint point = m_pActionWidget->mapFromGlobal(QCursor::pos());      // 将全局坐标映射到列表中
    int row = m_pActionWidget->row(m_pActionWidget->itemAt(point));
    CTimeTaskOrder * order = dynamic_cast<CTimeTaskOrder *>(m_pActionWidget
                            ->itemWidget(m_pActionWidget->item(row)));  // item转换为Order
    disconnect(order,SIGNAL(orderDeled()),this,SLOT(delAction()));
    delete m_pActionWidget->takeItem(row);
}

void CTimeTaskInfoW::init_()
{
    for (int i = 0;i < MAX_TASK_COUNT;i++){
        m_pIDCombo->addItem(QString::number(i + 1));
    }

    m_pPriorityCom = new QComboBox(m_pFrame);
    QStringList priority;
    priority << tr("低") << tr("中") << tr("高");
    m_pPriorityCom->addItems(priority);
    m_pCircuitSelection = new CCircuitSelection(m_pFrame);
    m_pActionWidget = new QListWidget(m_pFrame);
    m_pActionWidget->setAlternatingRowColors(true);

    m_pCancelBtn = new QPushButton(tr("取消"),this);
    m_pAddActionBtn = new QPushButton(tr("添加动作"),this);

    layout_();

    checkCircuit();                                                     // 更新回路是否已经选中
}

void CTimeTaskInfoW::layout_()
{
    m_pFrame->setGeometry(TASK_FRAME_L,TASK_FRAME_T,
        TASK_FRAME_W,TASK_FRAME_H);
    m_pAddActionBtn->setGeometry(TASK_ADDACTION_BTN_L,TASK_ADDACTION_BTN_T,
        TASK_ADDACTION_BTN_W,TASK_ADDACTION_BTN_H);
    m_pCancelBtn->setGeometry(TASK_CANCEL_BTN_L,TASK_CANCEL_BTN_T,
        TASK_CANCEL_BTN_W,TASK_CANCEL_BTN_H);
    m_pSaveBtn->setGeometry(TASK_SAVE_BTN_L,TASK_SAVE_BTN_T,
        TASK_SAVE_BTN_W,TASK_SAVE_BTN_H);

    QLabel* enableL = new QLabel(tr("时控使能"),m_pFrame);
    QLabel* idLabel = new QLabel(tr("任务ID"),m_pFrame);
    QLabel* priorityL = new QLabel(tr("优先级"),m_pFrame);
    QLabel* startL = new QLabel(tr("开始时间"),m_pFrame);
    QLabel* endL = new QLabel(tr("结束时间"),m_pFrame);
    QLabel* loopWayL = new QLabel(tr("循环方式"),m_pFrame);

    enableL->setGeometry(ENABLE_LABEL_L,ENABLE_LABEL_T,
                         ENABLE_LABEL_W,ENABLE_LABEL_H);
    m_pEnableCheck->setGeometry(ENABLE_CHECK_L,ENABLE_CHECK_T,
                                ENABLE_CHECK_W,ENABLE_CHECK_H);
    idLabel->setGeometry(TIME_ID_LABEL_L,TIME_ID_LABEL_T,
                         TIME_ID_LABEL_W,TIME_ID_LABEL_H);
    m_pIDCombo->setGeometry(TIME_ID_COM_L,TIME_ID_COM_T,
                            TIME_ID_COM_W,TIME_ID_COM_H);
    priorityL->setGeometry(TIME_PRIORITYL_L,TIME_PRIORITYL_T,
                           TIME_PRIORITYL_W,TIME_PRIORITYL_H);
    m_pPriorityCom->setGeometry(TIME_PRIORITY_L,TIME_PRIORITY_T,
                                TIME_PRIORITY_W,TIME_PRIORITY_H);
    startL->setGeometry(TIME_START_TIMEL_L,TIME_START_TIMEL_T,
                        TIME_START_TIMEL_W,TIME_START_TIMEL_H);
    m_pStartTime->setGeometry(TIME_START_TIME_L,TIME_START_TIME_T,
                              TIME_START_TIME_W,TIME_START_TIME_H);
    endL->setGeometry(TIME_END_TIMEL_L,TIME_END_TIMEL_T,
                      TIME_END_TIMEL_W,TIME_END_TIMEL_H);
    m_pEndTime->setGeometry(TIME_END_TIME_L,TIME_END_TIME_T,
                            TIME_END_TIME_W,TIME_END_TIME_H);
    loopWayL->setGeometry(TIME_LOOP_LABEL_L,TIME_LOOP_LABEL_T,
                          TIME_LOOP_LABEL_W,TIME_LOOP_LABEL_H);
    m_pWeekSelection->setGeometry(TIME_LOOP_WAY_L,TIME_LOOP_WAY_T,
                                  TIME_LOOP_WAY_W,TIME_LOOP_WAY_H);
    m_pActionWidget->setGeometry(ACTION_REPORT_L,ACTION_REPORT_T,
                                 ACTION_REPORT_W,ACTION_REPORT_H);
    m_pCircuitSelection->setGeometry(SENSOR_SELECTION_L,SENSOR_SELECTION_T,
                                     SENSOR_SELECTION_W,SENSOR_SELECTION_H);
}

void CTimeTaskInfoW::localConnect()
{
    connect(m_pCancelBtn,SIGNAL(clicked()),this,SLOT(close()));
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(saveTaskInfo()));
    connect(m_pAddActionBtn,SIGNAL(clicked()),this,SLOT(addAction()));
}

void CTimeTaskInfoW::checkCircuit()
{
    for (int i = 0; i < 16;i++)
    {
        if (m_pDataMgr->getCircuitEnable(i + 1))
            if (TIME_TASK_TYPE != m_pDataMgr->getCircuitBelong(i + 1))
                m_pCircuitSelection->setCircuitEnable(i,false);
    }
}

void CTimeTaskInfoW::updateTaskInfo()
{
    if (!isExit())
        return;

    m_pIDCombo->setEnabled(false);
    CTimeTask *timeTask = m_pDataMgr->getTimeTask(m_iIndex);
    if (timeTask->enable())
        m_pEnableCheck->setChecked(true);
    else
        m_pEnableCheck->setChecked(false);
    m_pIDCombo->setCurrentIndex(timeTask->id() - 1);
    m_pStartTime->setDateTime(QDateTime::fromString(timeTask->startTime(),"yyyy-MM-dd hh:mm:ss"));
    m_pEndTime->setDateTime(QDateTime::fromString(timeTask->endTime(),"yyyy-MM-dd hh:mm:ss"));
    m_pWeekSelection->setWeekSelection(timeTask->weekSelection());
    m_pCircuitSelection->setCircuitSelection(timeTask->circuitSelection());
    m_pPriorityCom->setCurrentIndex(timeTask->priority() - 1);
    for (int orderSize = 0;orderSize < timeTask->orderCount();orderSize++)
    {
        addAction(timeTask->timeOrder()[orderSize]);
    }
}

void CTimeTaskInfoW::addAction(CTimeOrder& timeOrder)
{
    QListWidgetItem* item = new QListWidgetItem;
    CTimeTaskOrder* order = new CTimeTaskOrder;
    order->setOrder(timeOrder.order() - 1);
    order->setHour(timeOrder.hour());
    order->setMinute(timeOrder.minute());

    m_pActionWidget->addItem(item);
    m_pActionWidget->setItemWidget(item,order);
    item->setSizeHint(QSize(ACTION_REPORT_W - 30,REPORT_ROW_HEIGHT + 10)); // 设置每行的行高
    connect(order,SIGNAL(orderDeled()),this,SLOT(delAction()));
}

bool CTimeTaskInfoW::saveTask()
{
    if(m_pActionWidget->count() == 0){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("指令不能为空!"));
        return false;
    }
    else if(0 == m_pWeekSelection->getWeekSelection()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("循环方式不能为空!"));
        return false;
    }
    else if(m_pEndTime->dateTime().toTime_t() <= m_pStartTime->dateTime().toTime_t())
    {
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("结束时间不能小于或等于开始时间!"));
        return false;
    }

    CTimeTask timeTask;
    if (m_pEnableCheck->isChecked())
        timeTask.setEnable(1);
    else
        timeTask.setEnable(0);
    timeTask.setId(m_pIDCombo->currentIndex() + 1);
    timeTask.setStartTime(m_pStartTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    timeTask.setEndTime(m_pEndTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    timeTask.setWeekSelection(m_pWeekSelection->getWeekSelection());

    timeTask.setOrderCount(m_pActionWidget->count());
    timeTask.setPriority(m_pPriorityCom->currentIndex() + 1);
    timeTask.setCircuitSelection(m_pCircuitSelection->getCircuitSelection());
    timeTask.setCircuitCount(m_pCircuitSelection->getCircuitCount());
    m_pCircuitSelection->getCircuitList(timeTask.getCircuitList());
    // 设置回路选中记录
    for (int i = 0; i < timeTask.getCircuitList().size();i++)
    {
        m_pDataMgr->setTasktRecord(TIME_TASK_TYPE,timeTask.getCircuitList()[i]);
    }

    for (int orderSize = 0;orderSize < m_pActionWidget->count();orderSize++)
    {
        CTimeTaskOrder * timeTaskOrder = qobject_cast<CTimeTaskOrder *>(m_pActionWidget->itemWidget(m_pActionWidget->item(orderSize)));  // item转换为Order
        timeTask.timeOrder()[orderSize].setHour(timeTaskOrder->hour());
        timeTask.timeOrder()[orderSize].setMinute(timeTaskOrder->minute());
        timeTask.timeOrder()[orderSize].setOrder(timeTaskOrder->orderIndex() + 1);
    }

    // 采用冒泡算法对指令按照时间先后顺序排序
    CTimeOrder timeOrder;
    for(int i = 0;i < m_pActionWidget->count() - 1;i++)
    {
        for(int j = 0;j < m_pActionWidget->count() - 1 - i;j++)
        {
            if(timeTask.timeOrder()[j].hour() > timeTask.timeOrder()[j + 1].hour() || 
                (timeTask.timeOrder()[j].hour() == timeTask.timeOrder()[j + 1].hour() && 
                timeTask.timeOrder()[j].minute() >= timeTask.timeOrder()[j + 1].minute()))
            {
                timeOrder.setHour(timeTask.timeOrder()[j].hour());
                timeOrder.setMinute(timeTask.timeOrder()[j].minute());
                timeOrder.setOrder(timeTask.timeOrder()[j].order());

                timeTask.timeOrder()[j].setHour(timeTask.timeOrder()[j + 1].hour());
                timeTask.timeOrder()[j].setMinute(timeTask.timeOrder()[j + 1].minute());
                timeTask.timeOrder()[j].setOrder(timeTask.timeOrder()[j + 1].order());

                timeTask.timeOrder()[j + 1].setHour(timeOrder.hour());
                timeTask.timeOrder()[j + 1].setMinute(timeOrder.minute());
                timeTask.timeOrder()[j + 1].setOrder(timeOrder.order());
            }
        }

    }

    m_pDataMgr->saveTimeTask(timeTask);

    return true;
}
