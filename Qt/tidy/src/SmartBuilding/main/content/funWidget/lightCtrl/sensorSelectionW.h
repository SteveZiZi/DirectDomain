/****************************************************************************
**
** file: sensorSelectionW.h
**
** description:
**     光控传感器选择
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef SENSORSELECTIONW_H
#define SENSORSELECTIONW_H

#include <QtGui/QWidget>
#include "src/SmartBuilding/inc/xtDef.h"

class QCheckBox;
class CSensorSelectionW : public QWidget
{
    Q_OBJECT

public:
    CSensorSelectionW(QWidget *parent = 0);
    ~CSensorSelectionW();

    /*
     *  @Func:      setSensorSelection
     *  @Desc:      设置传感器选择
     *  @PaamIn:    sensorSelection 传感器的选择
     *  @Return:
     *      none
     */
    void setSensorSelection(const S_DWORD& sensorSelection);
    /*
     *  @Func:      getSensorSelection
     *  @Desc:      获取传感器的选择
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE 传感器的选择结果
     */
    S_DWORD getSensorSelection();

    void updateSensor();

private:
    void init_();
    void layout_();

private:
    QList<QCheckBox*>               m_lSensorList;
};

#endif // SENSORSELECTIONW_H
