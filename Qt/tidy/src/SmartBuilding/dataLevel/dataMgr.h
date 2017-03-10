/****************************************************************************
**
** file: dataMgr.h
**
** description:
**     ���ݹ�����
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

#define ADJUST_ORDER_CMD                        43                      // ������·��ѹ
#define TURN_ON_ORDER_CMD                       44                      // �򿪻�·
#define TURN_OFF_ORDER_CMD                      45                      // �رջ�·
#define SET_TEL_NUM_CMD                         50                      // ���õ绰����
#define SET_LOCAL_IP_INFO_CMD                   51                      // ���ñ���IP��Ϣ
#define READ_TIME_TASK_CMD                      52                      // ��ȡʱ������
#define DEL_TIME_TASK_CMD                       53                      // ɾ��ʱ������
#define SAVE_TIME_TASK_CMD                      53                      // ����ʱ������
#define READ_LIGHT_TASK_CMD                     54                      // ��ȡ�������
#define DEL_LIGHT_TASK_CMD                      55                      // ɾ���������
#define SAVE_LIGHT_TASK_CMD                     55                      // ����������
#define READ_CONSTANCE_AS_TASK_CMD              56                      // ��ȡ��������
#define SAVE_CONSTANCE_AS_TASK_CMD              57                      // �����������
#define SET_TIMING_CMD                          58                      // ����ʱЧ
#define SAVE_DEFEND_INFO_CMD                    59                      // ����©�籣����Ϣ
#define CLEAR_BLACK_BOX_CMD                     60                      // ��պ�ϻ��
#define GET_IFCONFIG_CMD                        61                      // ��ȡIP��Ϣ
#define SET_SERVER_IP_INFO_CMD                  62                      // ���÷�����IP��Ϣ

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
     *  @Desc:      ��ȡ��������
     *  @PaamOut:    constanceAsTask
     *  @Return:
     *      bool ��ȡ�Ƿ�ɹ�
     */
    bool getConstanceAsTask(CConstanceAsTask& constanceAsTask);
    /*
     *  @Func:      saveConstanceAsTask
     *  @Desc:      �����������
     *  @PaamOut:    constanceAsTask
     *  @Return:
     *      none
     */
    void saveConstanceAsTask(CConstanceAsTask& constanceAsTask);
    /*
     *  @Func:      getLightTask
     *  @Desc:      ��ȡ����������
     *  @Return:
     *      int ����������
     */
    int getLightTask();
    /*
     *  @Func:      getLightTask
     *  @Desc:      ��ȡ����������
     *  @PaamIn:    index ��������
     *  @Return:
     *      CLightTask �����������
     */
    CLightTask* getLightTask(int index);
    /*
     *  @Func:      saveLightTask
     *  @Desc:      ��������������
     *  @PaamIn:    lightTask ��������
     *  @Return:
     *      none
     */
    void saveLightTask(CLightTask& lightTask);
    /*
     *  @Func:      delLightTask
     *  @Desc:      ɾ���������
     *  @PaamIn:    id id
     *  @Return:
     *      bool true �ɹ� false ʧ��
     */
    bool delLightTask(int index);
    /*
     *  @Func:      getTimeTask
     *  @Desc:      ��ȡʱ���������
     *  @Return:
     *      int ʱ���������
     */
    int getTimeTask();
    /*
     *  @Func:      getTimeTask
     *  @Desc:      ��ȡʱ���������
     *  @PaamIn:    index ��������
     *  @Return:
     *      CLightTask ʱ����������
     */
    CTimeTask* getTimeTask(int index);
    /*
     *  @Func:      saveTimeTask
     *  @Desc:      ����ʱ����������
     *  @PaamIn:    timeTask ��������
     *  @Return:
     *      none
     */
    void saveTimeTask(CTimeTask& timeTask);
    /*
     *  @Func:      delTimeTask
     *  @Desc:      ɾ��ʱ������
     *  @PaamIn:    id id
     *  @Return:
     *      bool true �ɹ� false ʧ��
     */
    bool delTimeTask(int index);
    /*
     *  @Func:      getWarnList
     *  @Desc:      ��ȡ״̬�澯��Ϣ
     *  @PaamOut:    warnList �澯״̬�б�
     *  @Return:
     *      bool true �ɹ� false ʧ��
     */
    bool getWarnList(QStringList& warnList);
    /*
     *  @Func:      getTelNo
     *  @Desc:      ��ȡ�绰����
     *  @PaamIn:    none
     *  @Return:
     *      �绰����
     */
    QString getTelNo();
    /*
     *  @Func:      setTelNo
     *  @Desc:      ���õ绰����
     *  @PaamIn:    telNo
     *  @Return: none
     */
    void setTelNo(const QString& telNo);
    /*
     *  @Func:      getIpInfo
     *  @Desc:      ��ȡIP��Ϣ
     *  @PaamOut:    ipInfo IP
     *  @Return:
     *      bool true �ɹ� false ʧ��
     */
    bool getIpInfo(QStringList& ipInfo);
    /*
     *  @Func:      setIpInfo
     *  @Desc:      ����IP��Ϣ
     *  @PaamOut:    ipInfo IP
     *  @Return:
     *      bool true �ɹ� false ʧ��
     */
    bool setIpInfo(QStringList& ipInfo);
    /*
     *  @Func:      sendCommands
     *  @Desc:      ��������
     *  @PaamIn:    fun ָ�� dataBuf ���� circuitNo ��·�� subNo ��·��
     *  @Return: none
     */
    void sendCommands(INT8U fun,INT8U* dataBuf,INT8U circuitNo = 0,INT8U subNo = 0);
    /*
     *  @Func:      syncTime
     *  @Desc:      ͬ��ʱ��
     *  @PaamIn:    dateTime ʱ����Ϣ
     *  @Return:none
     */
    void syncTime(const QDateTime& dateTime);
    /*
     *  @Func:      setQtTime
     *  @Desc:      ͬ��qt�Ĺ����ڴ�
     *  @PaamIn:    none
     *  @Return:none
     */
    void setQtTime();
    /*
     *  @Func:      getCircuitWarnState
     *  @Desc:      ��ȡ��·�澯״̬
     *  @PaamIn:    index ��·����
     *  @Return:bool true �ɹ� false ʧ��
     */
    bool getCircuitWarnState(int index);
    /*
     *  @Func:      getCloseFunNo
     *  @Desc:      ��ȡ��ѹֵ��Ӧ�İٷֱ�������
     *  @PaamIn:    circuitNo ��·�� vol ��ѹֵ
     *  @Return: int �ٷֱ�������
     */
    int getCloseFunNo(INT8U circuitNo,int vol);
    /*
     *  @Func:      getPrecentVol
     *  @Desc:      ��ȡ�ٷֱȶ�Ӧ�ĵ�ѹֵ
     *  @PaamIn:    precentNo �ٷֱ������� circuitNo ��·��
     *  @Return: ��ѹֵ
     */
    QString getPrecentVol(int precentNo,INT8U circuitNo);
    bool getCircuitEnable(int circuitNo);
    int getCircuitBelong(int circuitNo);
    void setTasktRecord(int type,int circuitNo);
    void clearTaskRecord(int circuitNo);
    /*
     *  @Func:      enableSensor
     *  @Desc:      ���ù�����Ƿ��ѡ
     *  @PaamIn:    index ����
     *  @Return: bool true ��ѡ false ����ѡ
     */
    bool enableSensor(int index);
    /*
     *  @Func:      setChannel
     *  @Desc:      ����©�籣��ͨ��
     *  @PaamIn:    index ���� channel ͨ��
     *  @Return: none
     */
    void setChannel(int index,int channel);
    /*
     *  @Func:      setDtVol
     *  @Desc:      ����©�籣����ֵ
     *  @PaamIn:    index ���� vol ��ѹ��ֵ
     *  @Return: none
     */
    void setDtVol(int index,int vol);
    /*
     *  @Func:      setDelayMs
     *  @Desc:      ����©�籣����ʱʱ��
     *  @PaamIn:    index ���� delay ��ʱʱ��
     *  @Return: none
     */
    void setDelayMs(int index,int delay);
    /*
     *  @Func:      setDefendEnable
     *  @Desc:      ����©�籣��ʹ��
     *  @PaamIn:    index ���� enable ʹ��
     *  @Return: none
     */
    void setDefendEnable(int index,int enable);
    /*
     *  @Func:      setCutEnable
     *  @Desc:      ����©�籣��Ͷ��ʹ��
     *  @PaamIn:    index ���� enable ʹ��
     *  @Return: none
     */
    void setCutEnable(int index,int enable);
    /*
     *  @Func:      getLogicAddr
     *  @Desc:      ��ȡ�߼���ַ
     *  @PaamIn:    none
     *  @Return: �߼���ַ
     */
    QString getLogicAddr();
    /*
     *  @Func:      getSoftVersion
     *  @Desc:      ��ȡ����汾��
     *  @PaamIn:    none
     *  @Return: ����汾
     */
    QString getSoftVersion();
    /*
     *  @Func:      getCPCNo
     *  @Desc:      ��ȡ���й��
     *  @PaamIn:    none
     *  @Return: ���й��
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
