/****************************************************************************
**
** file: timeTaskInfo.h
**
** description:
**     ʱ����ϸ��Ϣ����
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef TIMETASKINFOW_H
#define TIMETASKINFOW_H

#include "src/SmartBuilding/main/content/common_ctl/taskBaseInfoW.h"
#include "src/SmartBuilding/main/content/funWidget/timeCtrl/timeTask.h"

class QComboBox;
class CCircuitSelection;
class QListWidget;
class QPushButton;
class CDataMgr;
class CTimeTaskInfoW : public CTaskBaseInfoW
{
    Q_OBJECT

public:
    CTimeTaskInfoW(QWidget *parent = 0,bool exit = false,int index = -1); //indexΪ��Ӧ����
    ~CTimeTaskInfoW();

private slots:
    void saveTaskInfo();
    void addAction();
    void delAction();

private:
    virtual void init_();
    virtual void layout_();
    virtual void localConnect();
    void checkCircuit();
    void updateTaskInfo();
    void addAction(CTimeOrder& timeOrder);
    bool saveTask();                                                    // �������е����ݽ����ײ�

private:
    CDataMgr*                       m_pDataMgr;

    QComboBox*                      m_pPriorityCom;
    CCircuitSelection*              m_pCircuitSelection;
    QListWidget*                    m_pActionWidget;

    QPushButton*                    m_pCancelBtn;
    QPushButton*                    m_pAddActionBtn;

    int                             m_iIndex;
};

#endif // TIMETASKINFOW_H
