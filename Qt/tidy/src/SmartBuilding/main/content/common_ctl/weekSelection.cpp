#include "weekSelection.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#define CHECK_BOX_H             30

enum WEEKDAY
{
    SUNDAY = 0x01,
    MONDAY = 0x02,
    TUESDAY = 0x04,
    WEDNESDAY = 0x08,
    THURSDAY = 0x10,
    FIRDAY = 0x20,
    SATURDAY = 0x40
};

CWeekSelection::CWeekSelection(QWidget *parent)
: QWidget(parent)
{
    init_();
}

CWeekSelection::~CWeekSelection()
{

}

void CWeekSelection::setWeekSelection(S_BYTE selection)
{
    if(selection & SUNDAY)
        m_pSunCheck->setChecked(true);
    if(selection & MONDAY)
        m_pMonCheck->setChecked(true);
    if(selection & TUESDAY)
        m_pTuesCheck->setChecked(true);
    if(selection & WEDNESDAY)
        m_pWedCheck->setChecked(true);
    if(selection & THURSDAY)
        m_pThurCheck->setChecked(true);
    if(selection & FIRDAY)
        m_pFirCheck->setChecked(true);
    if(selection & SATURDAY)
        m_pSatCheck->setChecked(true);
}

S_BYTE CWeekSelection::getWeekSelection()
{
    S_BYTE weekSelection = 0;
    if(m_pSunCheck->isChecked())
        weekSelection |= SUNDAY;
    if(m_pMonCheck->isChecked())
        weekSelection |= MONDAY;
    if(m_pTuesCheck->isChecked())
        weekSelection |= TUESDAY;
    if(m_pWedCheck->isChecked())
        weekSelection |= WEDNESDAY;
    if(m_pThurCheck->isChecked())
        weekSelection |= THURSDAY;
    if(m_pFirCheck->isChecked())
        weekSelection |= FIRDAY;
    if(m_pSatCheck->isChecked())
        weekSelection |= SATURDAY;
    return weekSelection;
}

void CWeekSelection::init_()
{
    m_pMonCheck = new QCheckBox(tr("周一  "));
    m_pTuesCheck = new QCheckBox(tr("周二  "));
    m_pWedCheck = new QCheckBox(tr("周三  "));
    m_pThurCheck = new QCheckBox(tr("周四"));
    m_pFirCheck = new QCheckBox(tr("周五"));
    m_pSatCheck = new QCheckBox(tr("周六"));
    m_pSunCheck = new QCheckBox(tr("周日"));

    m_pMonCheck->setFixedHeight(CHECK_BOX_H);
    m_pTuesCheck->setFixedHeight(CHECK_BOX_H);
    m_pWedCheck->setFixedHeight(CHECK_BOX_H);
    m_pThurCheck->setFixedHeight(CHECK_BOX_H);
    m_pFirCheck->setFixedHeight(CHECK_BOX_H);
    m_pSatCheck->setFixedHeight(CHECK_BOX_H);
    m_pSunCheck->setFixedHeight(CHECK_BOX_H);

    layout_();
}

void CWeekSelection::layout_()
{
    QHBoxLayout* week1To3Layout = new QHBoxLayout;
    week1To3Layout->addWidget(m_pMonCheck);
    week1To3Layout->addWidget(m_pTuesCheck);
    week1To3Layout->addWidget(m_pWedCheck);
    week1To3Layout->addStretch();

    QHBoxLayout* week4To7Layout = new QHBoxLayout;
    week4To7Layout->addWidget(m_pThurCheck);
    week4To7Layout->addWidget(m_pFirCheck);
    week4To7Layout->addWidget(m_pSatCheck);
    week4To7Layout->addWidget(m_pSunCheck);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(week1To3Layout);
    mainLayout->addLayout(week4To7Layout);
    this->setLayout(mainLayout);
}

