/****************************************************************************
**
** file: weightWidget.h
**
** description:
**     Ȩ�ؽ���
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef WEIGHTWIDGET_H
#define WEIGHTWIDGET_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

class QCheckBox;
class QComboBox;
class CWeightWidget : public QWidget
{
    Q_OBJECT

public:
    CWeightWidget(QWidget *parent = 0);
    ~CWeightWidget();

    /*
     *  @Func:      setSensor
     *  @Desc:      ���ô�����ѡ��
     *  @PaamIn:    sensorSelection ��������ѡ��
     *  @Return:
     *      none
     */
    void setSensor(const S_DWORD& sensorSelection);
    /*
     *  @Func:      getSensor
     *  @Desc:      ��ȡ��������ѡ��
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE ��������ѡ����
     */
    S_DWORD getSensor();
    /*
     *  @Func:      setWeight
     *  @Desc:      ���ô�������Ȩ��
     *  @PaamIn:    weight:Ȩ�� bufLen:����������
     *  @Return:
     *      true �ɹ� false ʧ��
     */
    bool setWeight(INT8U* weight,int bufLen);
    /*
     *  @Func:      getWeight
     *  @Desc:      ��ȡ��������Ȩ��
     *  @PaamIn:    bufLen:����������
     *  @PaamOut:   weight:Ȩ��
     *  @Return:
     *      true �ɹ� false ʧ��
     */
    bool getWeight(INT8U* weight,int bufLen);

    void updateSensor();
private:
    void init_();
    void layout_();
    void resetWeight();

private:
    QList<QCheckBox*>               m_lSensorCheckList;
    QList<QComboBox*>               m_lWeightComList;
    QList<int>                      m_lWeight;
};

#endif // WEIGHTWIDGET_H
