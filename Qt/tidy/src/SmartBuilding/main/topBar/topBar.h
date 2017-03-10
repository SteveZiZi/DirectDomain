/****************************************************************************
**
** file: topBar.h
**
** description:
**     ∂•≤„œ‘ æ¿∏
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef TOPBAR_H
#define TOPBAR_H

#include <QtGui/QWidget>

class QLabel;
class QTimer;
class CDataMgr;
class CTopBar : public QWidget
{
    Q_OBJECT

public:
    CTopBar(QWidget *parent = 0);
    ~CTopBar();

    void changeState(bool state);

private slots:
    void updateTime();
    void updateWarnState();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CDataMgr*           m_pDataMgr;

    QLabel*             m_pLogoLabel;
    QLabel*             m_pWarnLabel;
    QLabel*             m_pGPRSLabel;
    QLabel*             m_pWIFILabel;
    QLabel*             m_pTimeLabel;
    QLabel*             m_pStateLabel;

    QTimer*             m_pTimeTimer;
    QTimer*             m_pWarnTimer;
};

#endif // TOPBAR_H
