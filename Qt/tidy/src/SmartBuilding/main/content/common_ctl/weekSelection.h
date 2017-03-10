/****************************************************************************
**
** file: weekSelection.h
**
** description:
**     ����ѡ��ʽ.
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef WEEKSELECTION_H
#define WEEKSELECTION_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

class QCheckBox;
class CWeekSelection : public QWidget
{
    Q_OBJECT

public:
    CWeekSelection(QWidget *parent = 0);
    ~CWeekSelection();

    /*
     *  @Func:      setWeekSelection
     *  @Desc:      ��������ѡ��
     *  @PaamIn:    selection ���ڵ�ѡ��
     *  @Return:
     *      none
     */
    void setWeekSelection(S_BYTE selection);
    /*
     *  @Func:      getWeekSelection
     *  @Desc:      ��ȡ���ڵ�ѡ����
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE ���ڵ�ѡ����
     */
    S_BYTE getWeekSelection();

private:
    void init_();
    void layout_();

private:
    QCheckBox*                  m_pMonCheck;
    QCheckBox*                  m_pTuesCheck;
    QCheckBox*                  m_pWedCheck;
    QCheckBox*                  m_pThurCheck;
    QCheckBox*                  m_pFirCheck;
    QCheckBox*                  m_pSatCheck;
    QCheckBox*                  m_pSunCheck;
};

#endif // WEEKSELECTION_H
