#include "timeCtrlWidget.h"

#include "src/SmartBuilding/inc/xtUtility.h"
#include "src/SmartBuilding/main/content/common_ctl/missionWidget.h"

CTimeCtrlW::CTimeCtrlW(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();

    xt_msleep(500);
    m_pMissionWidget->readMission();
}

CTimeCtrlW::~CTimeCtrlW()
{

}

void CTimeCtrlW::init_()
{
    m_pMissionWidget = new CMissionWidget(this);
    m_pMissionWidget->setMissionType(CMissionWidget::TIME_TYPE);
}

void CTimeCtrlW::localConnect()
{

}

