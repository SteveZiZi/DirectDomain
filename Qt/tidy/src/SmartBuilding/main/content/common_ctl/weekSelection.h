/****************************************************************************
**
** file: weekSelection.h
**
** description:
**     星期选择方式.
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
     *  @Desc:      设置星期选择
     *  @PaamIn:    selection 星期的选择
     *  @Return:
     *      none
     */
    void setWeekSelection(S_BYTE selection);
    /*
     *  @Func:      getWeekSelection
     *  @Desc:      获取星期的选择结果
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE 星期的选择结果
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
