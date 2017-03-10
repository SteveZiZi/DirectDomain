/****************************************************************************
**
** file: weightWidget.h
**
** description:
**     权重界面
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
     *  @Desc:      设置传感器选择
     *  @PaamIn:    sensorSelection 传感器的选择
     *  @Return:
     *      none
     */
    void setSensor(const S_DWORD& sensorSelection);
    /*
     *  @Func:      getSensor
     *  @Desc:      获取传感器的选择
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE 传感器的选择结果
     */
    S_DWORD getSensor();
    /*
     *  @Func:      setWeight
     *  @Desc:      设置传感器的权重
     *  @PaamIn:    weight:权重 bufLen:缓存区长度
     *  @Return:
     *      true 成功 false 失败
     */
    bool setWeight(INT8U* weight,int bufLen);
    /*
     *  @Func:      getWeight
     *  @Desc:      获取传感器的权重
     *  @PaamIn:    bufLen:缓存区长度
     *  @PaamOut:   weight:权重
     *  @Return:
     *      true 成功 false 失败
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
