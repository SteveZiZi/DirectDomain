/****************************************************************************
**
** file: sensorSelectionW.h
**
** description:
**     ��ش�����ѡ��
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
     *  @Desc:      ���ô�����ѡ��
     *  @PaamIn:    sensorSelection ��������ѡ��
     *  @Return:
     *      none
     */
    void setSensorSelection(const S_DWORD& sensorSelection);
    /*
     *  @Func:      getSensorSelection
     *  @Desc:      ��ȡ��������ѡ��
     *  @PaamIn:    none
     *  @Return:
     *      S_BYTE ��������ѡ����
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
