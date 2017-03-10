/****************************************************************************
**
** file: circuitSelection.h
**
** description:
**     ��·ѡ���ܽ���
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
     *  @Desc:      ���û�·
     *  @PaamIn:    circuitSelection ��·��ѡ��
     *  @Return:
     *      none
     */
    void setCircuitSelection(const S_WORD& circuitSelection);
    /*
     *  @Func:      getCircuitSelection
     *  @Desc:      ��ȡ��·��ѡ��
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE ��·��ѡ����
     */
    S_WORD getCircuitSelection();
    /*
     *  @Func:      getCircuitCount
     *  @Desc:      ��ȡ��·����
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE ��·�ĸ���
     */
    S_BYTE getCircuitCount();
     /*
     *  @Func:      getCircuitList
     *  @Desc:      ��ȡѡ�еĻ�·�б�
     *  @PaamOut:    circuitList
     *  @Return:
     *      S_BYTE ѡ�еĻ�·���б�
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
