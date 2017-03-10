#include "dataMgr.h"

#include <QFile>
#include <QList>
#include <QDebug>
#include <QSqlQuery>
#include <QTextStream>

#include "src/SmartBuilding/dataLevel/dataComm/include/typedef.h"
#include "src/SmartBuilding/dataLevel/dataComm/sharemem/ipcStatus.h"
#include "src/SmartBuilding/dataLevel/dataComm/sharemem/ipcSharemem.h"

#include "src/SmartBuilding/dataLevel/database.h"
#include "src/SmartBuilding/dataLevel/cabinet.h"
#include "src/SmartBuilding/dataLevel/circuit.h"
#include "src/SmartBuilding/dataLevel/rectifier.h"

#define TOTAL_DATA_FILE                 "./dat/total.dat"
#define WARN_CONFIG_FILE                "./dat/warningconfig.dat"
#define QT_TIME_FILE                    "./dat/QtTime.dat"

#define MSZ                             1
typedef enum
{
    MD_RCU=0,
    MD_AIRCONDITION=1,
    MD_RELAY_8=2,
    MD_LIGHT_4=3,
    MD_RELAY_2=4,
    MD_LEDV12_3=5,
    MD_DOORDISPLAY=6,
    MD_RFID_CARD=7,
    MD_KEYBOARD_20=8,
    MD_RTF =9,
    MD_LUX_SENSOR = 10,
    MD_TYPE_MAX
}T_emDeviceType;

CDataMgr* CDataMgr::getInstance()
{
    static CDataMgr dataMgr;
    return &dataMgr;
}

CCabinet* CDataMgr::getIpcData()
{
#if MSZ
    m_pCabinet->clearCircuit();

    m_iCircuitCount = IPCSHAREMEM.m_ClNum;
    for (int circuitNo = 0;circuitNo < m_iCircuitCount;circuitNo++)
    {
        CCircuit* circuit = new CCircuit;
        circuit->setCircuitNo(circuitNo + 1);
        m_pCabinet->appendCircuit(circuit);
    }

    for (int deviceNo = 1;deviceNo < CONS_DEVICE_MAX_COUNT;deviceNo++)
    {
        if(IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_DeviceNo){
            if(IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_DeviceType == MD_RTF){
                CRectifier* rectifier = new CRectifier;
                rectifier->setDeviceNo(deviceNo);
                QString value;
                // 电压
                value = QString::number((INT16U)(IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_Reg0501[0] << 8 | IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_Reg0501[1]));
                double volgate = value.toDouble() / 10;
                rectifier->setVolgate(volgate);
                // 电流
                value = QString::number((INT16U)(IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_Reg0501[2] << 8 | IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_Reg0501[3]));
                double current = value.toDouble() / 10;
                rectifier->setCurrent(current);
                // 温度
                value = QString::number(IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_Reg0501[4]);
                double temp = value.toDouble();
                // 状态
                INT16U state = (INT16U)(IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_Reg0501[7] | IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_Reg0501[8] << 8);
                rectifier->setState(state);
                CCircuit* circuit = m_pCabinet->itemOfCircuit(IPCSHAREMEM.m_IpcDevicesRegData[deviceNo].m_ClNo);
                if (circuit)
                    circuit->appendRectifier(rectifier);
            }
        }
    }

    for (int circuitNo = 0;circuitNo < m_iCircuitCount;circuitNo++)
    {
        CCircuit* circuit = m_pCabinet->itemOfCircuit(circuitNo);
        if (circuit)
            circuit->updateParam();
    }

    m_pCabinet->updateParam();
    m_pCabinet->setCabinetNo(1);
#endif
    return m_pCabinet;
}

QString CDataMgr::getTotalData()
{
    QString rc;
#if MSZ
    double power = m_pCabinet->power();
    double totalPower;
    QFile file(TOTAL_DATA_FILE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open total.dat fail";
        return rc;
    }
    QTextStream in(&file);
    QString totalDate = in.readAll();
    file.close();
    if(totalDate.isEmpty())
    {
        QString str = QString("%1 = %2")
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(QString::number(IPCSTATUS.m_Reg00010000));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "open total.dat fail";
            return rc;
        }
        QTextStream out(&file);
        out << str;
        file.close();;
        totalDate = str;
    }
    QStringList totalDateList = totalDate.split(" = ");
    totalPower = totalDateList[1].toDouble();
    double dtemp = QString::number(power / 1000 * (QDateTime::fromString(totalDateList[0],"yyyy-MM-dd hh:mm:ss").toTime_t() - 
                                   QDateTime::currentDateTime().toTime_t()) / 3600,'g',3).toDouble();
    if(dtemp > 5.0)
        dtemp = 0.0;
    totalPower += dtemp;
    IPCSTATUS.m_Reg00010000 = totalPower;
    IPCSTATUS.Update();
    QString input = QString("%1 = %2")
                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(QString::number(totalPower));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "open total.dat fail";
        return rc;
    }
    QTextStream out(&file);
    out << input;
    file.close();

    rc = QString::number(totalPower,'f',2);
#endif
    return rc;
}

bool CDataMgr::getConstanceAsTask(CConstanceAsTask& constanceAsTask)
{
    bool rc = false;
#if MSZ
    // read com
    INT8U dataBuf[4] = {0};
    sendCommands(READ_CONSTANCE_AS_TASK_CMD,dataBuf);

    QDateTime dateTime;
    for(int i = 0; i < CONS_IPC_CONSTANTLUXTASK_MAX_COUNT;i++)
    {
        if(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_TaskNo != 0)
        {
            rc = true;
            constanceAsTask.setEnable(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_Enable);    // 使能
            constanceAsTask.setId(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_TaskNo);        // 任务号1~12
            constanceAsTask.setPrority(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_Priority); // 优先级
            constanceAsTask.setCircuitSelection(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_ClrelayNo); // 回路号
            dateTime.setTime_t(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_StartTime);
            constanceAsTask.setStartTime(dateTime.toString("yyyy-MM-dd hh:mm:ss"));                         // 起始时间
            dateTime.setTime_t(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_EndTime);
            constanceAsTask.setEndTime(dateTime.toString("yyyy-MM-dd hh:mm:ss"));                           // 结束时间
            constanceAsTask.setWeekSelection(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_PeriodDetailValue);  // 循环方式
            constanceAsTask.setSensorSelection(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_LuxSensorsBS);     //光照传感器
            constanceAsTask.setConstanceLux(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_ConstantLux);         // 恒照值
            constanceAsTask.setBeginHour(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_TimePeriodInDay[0][0]);
            constanceAsTask.setBeginMins(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_TimePeriodInDay[0][1]);
            constanceAsTask.setEndHour(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_TimePeriodInDay[0][2]);
            constanceAsTask.setEndMins(IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[i].m_TimePeriodInDay[0][3]);
            INT8U weight[16];
            for(int i = 0;i < sizeof(weight);i++)
            {
                weight[i] = IPCSTATUS.m_DeviceCfgData[97 + i].m_LuxRate;
            }
            constanceAsTask.setWeight(weight,sizeof(weight));
        }
    }
#endif
    return rc;
}

void CDataMgr::saveConstanceAsTask(CConstanceAsTask& constanceAsTask)
{
#if MSZ
    QDateTime dateTime;
    int id = constanceAsTask.id();
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_Enable = constanceAsTask.enable();       // 使能
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_TaskNo = id;                             // ID
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_Priority = constanceAsTask.priority();   // 优先级
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_ClrelayNo = constanceAsTask.circuitSelection(); // 回路
    dateTime = QDateTime::fromString(constanceAsTask.startTime(),"yyyy-MM-dd hh:mm:ss");                // 起始时间
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_StartTime = dateTime.toTime_t();
    dateTime = QDateTime::fromString(constanceAsTask.endTime(),"yyyy-MM-dd hh:mm:ss");                  // 起始时间
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_EndTime = (time_t)dateTime.toTime_t();
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_LuxSensorsBS = constanceAsTask.sensorSelection();
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_ConstantLux = constanceAsTask.constanceLux(); // 启动阀值
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_PeriodDetailValue = constanceAsTask.weekSelection(); // 循环方式
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_PeriodUnit = EM_IPC_PERIODUNIT_WEEK; // 周期细节描述
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_TimePeriodInDay[0][0] = constanceAsTask.beginHour();
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_TimePeriodInDay[0][1] = constanceAsTask.beginMins();
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_TimePeriodInDay[0][2] = constanceAsTask.endHour();
    IPCSHAREMEM.m_IpcConstantLuxTasks.m_ConstantLuxTasks[id].m_TimePeriodInDay[0][3] = constanceAsTask.endMins();
    INT8U weight[16] = {0};
    constanceAsTask.getWeight(weight,16);
    for(int i = 0;i < sizeof(weight);i++)
    {
        IPCSTATUS.m_DeviceCfgData[97 + i].m_LuxRate = weight[i];
    }

    // save com
    INT8U dataBuf[4] = {0};
    sendCommands(SAVE_CONSTANCE_AS_TASK_CMD,dataBuf);
#endif
}

int CDataMgr::getLightTask()
{
    int count = -1;
#if MSZ
    INT8U dataBuf[4] = {0};
    sendCommands(READ_LIGHT_TASK_CMD,dataBuf);

    clearLightTaskList();

    for(int i = 0; i < CONS_IPC_LUXTASK_MAX_COUNT + 1;i++)
    {
        if(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_TaskNo != 0)
        {
            CLightTask *lightTask = new CLightTask;
            QDateTime dateTime;
            lightTask->setEnable(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Enable);          // 使能
            lightTask->setId(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_TaskNo);              // 任务号1~12
            lightTask->setPrority(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Priority);       // 优先级
            dateTime.setTime_t(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_StartTime);
            lightTask->setStartTime(dateTime.toString("yyyy-MM-dd hh:mm:ss"));               // 起始时间
            dateTime.setTime_t(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_EndTime);
            lightTask->setEndTime(dateTime.toString("yyyy-MM-dd hh:mm:ss"));                 // 结束时间
            lightTask->setWeekSelection(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_PeriodDetailValue); // 循环方式
            lightTask->setSensorSelection(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_LuxSensorsBS);    //光照传感器
            // 运行状态暂无
            lightTask->setStartLux(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_StartLuxlimit); // 启动阀值
            lightTask->setEndLux(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_EndLuxlimit);     // 结束阀值
            lightTask->setCircuitSelection(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[0].m_ActionItems[0].m_DeviceNo);// 获取回路信息
            int planCount = 0;
            for(int j = 0;j < CONS_IPC_LUXTASKPLAN_MAX_COUNT;j++){
                if(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[j].m_Enable)
                    planCount++;
            }
            lightTask->setOrderCount(planCount);                                             // 指令个数 最大12个
            if(planCount){
                // 获取指令方案
                for(int l = 0;l < planCount;l++)
                {
                    lightTask->lightOrder()[l].setMinutes(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[l].m_KeepMins);
                    if(0x2D == IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[l].m_ActionItems[0].m_Function)
                        lightTask->lightOrder()[l].setOrder(12);
                    if(0x2B == IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[l].m_ActionItems[0].m_Function)
                    {
                        QString str = QString::number(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[l].m_ActionItems[0].m_FuncData[0] & 0xff,16);
                        str += QString::number(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[l].m_ActionItems[0].m_FuncData[1] & 0xff,16);
                        if(IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[l].m_ActionItems[0].m_FuncData[1] == 0)
                            str += "0";
                        int vol = str.toInt() / 10;
                        int line = IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[i].m_Plans[l].m_ActionItems[0].m_DeviceNo;
                        lightTask->lightOrder()[l].setOrder(getCloseFunNo(line,vol));
                    }
                }
            }
            m_lLightTaskList.push_back(lightTask);
        }
    }
    count = m_lLightTaskList.size();
#endif
    return count;
}

CLightTask* CDataMgr::getLightTask(int index)
{
    Q_ASSERT(index != -1 && m_lLightTaskList.size() > 0);

    return m_lLightTaskList[index];
}

void CDataMgr::saveLightTask(CLightTask& lightTask)
{
#if MSZ
    QDateTime dateTime;
    int id = lightTask.id();
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Enable = lightTask.enable();             // 使能
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_TaskNo = id;                             // ID
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Priority = lightTask.priority();         // 优先级
    dateTime = QDateTime::fromString(lightTask.startTime(),"yyyy-MM-dd hh:mm:ss");      // 起始时间
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_StartTime = dateTime.toTime_t();
    dateTime = QDateTime::fromString(lightTask.endTime(),"yyyy-MM-dd hh:mm:ss");        // 起始时间
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_EndTime = dateTime.toTime_t();
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_LuxSensorsBS = lightTask.sensorSelection();
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_StartLuxlimit = lightTask.startLux();    // 启动阀值
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_EndLuxlimit = lightTask.endLux();        // 结束阀值
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_PeriodDetailValue = lightTask.weekSelection(); // 循环方式
    IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_PeriodUnit = EM_IPC_PERIODUNIT_WEEK;     // 周期细节描述
    for(int n = 0;n < CONS_IPC_LUXTASKPLAN_MAX_COUNT;n++)                               // 先将所有的指令使能设置为false
    {
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[n].m_Enable = false;
    }
    for(int i = 0;i < lightTask.orderCount();i++)                                       // 然后开始设置指令
    {
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_Enable = true;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_KeepMins = lightTask.lightOrder()[i].minutes();
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItemsNum = 1;       // 当前只有一个回路 所有actionItemsNum = 1
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_SubNo = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_DeviceNo = lightTask.circuitSelection();
        if(12 == lightTask.lightOrder()[i].order())
        {
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_Function = 0x2D;
        }
        if((lightTask.lightOrder()[i].order() >= 0) && (12 != lightTask.lightOrder()[i].order()))
        {
            bool ok;
            QString str = getPrecentVol(lightTask.lightOrder()[i].order(),IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_DeviceNo);
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_Function = 0x2B;
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_FuncData[0] = str.left(2).toInt(&ok,16);
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_FuncData[1] = str.right(2).toInt(&ok,16);
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_FuncData[2] = 0;
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[i].m_ActionItems[0].m_FuncData[3] = 0;
        }
    }

    // save com
    INT8U dataBuf[4] = {0};
    sendCommands(SAVE_LIGHT_TASK_CMD,dataBuf);
#endif 
}

bool CDataMgr::delLightTask(int index)
{
    bool rc = false;
#if MSZ

    if (index != -1)
    {
        rc = true;
        int id = m_lLightTaskList[index]->id();
        clearTaskRecord(m_lLightTaskList[index]->circuitSelection());   // 清除数据库中的标记
        delete m_lLightTaskList.takeAt(index);                          // 删除本地数据

        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_TaskNo = 0;       // ID
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Enable = 0;       // 使能
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Priority = 0;     // 优先级
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_StartTime = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_EndTime = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_PeriodUnit = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_PeriodDetailValue = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_RunState = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_StartLuxlimit = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_EndLuxlimit = 0;
        IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_LuxSensorsBS = 0;

        for(int n = 0;n < CONS_IPC_LUXTASKPLAN_MAX_COUNT;n++)           // 将所有的指令使能设置为false
        {
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[n].m_Enable = 0;
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[n].m_KeepMins = 0;
            IPCSHAREMEM.m_IpcLuxTasks.m_LuxTasks[id].m_Plans[n].m_ActionItemsNum = 0;
        }

        // del com
        INT8U dataBuf[4] = {0};
        sendCommands(DEL_LIGHT_TASK_CMD,dataBuf);
    }
#endif
    return rc;
}

int CDataMgr::getTimeTask()
{
    int count = -1;
#if MSZ
    // read com
    INT8U dataBuf[4] = {0};
    sendCommands(READ_TIME_TASK_CMD,dataBuf);

    clearTimeTaskList();

    for(int i = 0; i < CONS_IPC_NORTASK_MAX_COUNT + 1;i++)
    {
        if(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_TaskNo != 0)
        {
            CTimeTask *timeTask = new CTimeTask;
            QDateTime dateTime;
            timeTask->setEnable(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Enable);              // 使能
            timeTask->setId(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_TaskNo);                  // 任务号1~12
            timeTask->setPriority(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Priority);          // 优先级
            dateTime.setTime_t(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_StartTime);
            timeTask->setStartTime(dateTime.toString("yyyy-MM-dd hh:mm:ss"));                // 起始时间
            dateTime.setTime_t(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_EndTime);
            timeTask->setEndTime(dateTime.toString("yyyy-MM-dd hh:mm:ss"));                  // 结束时间
            timeTask->setWeekSelection(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_PeriodDetailValue); // 循环方式
            int planCount = 0;
            for(int j = 0;j < CONS_IPC_TASKPLAN_MAX_COUNT;j++){
                if(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[j].m_Enable)
                    planCount++;
            }
            timeTask->setOrderCount(planCount);                                              // 指令个数 最大12个
            S_WORD circuit = 0;
            if(planCount > 0){
                // 获取回路
                for(int k = 0; k < IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[0].m_ActionItemsNum;k++){
                    for (int n = 1;n <= 16;n++){
                        if (n == IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[0].m_ActionItems[k].m_DeviceNo)
                            circuit |= (0x0001 << (n - 1));
                    }
                }
                timeTask->setCircuitSelection(circuit);
                // 获取指令方案
                for(int l = 0;l < planCount;l++)
                {
                    for(int m = 0; m < IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_ActionItemsNum;m++)
                    {
                        timeTask->timeOrder()[l].setHour(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_BeginTimeHHMM[0]);
                        timeTask->timeOrder()[l].setMinute(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_BeginTimeHHMM[1]);
                        if(0x2D == IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_ActionItems[m].m_Function)
                            timeTask->timeOrder()[l].setOrder(12);
                        if(0x2B == IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_ActionItems[m].m_Function)
                        {
                            QString str = QString::number(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_ActionItems[m].m_FuncData[0] & 0xff,16);
                            str += QString::number(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_ActionItems[m].m_FuncData[1] & 0xff,16);
                            if(IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_ActionItems[m].m_FuncData[1] == 0)
                                str += "0";
                            int vol = str.toInt() / 10;
                            int line = IPCSHAREMEM.m_IpcNorTasks.m_Tasks[i].m_Plans[l].m_ActionItems[m].m_DeviceNo;
                            timeTask->timeOrder()[l].setOrder(getCloseFunNo(line,vol));
                        }
                    }
                }
            }
            m_lTimeTaskList.push_back(timeTask);
        }
    }
#endif
    count = m_lTimeTaskList.size();
    return count;
}

CTimeTask* CDataMgr::getTimeTask(int index)
{
    Q_ASSERT(index != -1 && m_lTimeTaskList.size() > 0);

    return m_lTimeTaskList[index];
}

void CDataMgr::saveTimeTask(CTimeTask& timeTask)
{
#if MSZ
    QDateTime dateTime;
    int id = timeTask.id();
    int circuitCount = timeTask.circuitCount();                                             // 回路个数
    IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Enable = timeTask.enable();                     // 使能
    IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_TaskNo = id;                                    // ID
    IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Priority = timeTask.priority();                 // 优先级
    dateTime = QDateTime::fromString(timeTask.startTime(),"yyyy-MM-dd hh:mm:ss");           // 起始时间
    IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_StartTime = dateTime.toTime_t();
    dateTime = QDateTime::fromString(timeTask.endTime(),"yyyy-MM-dd hh:mm:ss");             // 结束时间
    IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_EndTime = dateTime.toTime_t();
    IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_PeriodDetailValue = timeTask.weekSelection();   // 循环方式
    IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_PeriodUnit = EM_IPC_PERIODUNIT_WEEK;            // 周期细节描述
    for(int n = 0;n < CONS_IPC_TASKPLAN_MAX_COUNT;n++)                                      // 先将所有的指令使能设置为false
    {
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[n].m_Enable = false;
    }
    for(int i = 0;i < timeTask.orderCount();i++) // 然后开始设置指令
    {
        bool ok;
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_Enable = true;
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_BeginTimeHHMM[0] = timeTask.timeOrder()[i].hour();
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_BeginTimeHHMM[1] = timeTask.timeOrder()[i].minute();
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItemsNum = circuitCount;
        for(int j = 0;j < circuitCount;j++)
        {
            IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_SubNo = 0;
            IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_DeviceNo = timeTask.getCircuitList()[j];
            if(12 == timeTask.timeOrder()[i].order())
            {
                IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_Function = 0x2D;
            }
            else if((timeTask.timeOrder()[i].order()>= 0) && (timeTask.timeOrder()[i].order() != 12))
            {
                bool ok;
                QString str = getPrecentVol(timeTask.timeOrder()[i].order(),IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_DeviceNo);
                IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_Function = 0x2B;
                IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_FuncData[0] = str.left(2).toInt(&ok,16);
                IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_FuncData[1] = str.right(2).toInt(&ok,16);
                IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_FuncData[2] = 0;
                IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[i].m_ActionItems[j].m_FuncData[3] = 0;
            }
        }
    }

    // save com
    INT8U dataBuf[4] = {0};
    sendCommands(SAVE_TIME_TASK_CMD,dataBuf);
#endif
}

bool CDataMgr::delTimeTask(int index)
{
    bool rc = false;
#if MSZ

    if (index != -1)
    {
        rc = true;
        int id = m_lTimeTaskList[index]->id();
        // 清除数据库中的标记
        for (int i = 0;i < m_lTimeTaskList[index]->getCircuitList().size();i++)
        {
            clearTaskRecord(m_lTimeTaskList[index]->getCircuitList()[i]);
        }
        delete m_lTimeTaskList.takeAt(index);                           // 删除本地的数据

        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_TaskNo = 0;          // ID
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Enable = 0;          // 使能
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Priority = 0;        // 优先级
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_StartTime = 0;       // 起始时间
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_EndTime = 0;         // 结束时间
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_PeriodUnit = 0;      // 循环方案
        IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_PeriodDetailValue = 0; // 循环细节
        for(int n = 0;n < CONS_IPC_TASKPLAN_MAX_COUNT;n++)              // 将所有的指令使能设置为false
        {
            IPCSHAREMEM.m_IpcNorTasks.m_Tasks[id].m_Plans[n].m_Enable = false;
        }

        // del com
        INT8U dataBuf[4] = {0};
        sendCommands(DEL_TIME_TASK_CMD,dataBuf);
    }
#endif
    return rc;
}

bool CDataMgr::getWarnList(QStringList& warnList)
{
    warnList.clear();
#if MSZ
    QFile file(WARN_CONFIG_FILE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open warningconfig.dat fail";
        return false;
    }
    QTextStream in(&file);
    warnList.clear();
    QString temp = in.readLine();
    while (!temp.isNull())
    {
        warnList.append(temp);
        temp = in.readLine();
    }
    file.close();
#endif
    return true;
}

QString CDataMgr::getTelNo()
{
    QString telNo;
#if MSZ
    for(int i = 0; i < 6;i++)
    {
        if(i != 0){
            if(IPCSHAREMEM.m_TeleNo[i] < 10)
                telNo += "0";
            telNo += QString::number(IPCSHAREMEM.m_TeleNo[i]&0xff,16);
        }
        else
            telNo += QString::number(IPCSHAREMEM.m_TeleNo[i]&0x0f,16);
    }
#endif
    return telNo;
}

void CDataMgr::setTelNo(const QString& telNo)
{
#if MSZ
    bool ok;
    IPCSHAREMEM.m_TeleNo[0] = telNo.left(1).toInt(&ok,16) | 0xa0;
    for(int i = 1;i < 6;i++)
    {
        IPCSHAREMEM.m_TeleNo[i] = telNo.mid(2 * i - 1,2).toInt(&ok,16);
    }

    INT8U dataBuf[4] = {0};

    sendCommands(SET_TEL_NUM_CMD,dataBuf);
#endif 
}

bool CDataMgr::getIpInfo(QStringList& ipInfo)
{
#if MSZ
    INT8U dataBuf[4] = {0};
    sendCommands(GET_IFCONFIG_CMD,dataBuf);

    QString str;
    for(int i = 0; i < 4;i++)
    {
        str += QString::number(IPCSHAREMEM.m_Eth0Config.m_TermIP[i]);
        if((0 == i % 2) || (1 == i))
            str += ".";
    }

    ipInfo.append(str);
    str.clear();
    for(int i = 0; i < 4;i++)
    {
        str += QString::number(IPCSHAREMEM.m_Eth0Config.m_NetMask[i]);
        if((0 == i % 2) || (1 == i))
            str += ".";
    }

    ipInfo.append(str);
    str.clear();
    for(int i = 0; i < 4;i++)
    {
        str += QString::number(IPCSHAREMEM.m_Eth0Config.m_GateWay[i]);
        if((0 == i % 2) || (1 == i))
            str += ".";
    }

    ipInfo.append(str);
    str.clear();

    // 获取服务器IP和端口
    for(int i = 0; i < 4;i++)
    {
        str += QString::number(IPCSHAREMEM.m_ServerIpConfig.m_IP[i]);
        if((0 == i % 2) || (1 == i))
            str += ".";
    }
    ipInfo.append(str);

    str = QString::number(IPCSHAREMEM.m_ServerIpConfig.m_Port);
    ipInfo.append(str);
#endif
    return true;
}

bool CDataMgr::setIpInfo(QStringList& ipInfo)
{
#if MSZ
    bool ok;
    QStringList strList = ipInfo[0].split(".");
    for(int i = 0;i < 4;i++)
    {
        IPCSHAREMEM.m_Eth0Config.m_TermIP[i] = strList[i].toInt(&ok);
    }

    strList = ipInfo[1].split(".");
    for(int i = 0;i < 4;i++)
    {
        IPCSHAREMEM.m_Eth0Config.m_NetMask[i] = strList[i].toInt(&ok);
    }

    strList = ipInfo[2].split(".");
    for(int i = 0;i < 4;i++)
    {
        IPCSHAREMEM.m_Eth0Config.m_GateWay[i] = strList[i].toInt(&ok);
    }

    // 保存云端IP信息
    strList = ipInfo[3].split(".");
    for(int i = 0;i < 4;i++)
    {
        IPCSHAREMEM.m_ServerIpConfig.m_IP[i] = strList[i].toInt(&ok);
    }
    // 服务器端口
    IPCSHAREMEM.m_ServerIpConfig.m_Port = ipInfo[4].toInt();

    INT8U dataBuf[4] = {0};
    sendCommands(SET_LOCAL_IP_INFO_CMD,dataBuf);
    sendCommands(SET_SERVER_IP_INFO_CMD,dataBuf);
#endif
    return true;
}

void CDataMgr::sendCommands(INT8U fun,INT8U* dataBuf,INT8U circuitNo,INT8U subNo)
{
#if MSZ
    while(false == IPCSHAREMEM.m_IpcRealControlCommandQueue.m_bBusy)
    {
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_Num = 1;
        int num = IPCSHAREMEM.m_IpcRealControlCommandQueue.m_Num;
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_RealCtlCmd[num - 1].m_DeviceNo = circuitNo; //
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_RealCtlCmd[num - 1].m_SubNo = subNo;
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_RealCtlCmd[num - 1].m_Function = fun;
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_RealCtlCmd[num - 1].m_FuncData[0] = dataBuf[0];
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_RealCtlCmd[num - 1].m_FuncData[1] = dataBuf[1];
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_RealCtlCmd[num - 1].m_FuncData[2] = 0;
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_RealCtlCmd[num - 1].m_FuncData[3] = 0;
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_bBusy = true;
        IPCSHAREMEM.m_IpcRealControlCommandQueue.m_FinishedNum = 0;
        IPCSHAREMEM.Update();
    }
#endif 
}

void CDataMgr::syncTime(const QDateTime& dateTime)
{
    QString setTimeCmd = QString("date -s \"%1\"").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss"));
    QString sync = "hwclock -w";

#ifndef WIN32
    system(setTimeCmd.toLatin1().data());                               // linux下的校时操作
    system(sync.toLatin1().data());                                     // 将时钟同步到硬件时钟
#endif
}

void CDataMgr::setQtTime()
{
#if MSZ
    QDateTime sysTime = QDateTime::currentDateTime();
    IPCSHAREMEM.m_QtRunTimeStamp = sysTime.toTime_t();                  // 同步qt的共享内存
    QString sysTimeStr = sysTime.toString("yyyy-MM-dd hh:mm:ss");

    // qt的时间戳
    QFile file(QT_TIME_FILE);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "open QtTime.dat fail";
        return;
    }
    QTextStream out(&file);
    out << sysTimeStr;
    file.close();
#endif
}

bool CDataMgr::getCircuitWarnState(int index)
{
#if MSZ
    if (IPCSTATUS.m_ClleakState[index])
        return true;
    else
#endif
        return false;
}

int CDataMgr::getCloseFunNo(INT8U circuitNo,int vol)
{
    int precentNo = 0;
    int DValue[11] = {0};
    int minValue = 0;
    QString sql;
    sql = QString("SELECT * FROM dim WHERE circuit = \'L#%1%2\'")
             .arg(m_sCpcNo)
             .arg(circuitNo, 2, 10, QChar('0'));
    QSqlQuery query;
    query.exec(sql);
    if(query.next())                                                    // 不执行词句会有报错
    {
        for(int i = 1;i <= 11;i++)
        {
            int absValue = query.value(i).toInt() - vol;
            DValue[i - 1] = abs(absValue);
            minValue = min(minValue,DValue[i - 1]);
        }
        for(int i = 1;i <= 11;i++)
        {
            if(minValue == DValue[i - 1])
                precentNo = i;
        }
    }

    return precentNo;
}

QString CDataMgr::getPrecentVol(int precentNo,INT8U circuitNo)
{
    int perVolage = 0;
    QString sql,volValue,precent;
    getPrecent(precentNo,precent);
    sql = QString("SELECT %1 FROM dim WHERE circuit = \'L#%2%3\'")
          .arg(precent)
          .arg(m_sCpcNo)
          .arg(circuitNo, 2, 10, QChar('0'));
    QSqlQuery query;
    query.exec(sql);
    if(query.next())
    {
        perVolage = query.value(0).toInt() * 10;
        volValue = QString::number(perVolage);
    }

    return volValue;
}

bool CDataMgr::getCircuitEnable(int circuitNo)
{
    bool ret = false;
    QString sql;
    sql = QString("SELECT selected FROM line WHERE circuit = \'L#%1%2\'")
          .arg(m_sCpcNo)
          .arg(circuitNo, 2, 10, QChar('0'));

    QSqlQuery query;
    query.exec(sql);
    if(query.next()) {                                                  // 不执行词句会有报错
        if (query.value(0).toInt())
            ret = true;
    }

    return ret;
}

int CDataMgr::getCircuitBelong(int circuitNo)
{
    int belong = -1;
    QString sql = QString("SELECT belong FROM line WHERE circuit = \'L#%1%2\'")
           .arg(m_sCpcNo)
           .arg(circuitNo, 2, 10, QChar('0'));

    QSqlQuery query;
    query.exec(sql);
    if(query.next()) {                                                  // 不执行词句会有报错
        belong = query.value(0).toInt();
    }

    return belong;
}

void CDataMgr::setTasktRecord(int type,int circuitNo)
{
    QString sql = QString("UPDATE line SET selected = %1,belong = 1 WHERE circuit = \'L#%2%3\'")
            .arg(QString::number(type))
            .arg(m_sCpcNo)
            .arg(circuitNo, 2, 10, QChar('0'));

    QSqlQuery query;
    query.exec(sql);
    query.next();
}

void CDataMgr::clearTaskRecord(int circuitNo)
{
    QString sql;
    sql = QString("UPDATE line SET selected = 0,belong = 0 WHERE circuit = \'L#%1%2\'")
        .arg(m_sCpcNo)
        .arg(circuitNo, 2, 10, QChar('0'));

    QSqlQuery query;
    query.exec(sql);
    query.next();
}

bool CDataMgr::enableSensor(int index)
{
#if MSZ
    if (IPCSHAREMEM.m_IpcDevicesRegData[index].m_DeviceType == MD_LUX_SENSOR)
        return true;
    else
#endif
        return false;
}

void CDataMgr::setChannel(int index,int channel)
{
#if MSZ
    IPCSHAREMEM.m_IpcClLeakInfo[index].m_ChannelNo = channel;
#endif
}

void CDataMgr::setDtVol(int index,int vol)
{
#if MSZ
    IPCSHAREMEM.m_IpcClLeakInfo[index].m_DtVol = vol;
#endif
}

void CDataMgr::setDelayMs(int index,int delay)
{
#if MSZ
    IPCSHAREMEM.m_IpcClLeakInfo[index].m_DelayMs = delay;
#endif
}

void CDataMgr::setDefendEnable(int index,int enable)
{
#if MSZ
    IPCSHAREMEM.m_IpcClLeakInfo[index].m_Enable = enable;
#endif
}

void CDataMgr::setCutEnable(int index,int enable)
{
#if MSZ
    IPCSHAREMEM.m_IpcClLeakInfo[index].m_CutEnable = enable;
#endif
}

QString CDataMgr::getLogicAddr()
{
    QString str;
#if MSZ
    str = QString("%1%2%3%4")
             .arg(IPCSHAREMEM.m_cRTUA[0], 2, 16, QChar('0'))
             .arg(IPCSHAREMEM.m_cRTUA[1], 2, 16, QChar('0'))
             .arg(IPCSHAREMEM.m_cRTUA[2], 2, 10, QChar('0'))
             .arg(IPCSHAREMEM.m_cRTUA[3], 2, 10, QChar('0'));
#endif
    return str;
}

QString CDataMgr::getSoftVersion()
{
    return "1.7";
}

void CDataMgr::getPrecent(int value,QString& precent)
{
    switch(value)
    {
    case 11:
        precent = "precentZero";
        break;
    case 10:
        precent = "precentTen";
        break;
    case 9:
        precent = "precentTwenty";
        break;
    case 8:
        precent = "precentThirty";
        break;
    case 7:
        precent = "precentForty";
        break;
    case 6:
        precent = "precentFifty";
        break;
    case 5:
        precent = "precentSixty";
        break;
    case 4:
        precent = "precentSeventy";
        break;
    case 3:
        precent = "precentEighty";
        break;
    case 2:
        precent = "precentNinty";
        break;
    case 1:
        precent = "precentHundred";
        break;
    default:
        break;
    }
}

void CDataMgr::clearTimeTaskList()
{
    Q_FOREACH(CTimeTask* timeTask,m_lTimeTaskList){
        if (timeTask)
            delete timeTask;
    }

    m_lTimeTaskList.clear();
}

void CDataMgr::clearLightTaskList()
{
    Q_FOREACH(CLightTask* lightTask,m_lLightTaskList){
        if (lightTask)
            delete lightTask;
    }

    m_lLightTaskList.clear();
}

CDataMgr::CDataMgr()
{
    m_pCabinet = new CCabinet;
    m_sCpcNo = "01";
    QString str = QString("L#%1").arg(m_sCpcNo);
    if(!createConnection(str))
        qDebug() << "sql error";
}

CDataMgr::~CDataMgr()
{
    delete m_pCabinet;

    clearLightTaskList();
    clearTimeTaskList();
}
