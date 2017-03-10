/****************************************************************************
**
** file: dataMgr.h
**
** description:
**     数据管理类
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef DATAMGR_H
#define DATAMGR_H

#include <QList>
#include <QString>
#include <QDateTime>
#include <QStringList>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/lightTask.h"
#include "src/SmartBuilding/main/content/funWidget/timeCtrl/timeTask.h"
#include "src/SmartBuilding/main/content/funWidget/constanceAs/constanceAsTask.h"

#define ADJUST_ORDER_CMD                        43                      // 调整回路电压
#define TURN_ON_ORDER_CMD                       44                      // 打开回路
#define TURN_OFF_ORDER_CMD                      45                      // 关闭回路
#define SET_TEL_NUM_CMD                         50                      // 设置电话号码
#define SET_LOCAL_IP_INFO_CMD                   51                      // 设置本地IP信息
#define READ_TIME_TASK_CMD                      52                      // 读取时控任务
#define DEL_TIME_TASK_CMD                       53                      // 删除时控任务
#define SAVE_TIME_TASK_CMD                      53                      // 保存时控任务
#define READ_LIGHT_TASK_CMD                     54                      // 读取光控任务
#define DEL_LIGHT_TASK_CMD                      55                      // 删除光控任务
#define SAVE_LIGHT_TASK_CMD                     55                      // 保存光控任务
#define READ_CONSTANCE_AS_TASK_CMD              56                      // 读取恒照任务
#define SAVE_CONSTANCE_AS_TASK_CMD              57                      // 保存恒照任务
#define SET_TIMING_CMD                          58                      // 设置时效
#define SAVE_DEFEND_INFO_CMD                    59                      // 保存漏电保护信息
#define CLEAR_BLACK_BOX_CMD                     60                      // 清空黑匣子
#define GET_IFCONFIG_CMD                        61                      // 获取IP信息
#define SET_SERVER_IP_INFO_CMD                  62                      // 设置服务器IP信息

#define TIME_TASK_TYPE                          1
#define LIGHT_TASK_TYPE                         2
#define CONSTANCE_AS_TASK_TYPE                  3

class CCabinet;
class CDataMgr
{
public:
    static CDataMgr* getInstance();

    CCabinet* getIpcData();
    QString getTotalData();
    /*
     *  @Func:      getConstanceAsTask
     *  @Desc:      获取恒照任务
     *  @PaamOut:    constanceAsTask
     *  @Return:
     *      bool 获取是否成功
     */
    bool getConstanceAsTask(CConstanceAsTask& constanceAsTask);
    /*
     *  @Func:      saveConstanceAsTask
     *  @Desc:      保存恒照任务
     *  @PaamOut:    constanceAsTask
     *  @Return:
     *      none
     */
    void saveConstanceAsTask(CConstanceAsTask& constanceAsTask);
    /*
     *  @Func:      getLightTask
     *  @Desc:      获取光控任务个数
     *  @Return:
     *      int 光控任务个数
     */
    int getLightTask();
    /*
     *  @Func:      getLightTask
     *  @Desc:      获取光控任务个数
     *  @PaamIn:    index 任务索引
     *  @Return:
     *      CLightTask 光控任务详情
     */
    CLightTask* getLightTask(int index);
    /*
     *  @Func:      saveLightTask
     *  @Desc:      保存光控任务详情
     *  @PaamIn:    lightTask 任务详情
     *  @Return:
     *      none
     */
    void saveLightTask(CLightTask& lightTask);
    /*
     *  @Func:      delLightTask
     *  @Desc:      删除光控任务
     *  @PaamIn:    id id
     *  @Return:
     *      bool true 成功 false 失败
     */
    bool delLightTask(int index);
    /*
     *  @Func:      getTimeTask
     *  @Desc:      获取时控任务个数
     *  @Return:
     *      int 时控任务个数
     */
    int getTimeTask();
    /*
     *  @Func:      getTimeTask
     *  @Desc:      获取时控任务个数
     *  @PaamIn:    index 任务索引
     *  @Return:
     *      CLightTask 时控任务详情
     */
    CTimeTask* getTimeTask(int index);
    /*
     *  @Func:      saveTimeTask
     *  @Desc:      保存时控任务详情
     *  @PaamIn:    timeTask 任务详情
     *  @Return:
     *      none
     */
    void saveTimeTask(CTimeTask& timeTask);
    /*
     *  @Func:      delTimeTask
     *  @Desc:      删除时控任务
     *  @PaamIn:    id id
     *  @Return:
     *      bool true 成功 false 失败
     */
    bool delTimeTask(int index);
    /*
     *  @Func:      getWarnList
     *  @Desc:      获取状态告警信息
     *  @PaamOut:    warnList 告警状态列表
     *  @Return:
     *      bool true 成功 false 失败
     */
    bool getWarnList(QStringList& warnList);
    /*
     *  @Func:      getTelNo
     *  @Desc:      获取电话号码
     *  @PaamIn:    none
     *  @Return:
     *      电话号码
     */
    QString getTelNo();
    /*
     *  @Func:      setTelNo
     *  @Desc:      设置电话号码
     *  @PaamIn:    telNo
     *  @Return: none
     */
    void setTelNo(const QString& telNo);
    /*
     *  @Func:      getIpInfo
     *  @Desc:      获取IP信息
     *  @PaamOut:    ipInfo IP
     *  @Return:
     *      bool true 成功 false 失败
     */
    bool getIpInfo(QStringList& ipInfo);
    /*
     *  @Func:      setIpInfo
     *  @Desc:      设置IP信息
     *  @PaamOut:    ipInfo IP
     *  @Return:
     *      bool true 成功 false 失败
     */
    bool setIpInfo(QStringList& ipInfo);
    /*
     *  @Func:      sendCommands
     *  @Desc:      发送命令
     *  @PaamIn:    fun 指令 dataBuf 数据 circuitNo 回路号 subNo 子路号
     *  @Return: none
     */
    void sendCommands(INT8U fun,INT8U* dataBuf,INT8U circuitNo = 0,INT8U subNo = 0);
    /*
     *  @Func:      syncTime
     *  @Desc:      同步时间
     *  @PaamIn:    dateTime 时间信息
     *  @Return:none
     */
    void syncTime(const QDateTime& dateTime);
    /*
     *  @Func:      setQtTime
     *  @Desc:      同步qt的共享内存
     *  @PaamIn:    none
     *  @Return:none
     */
    void setQtTime();
    /*
     *  @Func:      getCircuitWarnState
     *  @Desc:      获取回路告警状态
     *  @PaamIn:    index 回路索引
     *  @Return:bool true 成功 false 失败
     */
    bool getCircuitWarnState(int index);
    /*
     *  @Func:      getCloseFunNo
     *  @Desc:      获取电压值对应的百分比索引号
     *  @PaamIn:    circuitNo 回路号 vol 电压值
     *  @Return: int 百分比索引号
     */
    int getCloseFunNo(INT8U circuitNo,int vol);
    /*
     *  @Func:      getPrecentVol
     *  @Desc:      获取百分比对应的电压值
     *  @PaamIn:    precentNo 百分比索引号 circuitNo 回路号
     *  @Return: 电压值
     */
    QString getPrecentVol(int precentNo,INT8U circuitNo);
    bool getCircuitEnable(int circuitNo);
    int getCircuitBelong(int circuitNo);
    void setTasktRecord(int type,int circuitNo);
    void clearTaskRecord(int circuitNo);
    /*
     *  @Func:      enableSensor
     *  @Desc:      设置光感器是否可选
     *  @PaamIn:    index 索引
     *  @Return: bool true 可选 false 不可选
     */
    bool enableSensor(int index);
    /*
     *  @Func:      setChannel
     *  @Desc:      设置漏电保护通道
     *  @PaamIn:    index 索引 channel 通道
     *  @Return: none
     */
    void setChannel(int index,int channel);
    /*
     *  @Func:      setDtVol
     *  @Desc:      设置漏电保护阀值
     *  @PaamIn:    index 索引 vol 电压阀值
     *  @Return: none
     */
    void setDtVol(int index,int vol);
    /*
     *  @Func:      setDelayMs
     *  @Desc:      设置漏电保护延时时间
     *  @PaamIn:    index 索引 delay 延时时间
     *  @Return: none
     */
    void setDelayMs(int index,int delay);
    /*
     *  @Func:      setDefendEnable
     *  @Desc:      设置漏电保护使能
     *  @PaamIn:    index 索引 enable 使能
     *  @Return: none
     */
    void setDefendEnable(int index,int enable);
    /*
     *  @Func:      setCutEnable
     *  @Desc:      设置漏电保护投切使能
     *  @PaamIn:    index 索引 enable 使能
     *  @Return: none
     */
    void setCutEnable(int index,int enable);
    /*
     *  @Func:      getLogicAddr
     *  @Desc:      获取逻辑地址
     *  @PaamIn:    none
     *  @Return: 逻辑地址
     */
    QString getLogicAddr();
    /*
     *  @Func:      getSoftVersion
     *  @Desc:      获取软件版本号
     *  @PaamIn:    none
     *  @Return: 软件版本
     */
    QString getSoftVersion();
    /*
     *  @Func:      getCPCNo
     *  @Desc:      获取集中柜号
     *  @PaamIn:    none
     *  @Return: 集中柜号
     */
    QString& getCPCNo() {return m_sCpcNo;}
private:
    CDataMgr();
    ~CDataMgr();
    void getPrecent(int value,QString& precent);
    void clearTimeTaskList();
    void clearLightTaskList();

private:
    CCabinet*                   m_pCabinet;

    QString                     m_sCpcNo;
    QList<CLightTask*>          m_lLightTaskList;
    QList<CTimeTask*>           m_lTimeTaskList;

    int                         m_iCircuitCount;
};

#endif // DATAMGR_H
