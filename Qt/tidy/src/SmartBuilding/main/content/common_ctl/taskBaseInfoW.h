/****************************************************************************
**
** file: taskBaseInfo.h
**
** description:
**     ������࣬����࣬ʱ�����񡢹�������������̳д˻���.
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef TASKBASEINFOW_H
#define TASKBASEINFOW_H

#include <QtGui/QWidget>

class QFrame;
class QCheckBox;
class QComboBox;
class QPushButton;
class QDateTimeEdit;
class CWeekSelection;
class CTaskBaseInfoW : public QWidget
{
    Q_OBJECT

signals:
    void taskUpdate();
    void taskShow();

public:
    CTaskBaseInfoW(QWidget *parent = 0,bool exit = false);
    virtual ~CTaskBaseInfoW();

    bool isExit() {return m_bExit;}
    void setExit(bool exit) {m_bExit = exit;}

protected:
    virtual void init_();
    virtual void layout_() = 0;
    virtual void localConnect();

protected:
    bool                        m_bExit;                                // �ж������Ƿ��Ѿ�����
    QFrame*                     m_pFrame;

    QCheckBox*                  m_pEnableCheck;

    QComboBox*                  m_pIDCombo;

    QPushButton*                m_pSaveBtn;

    QDateTimeEdit*              m_pStartTime;
    QDateTimeEdit*              m_pEndTime;

    CWeekSelection*             m_pWeekSelection;
};

#endif // TASKBASEINFOW_H
