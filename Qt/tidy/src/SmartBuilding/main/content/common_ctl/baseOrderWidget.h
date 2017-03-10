/****************************************************************************
**
** file: baseOrderWidget.h
**
** description:
**     动作指令基类，可被光控指令和时控指令继承
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef BASEORDERWIDGET_H
#define BASEORDERWIDGET_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

#define WIDGET_HEIGHT_H         30
#define WIDGET_WIDTH_W          120

class QPushButton;
class QComboBox;
class CBaseOrderWidget : public QWidget
{
    Q_OBJECT

public:
    CBaseOrderWidget(QWidget *parent = 0);
    virtual ~CBaseOrderWidget();

    S_BYTE orderIndex();
    void setOrder(S_BYTE orderIndex_);

signals:
    void orderDeled();

protected:
    virtual void init_();

private slots:
    void delSelf();

private:
    void localConnect();

protected:
    QPushButton*                m_pDelBtn;
    QComboBox*                  m_pOrderCom;
};

#endif // BASEORDERWIDGET_H
