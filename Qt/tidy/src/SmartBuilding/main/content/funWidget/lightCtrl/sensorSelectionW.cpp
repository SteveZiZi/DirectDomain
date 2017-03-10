#include "sensorSelectionW.h"

#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>

#include "src/SmartBuilding/dataLevel/dataMgr.h"

#define SENSOR_MAX_COUNT            16
#define WIDGET_HEIGHT_H             30

CSensorSelectionW::CSensorSelectionW(QWidget *parent)
: QWidget(parent)
{
    init_();
}

CSensorSelectionW::~CSensorSelectionW()
{

}

void CSensorSelectionW::setSensorSelection(const S_DWORD& sensorSelection)
{
    for(int i = 0;i < SENSOR_MAX_COUNT;i++)
    {
        if((1 << (i)) & sensorSelection)
            m_lSensorList[i]->setChecked(true);
    }
}

S_DWORD CSensorSelectionW::getSensorSelection()
{
    S_DWORD sensorSelection = 0;
    for(int i = 0; i < SENSOR_MAX_COUNT;i++)
    {
        if(m_lSensorList[i]->isChecked())
            sensorSelection |= 1 << (i);
    }

    return sensorSelection;
}

void CSensorSelectionW::updateSensor()
{
    for(int i = 97;i < 161;i++)
    {
        if (CDataMgr::getInstance()->enableSensor(i))
        {
            m_lSensorList[i - 97]->setEnabled(true);
        }
    }
}

void CSensorSelectionW::init_()
{
    for (int i = 0;i < SENSOR_MAX_COUNT;i++)
    {
        QCheckBox* sensorCheck = new QCheckBox;
        if(i < 9)
            sensorCheck->setText(QString("#A000%1").arg(QString::number(i + 1)));
        else
            sensorCheck->setText(QString("#A00%1").arg(QString::number(i + 1)));
        sensorCheck->setFixedHeight(WIDGET_HEIGHT_H);
        sensorCheck->setEnabled(false);
        m_lSensorList.append(sensorCheck);
    }

    layout_();
}

void CSensorSelectionW::layout_()
{
    QGridLayout* gridLayout = new QGridLayout;
    for (int row = 0;row < (SENSOR_MAX_COUNT / 4);row++)
    {
        for (int column = 0;column < (SENSOR_MAX_COUNT / 4);column++)
        {
            gridLayout->addWidget(m_lSensorList[row * 4 +column],row,column);
        }
    }

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(new QLabel(tr("¹âÕÕ´«¸ÐÆ÷ :")));
    mainLayout->addLayout(gridLayout);
    this->setLayout(mainLayout);
}
