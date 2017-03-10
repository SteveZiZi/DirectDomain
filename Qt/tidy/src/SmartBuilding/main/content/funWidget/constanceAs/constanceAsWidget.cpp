#include "constanceAsWidget.h"

#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QIntValidator>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/main/content/common_ctl/toolTip.h"
#include "src/SmartBuilding/main/content/common_ctl/weekSelection.h"
#include "src/SmartBuilding/main/content/funWidget/constanceAs/execTimeWidget.h"
#include "src/SmartBuilding/main/content/funWidget/constanceAs/weightWidget.h"
#include "src/SmartBuilding/main/content/funWidget/constanceAs/constanceAsTask.h"

CConstanceAsW::CConstanceAsW(QWidget *parent)
: CTaskBaseInfoW(parent)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
}

CConstanceAsW::~CConstanceAsW()
{

}

void CConstanceAsW::init_()
{
    m_pLuxValueEdit = new QLineEdit(m_pFrame);
    m_pCircuitCom = new QComboBox(m_pFrame);
    for (int i = 0;i < 16;i++)
    {
        m_pCircuitCom->addItem(QString("L#%1%2")
                    .arg(m_pDataMgr->getCPCNo())
                    .arg(i + 1, 2, 10, QChar('0')));
    }
    m_pFlushBtn = new QPushButton(tr("刷新"),this);
    m_pExecTimeW = new CExecTimeWidget(m_pFrame);
    m_pWeightWidget = new CWeightWidget(m_pFrame);
    m_pWeightWidget->updateSensor();
    m_pLuxValueEdit->setValidator(new QIntValidator(0,100000,this));

    m_pIDCombo->addItem("1");

    layout_();
}

void CConstanceAsW::layout_()
{
    m_pFrame->setGeometry(TASK_FRAME_L,TASK_FRAME_T,
                          TASK_FRAME_W,TASK_FRAME_H);
    m_pFlushBtn->setGeometry(TASK_FLUSH_BTN_L,TASK_FLUSH_BTN_T,
                             TASK_FLUSH_BTN_W,TASK_FLUSH_BTN_H);
    m_pSaveBtn->setGeometry(TASK_SAVE_BTN_L,TASK_SAVE_BTN_T,
                            TASK_SAVE_BTN_W,TASK_SAVE_BTN_H);

    QLabel* enableL = new QLabel(tr("恒照使能"),m_pFrame);
    QLabel* idLabel = new QLabel(tr("任务ID"),m_pFrame);
    QLabel* luxLabel = new QLabel(tr("恒照照度"),m_pFrame);
    QLabel* startL = new QLabel(tr("开始时间"),m_pFrame);
    QLabel* endL = new QLabel(tr("结束时间"),m_pFrame);
    QLabel* execTimeL = new QLabel(tr("执行时段"),m_pFrame);
    QLabel* circuitL = new QLabel(tr("回路选择"),m_pFrame);
    QLabel* loopWayL = new QLabel(tr("循环方式"),m_pFrame);
    QLabel* weightL = new QLabel(tr("权重设置"),m_pFrame);
    enableL->setGeometry(ENABLE_LABEL_L,ENABLE_LABEL_T,
                         ENABLE_LABEL_W,ENABLE_LABEL_H);
    m_pEnableCheck->setGeometry(ENABLE_CHECK_L,ENABLE_CHECK_T,
                                ENABLE_CHECK_W,ENABLE_CHECK_H);
    idLabel->setGeometry(ID_LABEL_L,ID_LABEL_T,ID_LABEL_W,ID_LABEL_H);
    m_pIDCombo->setGeometry(ID_COMBOX_L,ID_COMBOX_T,ID_COMBOX_W,ID_COMBOX_H);
    luxLabel->setGeometry(CON_LUX_LABEL_L,CON_LUX_LABEL_T,
                          CON_LUX_LABEL_W,CON_LUX_LABEL_H);
    m_pLuxValueEdit->setGeometry(CON_LUX_EDIT_L,CON_LUX_EDIT_T,
                                 CON_LUX_EDIT_W,CON_LUX_EDIT_H);
    startL->setGeometry(CON_STARTL_L,CON_STARTL_T,CON_STARTL_W,CON_STARTL_H);
    m_pStartTime->setGeometry(CON_STARTE_L,CON_STARTE_T,
                              CON_STARTE_W,CON_STARTE_H);
    endL->setGeometry(CON_END_LABEL_L,CON_END_LABEL_T,
                      CON_END_LABEL_W,CON_END_LABEL_H);
    m_pEndTime->setGeometry(CON_END_EDIT_L,CON_END_EDIT_T,
                            CON_END_EDIT_W,CON_END_EDIT_H);
    execTimeL->setGeometry(CON_EXECTIMEL_L,CON_EXECTIMEL_T,
                           CON_EXECTIMEL_W,CON_EXECTIMEL_H);
    m_pExecTimeW->setGeometry(CON_EXECTIMEE_L,CON_EXECTIMEE_T,
                              CON_EXECTIMEE_W,CON_EXECTIMEE_H);
    circuitL->setGeometry(CON_CIRCUITL_L,CON_CIRCUITL_T,CON_CIRCUITL_W,CON_CIRCUITL_H);
    m_pCircuitCom->setGeometry(CON_CIRCUITC_L,CON_CIRCUITC_T,
                               CON_CIRCUITC_W,CON_CIRCUITC_H);
    loopWayL->setGeometry(CON_LOOP_LABEL_L,CON_LOOP_LABEL_T,
                          CON_LOOP_LABEL_W,CON_LOOP_LABEL_H);
    m_pWeekSelection->setGeometry(CON_LOOP_WAY_L,CON_LOOP_WAY_T,
                                  CON_LOOP_WAY_W,CON_LOOP_WAY_H);
    weightL->setGeometry(CON_WEIGHTL_L,CON_WEIGHTL_T,
                         CON_WEIGHTL_W,CON_WEIGHTL_H);
    m_pWeightWidget->setGeometry(CON_WEIGHT_L,CON_WEIGHT_T,
                                 CON_WEIGHT_W,CON_WEIGHT_H);
}

void CConstanceAsW::localConnect()
{
    connect(m_pFlushBtn,SIGNAL(clicked()),this,SLOT(flushTask()));
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(saveTask()));
    connect(m_pCircuitCom,SIGNAL(currentIndexChanged(int)),this,SLOT(checkCircuit(int)));
}

void CConstanceAsW::flushTask()
{
    CConstanceAsTask constanceAsTask;

    if (m_pDataMgr->getConstanceAsTask(constanceAsTask)){
        if (constanceAsTask.enable())
            m_pEnableCheck->setChecked(true);
        else
            m_pEnableCheck->setChecked(false);
        m_pIDCombo->setCurrentIndex(constanceAsTask.id() - 1);
        m_pLuxValueEdit->setText(QString::number(constanceAsTask.constanceLux()));
        m_pStartTime->setDateTime(QDateTime::fromString(constanceAsTask.startTime(),"yyyy-MM-dd hh:mm:ss"));
        m_pEndTime->setDateTime(QDateTime::fromString(constanceAsTask.endTime(),"yyyy-MM-dd hh:mm:ss"));
        m_pWeekSelection->setWeekSelection(constanceAsTask.weekSelection());
        m_pWeightWidget->updateSensor();
        m_pWeightWidget->setSensor(constanceAsTask.sensorSelection());
        INT8U weight[16] = {0};
        constanceAsTask.getWeight(weight,16);
        m_pWeightWidget->setWeight(weight,16);
        m_pExecTimeW->setStartHour(constanceAsTask.beginHour());
        m_pExecTimeW->setStartMin(constanceAsTask.beginMins());
        m_pExecTimeW->setEndHour(constanceAsTask.endHour());
        m_pExecTimeW->setEndMin(constanceAsTask.endMins());
    }
}

void CConstanceAsW::saveTask()
{
    if (m_pLuxValueEdit->text().isEmpty()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("恒照光强度不能为0!"));
        return;
    }
    else if(0 == m_pWeightWidget->getSensor()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("光照传感器不能为空!"));
        return;
    }
    else if(0 == m_pWeekSelection->getWeekSelection()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("循环方式不能为空!"));
        return;
    }
    else if(m_pEndTime->dateTime().toTime_t() <= m_pStartTime->dateTime().toTime_t()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("执行时段开始时间不能超过结束时间!"));
        return;
    }
    else if(m_pExecTimeW->startHour() < m_pExecTimeW->endHour()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("执行时段开始时间不能超过结束时间!"));
        return;
    }
    else if(m_pExecTimeW->startHour() == m_pExecTimeW->endHour() && 
        m_pExecTimeW->startMin() == m_pExecTimeW->endMin()){
            popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("执行时段开始时间不能超过结束时间!"));
            return;
    }

    CConstanceAsTask constanceAsTask;
    if (m_pEnableCheck->isChecked()){
        constanceAsTask.setEnable(1);
        if (checkCircuit(m_pCircuitCom->currentIndex()))
            return;
        m_pDataMgr->setTasktRecord(CONSTANCE_AS_TASK_TYPE,m_pCircuitCom->currentIndex() + 1);
    }
    else{
        constanceAsTask.setEnable(0);
        m_pDataMgr->clearTaskRecord(m_pCircuitCom->currentIndex() + 1);
    }
    constanceAsTask.setId(m_pIDCombo->currentIndex() + 1);
    constanceAsTask.setStartTime(m_pStartTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    constanceAsTask.setEndTime(m_pEndTime->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    constanceAsTask.setWeekSelection(m_pWeekSelection->getWeekSelection());

    constanceAsTask.m_iConstanceLux = m_pLuxValueEdit->text().toUInt();
    constanceAsTask.m_cCircuitSelection = m_pCircuitCom->currentIndex() + 1;
    constanceAsTask.m_cBeginHour = m_pExecTimeW->startHour();
    constanceAsTask.m_cBeginMins = m_pExecTimeW->startMin();
    constanceAsTask.m_cEndHour = m_pExecTimeW->endHour();
    constanceAsTask.m_cEndMins = m_pExecTimeW->endMin();
    constanceAsTask.m_iSensorSelection = m_pWeightWidget->getSensor();
    m_pWeightWidget->getWeight(constanceAsTask.m_cWeight,16);

    m_pDataMgr->saveConstanceAsTask(constanceAsTask);

    popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("保存成功!"));
}

bool CConstanceAsW::checkCircuit(int index)
{
    bool ret = false;
    if (m_pDataMgr->getCircuitEnable(index + 1))
    {
        int belong = m_pDataMgr->getCircuitBelong(index + 1);
        if (TIME_TASK_TYPE == belong){
            ret = true;
            popToolTip(m_pCircuitCom->mapToGlobal(QPoint(0, 0)),tr("该回路已用于时控任务!"));
        }
        else if (LIGHT_TASK_TYPE == belong){
            ret = true;
            popToolTip(m_pCircuitCom->mapToGlobal(QPoint(0, 0)),tr("该回路已用于光控任务!"));
        }
    }

    return ret;
}
