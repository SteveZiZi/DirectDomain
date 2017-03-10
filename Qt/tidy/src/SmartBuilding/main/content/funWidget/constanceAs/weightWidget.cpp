#include "weightWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "src/SmartBuilding/dataLevel/dataMgr.h"

#define SENSOR_MAX_COUNT            16
#define WIDGET_HEIGHT_H             30
#define COMBOBOX_W                  95

CWeightWidget::CWeightWidget(QWidget *parent)
: QWidget(parent)
{
    init_();
    resetWeight();
}

CWeightWidget::~CWeightWidget()
{

}

void CWeightWidget::setSensor(const S_DWORD& sensorSelection)
{
    for(int i = 0;i < SENSOR_MAX_COUNT;i++)
    {
        if((1 << i) & sensorSelection)
            m_lSensorCheckList[i]->setChecked(true);
    }
}

S_DWORD CWeightWidget::getSensor()
{
    S_DWORD sensorSelection = 0;
    for(int i = 0; i < SENSOR_MAX_COUNT;i++)
    {
        if(m_lSensorCheckList[i]->isChecked())
            sensorSelection |= 1 << i;
    }

    return sensorSelection;
}

bool CWeightWidget::setWeight(INT8U* weight,int bufLen)
{
    if (bufLen != SENSOR_MAX_COUNT)
        return false;

    for (int i = 0;i < SENSOR_MAX_COUNT;i++)
    {
        m_lWeight[i] = weight[i];
        m_lWeightComList[i]->setCurrentIndex(10 - m_lWeight[i] / 10);
    }

    return true;
}

bool CWeightWidget::getWeight(INT8U* weight,int bufLen)
{
    if (bufLen != SENSOR_MAX_COUNT)
        return false;

    for (int i = 0;i < SENSOR_MAX_COUNT;i++)
    {
        m_lWeight[i] = 10 * (10 - m_lWeightComList[i]->currentIndex());
        weight[i] = m_lWeight[i];
    }
    return true;
}

void CWeightWidget::updateSensor()
{
    for(int i = 97;i < 161;i++)
    {
        if (CDataMgr::getInstance()->enableSensor(i))
        {
            m_lSensorCheckList[i - 97]->setEnabled(true);
            m_lWeightComList[i - 97]->setEnabled(true);
        }
    }
}

void CWeightWidget::init_()
{
    for (int i = 0;i < SENSOR_MAX_COUNT;i++)
    {
        int weightPrecnt = 0;
        QCheckBox* sensorCheck = new QCheckBox;
        if(i < 9)
            sensorCheck->setText(QString("#A000%1").arg(QString::number(i + 1)));
        else
            sensorCheck->setText(QString("#A00%1").arg(QString::number(i + 1)));
        sensorCheck->setFixedHeight(WIDGET_HEIGHT_H);

        QComboBox* weightCom = new QComboBox;
        weightCom->setFixedSize(COMBOBOX_W,WIDGET_HEIGHT_H);
        QStringList weight;
        weight << "100%" << "90%" << "80%" << "70%" << "60%"
               << "50%" << "40%" << "30%" << "20%" << "10%" << "0%";
        weightCom->addItems(weight);

        m_lSensorCheckList.append(sensorCheck);
        m_lWeightComList.append(weightCom);
        m_lWeight.append(weightPrecnt);
    }

    layout_();
}

void CWeightWidget::layout_()
{
    QVBoxLayout* leftSensorLayout = new QVBoxLayout;
    QVBoxLayout* leftWeightLayout = new QVBoxLayout;
    QVBoxLayout* rightSensorLayout = new QVBoxLayout;
    QVBoxLayout* rightWeightLayout = new QVBoxLayout;
    for (int row = 0;row < (SENSOR_MAX_COUNT / 2);row++)
    {
        leftSensorLayout->addWidget(m_lSensorCheckList[2 * row]);
        leftWeightLayout->addWidget(m_lWeightComList[2 * row]);
        rightSensorLayout->addWidget(m_lSensorCheckList[2 * row + 1]);
        rightWeightLayout->addWidget(m_lWeightComList[2 * row + 1]);
    }

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftSensorLayout);
    mainLayout->addLayout(leftWeightLayout);
    mainLayout->addLayout(rightSensorLayout);
    mainLayout->addLayout(rightWeightLayout);

    this->setLayout(mainLayout);
}

void CWeightWidget::resetWeight()
{
    for (int i = 0;i < SENSOR_MAX_COUNT;i++)
    {
        m_lWeightComList[i]->setCurrentIndex(10);
        m_lWeightComList[i]->setEnabled(false);
        m_lSensorCheckList[i]->setEnabled(false);
    }
}
