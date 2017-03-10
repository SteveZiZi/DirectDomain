/****************************************************************************
**
** file: constanceAsWidget.h
**
** description:
**     ∫„’’øÿ÷∆ΩÁ√Ê
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CONSTANCEASWIDGET_H
#define CONSTANCEASWIDGET_H

#include "src/SmartBuilding/main/content/common_ctl/taskBaseInfoW.h"

class QLineEdit;
class CExecTimeWidget;
class CWeightWidget;
class CDataMgr;
class CConstanceAsW : public CTaskBaseInfoW
{
    Q_OBJECT

public:
    CConstanceAsW(QWidget *parent = 0);
    ~CConstanceAsW();

protected:
    virtual void init_();
    virtual void layout_();
    virtual void localConnect();

private slots:
    void flushTask();
    void saveTask();
    bool checkCircuit(int index);

private:
    CDataMgr*                   m_pDataMgr;

    QLineEdit*                  m_pLuxValueEdit;
    QComboBox*                  m_pCircuitCom;
    QPushButton*                m_pFlushBtn;
    CExecTimeWidget*            m_pExecTimeW;
    CWeightWidget*              m_pWeightWidget;
};

#endif // CONSTANCEASWIDGET_H
