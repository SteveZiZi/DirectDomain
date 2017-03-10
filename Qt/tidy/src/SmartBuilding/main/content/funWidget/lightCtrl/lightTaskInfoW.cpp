#include "lightTaskInfoW.h"

#include <QDebug>
#include <QLabel>
#include <QDateTime>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QIntValidator>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/main/content/common_ctl/weekSelection.h"
#include "src/SmartBuilding/main/content/common_ctl/toolTip.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/sensorSelectionW.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/lightTaskOrder.h"

CLightTaskInfoW::CLightTaskInfoW(QWidget *parent,bool exit,int index)
: CTaskBaseInfoW(parent,exit)
, m_iIndex(index)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
    updateTaskInfo();
}

CLightTaskInfoW::~CLightTaskInfoW()
{

}

void CLightTaskInfoW::saveTaskInfo()
{
//     if (!isExit())
//         emit taskUpdate();

    if (saveTask()){
        emit taskUpdate();
        this->close();
    }
}

void CLightTaskInfoW::addAction()
{
    if (m_pActionWidget->count() >= 4){
        popToolTip(m_pAddActionBtn->mapToGlobal(QPoint(0, 0)),tr("已达到最大指令数!"));
        return;
    }

    QListWidgetItem* item = new QListWidgetItem;
    CBaseOrderWidget* order = new CLightTaskOrder;
    m_pActionWidget->addItem(item);
    m_pActionWidget->setItemWidget(item,order);
    item->setSizeHint(QSize(ACTION_REPORT_W - 30,REPORT_ROW_HEIGHT + 10)); // 设置每行的行高
    connect(order,SIGNAL(orderDeled()),this,SLOT(delAction()));
}

void CLightTaskInfoW::delAction()
{
    QPoint point = m_pActionWidget->mapFromGlobal(QCursor::pos());      // 将全局坐标映射到列表中
    int row = m_pActionWidget->row(m_pActionWidget->itemAt(point));
    CLightTaskOrder * order = dynamic_cast<CLightTaskOrder *>(m_pActionWidget
                            ->itemWidget(m_pActionWidget->item(row)));  // item转换为Order
    disconnect(order,SIGNAL(orderDeled()),this,SLOT(delAction()));
    delete m_pActionWidget->takeItem(row);
}

bool CLightTaskInfoW::checkCircuit(int index)
{
    bool ret = false;
    if (m_pDataMgr->getCircuitEnable(index + 1))
    {
        int belong = m_pDataMgr->getCircuitBelong(index + 1);
        qDebug() << "belong" << belong;
        if (TIME_TASK_TYPE == belong){
            ret = true;
            popToolTip(m_pCircuitCom->mapToGlobal(QPoint(0, 0)),tr("该回路已用于时控任务!"));
        }
        else if (CONSTANCE_AS_TASK_TYPE == belong){
            ret = true;
            popToolTip(m_pCircuitCom->mapToGlobal(QPoint(0, 0)),tr("该回路已用于恒照任务!"));
        }
    }

    return ret;
}

void CLightTaskInfoW::init_()
{
    m_pCircuitCom = new QComboBox(m_pFrame);
    for (int i = 0;i < 16;i++)
    {
        m_pCircuitCom->addItem(QString("L#%1%2")
                    .arg(m_pDataMgr->getCPCNo())
                    .arg(i + 1, 2, 10, QChar('0')));
    }

    m_pCancelBtn = new QPushButton(tr("取消"),this);
    m_pAddActionBtn = new QPushButton(tr("添加动作"),this);
    m_pStartLuxEdit = new QLineEdit(m_pFrame);
    m_pEndLuxEdit = new QLineEdit(m_pFrame);
    m_pActionWidget = new QListWidget(m_pFrame);
    m_pActionWidget->setAlternatingRowColors(true);
    m_pSensorSelection = new CSensorSelectionW(m_pFrame);
    m_pSensorSelection->updateSensor();
    QIntValidator* luxExp = new QIntValidator(0,100000,this);
    m_pStartLuxEdit->setValidator(luxExp);
    m_pEndLuxEdit->setValidator(luxExp);

    QStringList idList;
    idList << "1" << "2";
    m_pIDCombo->addItems(idList);
    layout_();
}

void CLightTaskInfoW::layout_()
{
    m_pFrame->setGeometry(TASK_FRAME_L,TASK_FRAME_T,
                          TASK_FRAME_W,TASK_FRAME_H);
    m_pAddActionBtn->setGeometry(TASK_ADDACTION_BTN_L,TASK_ADDACTION_BTN_T,
                                 TASK_ADDACTION_BTN_W,TASK_ADDACTION_BTN_H);
    m_pCancelBtn->setGeometry(TASK_CANCEL_BTN_L,TASK_CANCEL_BTN_T,
                              TASK_CANCEL_BTN_W,TASK_CANCEL_BTN_H);
    m_pSaveBtn->setGeometry(TASK_SAVE_BTN_L,TASK_SAVE_BTN_T,
                            TASK_SAVE_BTN_W,TASK_SAVE_BTN_H);

    QLabel* enableL = new QLabel(tr("光控使能"),m_pFrame);
    QLabel* idLabel = new QLabel(tr("任务ID"),m_pFrame);
    QLabel* startLuxLabel = new QLabel(tr("启动阀值"),m_pFrame);
    QLabel* endLuxLabel = new QLabel(tr("结束阀值"),m_pFrame);
    QLabel* startL = new QLabel(tr("开始时间"),m_pFrame);
    QLabel* endL = new QLabel(tr("结束时间"),m_pFrame);
    QLabel* circuitL = new QLabel(tr("回路选择"),m_pFrame);
    QLabel* loopWayL = new QLabel(tr("循环方式"),m_pFrame);
    enableL->setGeometry(ENABLE_LABEL_L,ENABLE_LABEL_T,
                         ENABLE_LABEL_W,ENABLE_LABEL_H);
    m_pEnableCheck->setGeometry(ENABLE_CHECK_L,ENABLE_CHECK_T,
                                ENABLE_CHECK_W,ENABLE_CHECK_H);
    idLabel->setGeometry(ID_LABEL_L,ID_LABEL_T,ID_LABEL_W,ID_LABEL_H);
    m_pIDCombo->setGeometry(ID_COMBOX_L,ID_COMBOX_T,ID_COMBOX_W,ID_COMBOX_H);
    startLuxLabel->setGeometry(START_LUX_LABEL_L,START_LUX_LABEL_T,
                               START_LUX_LABEL_W,START_LUX_LABEL_H);
    m_pStartLuxEdit->setGeometry(START_LUX_EDIT_L,START_LUX_EDIT_T,
                                 START_LUX_EDIT_W,START_LUX_EDIT_H);
    endLuxLabel->setGeometry(END_LUX_LABEL_L,END_LUX_LABEL_T,
                             END_LUX_LABEL_W,END_LUX_LABEL_H);
    m_pEndLuxEdit->setGeometry(END_LUX_EDIT_L,END_LUX_EDIT_T,
                               END_LUX_EDIT_W,END_LUX_EDIT_H);
    startL->setGeometry(LIGHT_START_TIMEL_L,LIGHT_START_TIMEL_T,
                        LIGHT_START_TIMEL_W,LIGHT_START_TIMEL_H);
    m_pStartTime->setGeometry(LIGHT_START_TIME_L,LIGHT_START_TIME_T,
                              LIGHT_START_TIME_W,LIGHT_START_TIME_H);
    endL->setGeometry(LIGHT_END_TIMEL_L,LIGHT_END_TIMEL_T,
                      LIGHT_END_TIMEL_W,LIGHT_END_TIMEL_H);
    m_pEndTime->setGeometry(LIGHT_END_TIME_L,LIGHT_END_TIME_T,
                            LIGHT_END_TIME_W,LIGHT_END_TIME_H);
    circuitL->setGeometry(LIGHT_CIRCUITL_L,LIGHT_CIRCUITL_T,
                          LIGHT_CIRCUITL_W,LIGHT_CIRCUITL_H);
    m_pCircuitCom->setGeometry(LIGHT_CIRCUIT_L,LIGHT_CIRCUIT_T,
                               LIGHT_CIRCUIT_W,LIGHT_CIRCUIT_H);
    loopWayL->setGeometry(LIGHT_LOOP_LABEL_L,LIGHT_LOOP_LABEL_T,
                          LIGHT_LOOP_LABEL_W,LIGHT_LOOP_LABEL_H);
    m_pWeekSelection->setGeometry(LIGHT_LOOP_WAY_L,LIGHT_LOOP_WAY_T,
                                  LIGHT_LOOP_WAY_W,LIGHT_LOOP_WAY_H);
    m_pActionWidget->setGeometry(ACTION_REPORT_L,ACTION_REPORT_T,
                                 ACTION_REPORT_W,ACTION_REPORT_H);
    m_pSensorSelection->setGeometry(SENSOR_SELECTION_L,SENSOR_SELECTION_T,
                                    SENSOR_SELECTION_W,SENSOR_SELECTION_H);
}

void CLightTaskInfoW::localConnect()
{
    connect(m_pCancelBtn,SIGNAL(clicked()),this,SLOT(close()));
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(saveTaskInfo()));
    connect(m_pAddActionBtn,SIGNAL(clicked()),this,SLOT(addAction()));
    connect(m_pCircuitCom,SIGNAL(currentIndexChanged(int)),this,SLOT(checkCircuit(int)));
}

void CLightTaskInfoW::updateTaskInfo()
{
    if (!isExit())
        return;

    m_pIDCombo->setEnabled(false);
    CLightTask *lightTask = m_pDataMgr->getLightTask(m_iIndex);
    if (lightTask->enable())
        m_pEnableCheck->setChecked(true);
    else
        m_pEnableCheck->setChecked(false);
    m_pIDCombo->setCurrentIndex(lightTask->id() - 1);
    m_pStartTime->setDateTime(QDateTime::fromString(lightTask->startTime(),"yyyy-MM-dd hh:mm:ss"));
    m_pEndTime->setDateTime(QDateTime::fromString(lightTask->endTime(),"yyyy-MM-dd hh:mm:ss"));
    m_pWeekSelection->setWeekSelection(lightTask->weekSelection());
    m_pSensorSelection->setSensorSelection(lightTask->sensorSelection());
    m_pStartLuxEdit->setText(QString::number(lightTask->startLux()));
    m_pEndLuxEdit->setText(QString::number(lightTask->endLux()));
    m_pCircuitCom->setCurrentIndex(lightTask->circuitSelection() - 1);
    for (int orderSize = 0;orderSize < lightTask->orderCount();orderSize++)
    {
        addAction(lightTask->lightOrder()[orderSize]);
    }
}

void CLightTaskInfoW::addAction(CLightOrder& lightOrder)
{
    QListWidgetItem* item = new QListWidgetItem;
    CLightTaskOrder* order = new CLightTaskOrder;
    order->setOrder(lightOrder.order() - 1);
    order->setDurationTime(lightOrder.minutes());

    m_pActionWidget->addItem(item);
    m_pActionWidget->setItemWidget(item,order);
    item->setSizeHint(QSize(ACTION_REPORT_W - 30,REPORT_ROW_HEIGHT + 10)); // 设置每行的行高
    connect(order,SIGNAL(orderDeled()),this,SLOT(delAction()));
}

bool CLightTaskInfoW::saveTask()
{
    if(m_pActionWidget->count() == 0){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("指令不能为空!"));
        return false;
    }
    else if(checkCircuit(m_pCircuitCom->currentIndex())){
        return false;
    }
    else if(m_pStartLuxEdit->text().isEmpty()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("启动阀值不能为空!"));
        return false;
    }
    else if(m_pEndLuxEdit->text().isEmpty()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("结束阀值不能为空!"));
        return false;
    }
    else if(m_pStartLuxEdit->text().toInt() >= m_pEndLuxEdit->text().toInt()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("启动阀值不能大于结束阀值!"));
        return false;
    }
    else if(0 == m_pSensorSelection->getSensorSelection()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("光照传感器不能为空!"));
        return false;
    }
    else if(0 == m_pWeekSelection->getWeekSelection()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("循环方式不能为空!"));
        return false;
    }
    else if(m_pEndTime->dateTime().toTime_t() <= m_pStartTime->dateTime().toTime_t()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("结束时间不能小于或等于开始时间!"));
        return false;
    }

    CLightTask lightTask;
    if (m_pEnableCheck->isChecked())
        lightTask.setEnable(1);
    else
        lightTask.setEnable(0);
    lightTask.setId(m_pIDCombo->currentIndex() + 1);
    lightTask.setStartTime(m_pStartTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    lightTask.setEndTime(m_pEndTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    lightTask.setWeekSelection(m_pWeekSelection->getWeekSelection());

    lightTask.setOrderCount(m_pActionWidget->count());
    lightTask.setCircuitSelection(m_pCircuitCom->currentIndex() + 1);
    lightTask.setStartLux(m_pStartLuxEdit->text().toUInt());
    lightTask.setEndLux(m_pEndLuxEdit->text().toUInt());
    lightTask.setSensorSelection(m_pSensorSelection->getSensorSelection());
    for (int orderSize = 0;orderSize < m_pActionWidget->count();orderSize++)
    {
        CLightTaskOrder * lightTaskOrder = qobject_cast<CLightTaskOrder *>(m_pActionWidget->itemWidget(m_pActionWidget->item(orderSize)));  // item转换为Order
        lightTask.lightOrder()[orderSize].setMinutes(lightTaskOrder->durationTime());
        lightTask.lightOrder()[orderSize].setOrder(lightTaskOrder->orderIndex() + 1);
    }

    m_pDataMgr->saveLightTask(lightTask);
    m_pDataMgr->setTasktRecord(LIGHT_TASK_TYPE,m_pCircuitCom->currentIndex() + 1);

    return true;
}
