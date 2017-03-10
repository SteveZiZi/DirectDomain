/****************************************************************************
**
** file: monitorWidget.h
**
** description:
**     运行功能界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

class CReportWidget;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QFrame;
class QTableWidgetItem;
class QTimer;
class CDataMgr;
class CCabinet;
class CCircuit;
class CRectifier;
class CMonitorW : public QWidget
{
    Q_OBJECT

public:
    CMonitorW(QWidget *parent = 0);
    ~CMonitorW();

private slots:
    void changeState(int state);
    void addTenPrecent();
    void subTenPrecent();
    void turnOnOff();
    void getCircuitInfo(int circuitNo);
    void backSlot();
    void updateShow();
    void showRectifierData(int row,int column);
    void showWarnDetail(QTableWidgetItem* item);

private:
    void init_();
    void layout_();
    void localConnect();
    void setAging();                                                    // 设置时效
    void dimming(int precent,int circuitNo);                            // 调光
    void setSwitchState(bool state);
    void setCabinetData(CCabinet* cabinet);
    void setCircuitData(int row,CCircuit* circuit);
    void setRectifierData(int row,CRectifier* rectifier);
    QString getRectifierState(INT16S state);

private:
    CDataMgr*                   m_pDataMgr;

    QTimer*                     m_pUpdateTimer;
    CReportWidget*              m_pCabinetReport;
    CReportWidget*              m_pDataReport;
    QCheckBox*                  m_pManualCheck;
    QComboBox*                  m_pDimComBox;                           // 回路
    QLineEdit*                  m_pTimeLiness;
    QLineEdit*                  m_pDimValue;
    QLineEdit*                  m_pTotalPower;
    QPushButton*                m_pTenAddBtn;
    QPushButton*                m_pTenSubBtn;
    QPushButton*                m_pSwitchBtn;
    QPushButton*                m_pBackBtn;
    QFrame*                     m_pRightFrame;

    bool                        m_bShowRectifer;                        // 是否呈现整流器内容
    bool                        m_bTurning;                             // 回路开关正在进行中
    bool                        m_bSwitched;                            // 回路状态 false为关闭
    int                         m_iCurrentRow;
    int                         m_iCurrentColumn;
    QString                     m_sCpcNo;
    QStringList                 m_lWarnList;
};

#endif // MONITORWIDGET_H
