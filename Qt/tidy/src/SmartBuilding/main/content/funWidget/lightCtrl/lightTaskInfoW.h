/****************************************************************************
**
** file: lightTaskInfo.h
**
** description:
**     光控控制功能详细信息
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LIGHTTASKINFOW_H
#define LIGHTTASKINFOW_H

#include "src/SmartBuilding/main/content/common_ctl/taskBaseInfoW.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/lightTask.h"

class QLineEdit;
class CSensorSelectionW;
class QListWidget;
class CDataMgr;
class CLightTaskInfoW : public CTaskBaseInfoW
{
    Q_OBJECT

public:
    CLightTaskInfoW(QWidget *parent = 0,bool exit = false,int index = -1);
    ~CLightTaskInfoW();

private slots:
    void saveTaskInfo();
    void addAction();
    void delAction();
    bool checkCircuit(int index);

private:
    virtual void init_();
    virtual void layout_();
    virtual void localConnect();
    void updateTaskInfo();
    void addAction(CLightOrder& lightOrder);
    bool saveTask();                                                    // 将界面中的数据交给底层

private:
    CDataMgr*                       m_pDataMgr;

    QComboBox*                      m_pCircuitCom;

    QPushButton*                    m_pCancelBtn;
    QPushButton*                    m_pAddActionBtn;

    QLineEdit*                      m_pStartLuxEdit;
    QLineEdit*                      m_pEndLuxEdit;
    QListWidget*                    m_pActionWidget;
    CSensorSelectionW*              m_pSensorSelection;

    int                             m_iIndex;
};

#endif // LIGHTTASKINFOW_H
