/****************************************************************************
**
** file: missionWidget.h
**
** description:
**     任务列表控件
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef MISSIONWIDGET_H
#define MISSIONWIDGET_H

#include <QtGui/QWidget>

class CTimeCtrlW;
class CLightCtrlW;
class CReportWidget;
class QPushButton;
class CTaskBaseInfoW;
class CDataMgr;
class CMissionWidget : public QWidget
{
    Q_OBJECT

    enum S_MISSIONTYPE
    {
        TIME_TYPE,
        LIGHT_TYPE,

        NONE_TYPE
    };
public:
    CMissionWidget(QWidget *parent = 0);
    ~CMissionWidget();

    void setMissionType(const S_MISSIONTYPE& type) {m_sMissionType = type;}

public slots:
    void readMission();

private slots:
    void addMission();
    void showTask(int row,int column);
    void delOneRow();

private:
    void init_();
    void layout_();
    void localConnect();
    void addRow(int row);

private:
    CDataMgr*                       m_pDataMgr;

    CReportWidget*                  m_pMissionReport;
    QPushButton*                    m_pAddNewTaskBtn;
    QPushButton*                    m_pUpdateTaskBtn;
    QPushButton*                    m_pDelOneTaskBtn;

    CTaskBaseInfoW*                 m_pTaskInfoW;

    S_MISSIONTYPE                   m_sMissionType;

private:
    friend class CTimeCtrlW;
    friend class CLightCtrlW;
};

#endif // MISSIONWIDGET_H
