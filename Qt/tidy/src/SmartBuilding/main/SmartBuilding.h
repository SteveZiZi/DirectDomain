/****************************************************************************
**
** file: SmartBuilding.h
**
** description:
**     Ö÷½çÃæ
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef SMARTBUILDING_H
#define SMARTBUILDING_H

#include <QtGui/QWidget>
#include "ui_SmartBuilding.h"

class CTopBar;
class CContentMgr;
class SmartBuilding : public QWidget
{
    Q_OBJECT

public:
    SmartBuilding(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~SmartBuilding();

protected:
    bool event(QEvent *);

private:
    void init_();
    void layout_();
    void localConnect();
    void changeState(bool state);

private:
    Ui::SmartBuildingClass ui;

    CTopBar*                m_pTopBar;
    CContentMgr*            m_pContentMgr;
};

#endif // SMARTBUILDING_H
