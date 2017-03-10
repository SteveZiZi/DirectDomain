/****************************************************************************
**
** file: circuitSelection.h
**
** description:
**     回路选择功能界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CIRCUITSELECTION_H
#define CIRCUITSELECTION_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

class QCheckBox;
class CCircuitSelection : public QWidget
{
    Q_OBJECT

public:
    CCircuitSelection(QWidget *parent = 0);
    ~CCircuitSelection();

     /*
     *  @Func:      setCircuitSelection
     *  @Desc:      设置回路
     *  @PaamIn:    circuitSelection 回路的选择
     *  @Return:
     *      none
     */
    void setCircuitSelection(const S_WORD& circuitSelection);
    /*
     *  @Func:      getCircuitSelection
     *  @Desc:      获取回路的选择
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE 回路的选择结果
     */
    S_WORD getCircuitSelection();
    /*
     *  @Func:      getCircuitCount
     *  @Desc:      获取回路个数
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE 回路的个数
     */
    S_BYTE getCircuitCount();
     /*
     *  @Func:      getCircuitList
     *  @Desc:      获取选中的回路列表
     *  @PaamOut:    circuitList
     *  @Return:
     *      S_BYTE 选中的回路的列表
     */
    void getCircuitList(QList<int>& circuitList);
    void setCircuitEnable(int index,bool enable);

private:
    void init_();
    void layout_();

private:
    QList<QCheckBox*>               m_lCircuitCheckList;
};

#endif // CIRCUITSELECTION_H
