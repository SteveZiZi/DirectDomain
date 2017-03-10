/****************************************************************************
**
** file: baseTask.h
**
** description:
**     任务基本属性
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef BASETASK_H
#define BASETASK_H

#include <QString>
#include "src/SmartBuilding/inc/xtDef.h"

class CBaseTask
{
public:
    CBaseTask();
    ~CBaseTask();

    INT8U enable() {return m_cEnabel;}
    void setEnable(INT8U enable_) {m_cEnabel = enable_;}
    INT8U id() {return m_cId;}
    void setId(INT8U id_) {m_cId = id_;}
    QString& startTime() {return m_sStartTime;}
    void setStartTime(const QString& startTime_) {m_sStartTime = startTime_;}
    QString& endTime() {return m_sEndTime;}
    void setEndTime(const QString& endTime_) {m_sEndTime = endTime_;}
    INT8U weekSelection() {return m_cWeekSelection;}
    void setWeekSelection(INT8U weekSelection_) {m_cWeekSelection = weekSelection_;}

private:
    INT8U                   m_cEnabel;
    INT8U                   m_cId;
    QString                 m_sStartTime;
    QString                 m_sEndTime;
    INT8U                   m_cWeekSelection;
};

#endif // BASETASK_H
