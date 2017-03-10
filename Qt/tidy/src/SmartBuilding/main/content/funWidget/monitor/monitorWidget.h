/****************************************************************************
**
** file: monitorWidget.h
**
** description:
**     ���й��ܽ���
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
    void setAging();                                                    // ����ʱЧ
    void dimming(int precent,int circuitNo);                            // ����
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
    QComboBox*                  m_pDimComBox;                           // ��·
    QLineEdit*                  m_pTimeLiness;
    QLineEdit*                  m_pDimValue;
    QLineEdit*                  m_pTotalPower;
    QPushButton*                m_pTenAddBtn;
    QPushButton*                m_pTenSubBtn;
    QPushButton*                m_pSwitchBtn;
    QPushButton*                m_pBackBtn;
    QFrame*                     m_pRightFrame;

    bool                        m_bShowRectifer;                        // �Ƿ��������������
    bool                        m_bTurning;                             // ��·�������ڽ�����
    bool                        m_bSwitched;                            // ��·״̬ falseΪ�ر�
    int                         m_iCurrentRow;
    int                         m_iCurrentColumn;
    QString                     m_sCpcNo;
    QStringList                 m_lWarnList;
};

#endif // MONITORWIDGET_H
