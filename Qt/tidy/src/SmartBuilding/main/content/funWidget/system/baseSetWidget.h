/****************************************************************************
**
** file: baseSetWidget.h
**
** description:
**     基础设置界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef BASESETWIDGET_H
#define BASESETWIDGET_H

#include <QtGui/QWidget>

class QPushButton;
class QLineEdit;
class QDateTimeEdit;
class QFrame;
class QLabel;
class CDataMgr;
class CBaseSetW : public QWidget
{
    Q_OBJECT

public:
    CBaseSetW(QWidget *parent = 0);
    ~CBaseSetW();

private slots:
    void timing();
    void getTelNum();
    void setTelNum();
    void readInfo();
    void saveInfo();
    void clearTotalData();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CDataMgr*                   m_pDataMgr;

    QFrame*                     m_pFrame;

    QDateTimeEdit*              m_pDateTime;

    QPushButton*                m_pTimingBtn;
    QPushButton*                m_pTelNumBtn;
    QPushButton*                m_pSetTelNumBtn;
    QPushButton*                m_pClearTotalBtn;
    QPushButton*                m_pReadBtn;
    QPushButton*                m_pSaveBtn;

    QLineEdit*                  m_pTelNumEdit;
    QLineEdit*                  m_pIpAddEdit;
    QLineEdit*                  m_pMaskAddEdit;
    QLineEdit*                  m_pGateWayEdit;
    QLineEdit*                  m_pServerAddEdit;
    QLineEdit*                  m_pPortEdit;

    QLabel*                     m_pLogicAddLabel;
    QLabel*                     m_pSoftVerLabel;
};

#endif // BASESETWIDGET_H
