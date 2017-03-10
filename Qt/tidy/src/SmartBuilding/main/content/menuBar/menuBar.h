/****************************************************************************
**
** file: menuBar.h
**
** description:
**     ²Ëµ¥À¸
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef MENUBAR_H
#define MENUBAR_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

class QToolButton;
class CMenuBar : public QWidget
{
    Q_OBJECT

public:
    CMenuBar(QWidget *parent = 0);
    ~CMenuBar();

signals:
    void menuSwitched(int);

private slots:
    void updateMenus();

private:
    void init_();
    void layout_();
    void localConnect();
    int findBtn(QToolButton* btn);

private:
    QList<QToolButton*>             m_lMenuBtn;
};

#endif // MENUBAR_H
