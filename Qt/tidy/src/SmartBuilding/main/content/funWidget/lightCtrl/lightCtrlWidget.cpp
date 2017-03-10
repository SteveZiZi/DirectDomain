#include "lightCtrlWidget.h"

#include <QPushButton>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/inc/xtUtility.h"
#include "src/SmartBuilding/main/content/common_ctl/missionWidget.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/lightTaskInfoW.h"

CLightCtrlW::CLightCtrlW(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();

    xt_msleep(500);
    m_pMissionWidget->readMission();
}

CLightCtrlW::~CLightCtrlW()
{

}

void CLightCtrlW::init_()
{
    m_pMissionWidget = new CMissionWidget(this);
    m_pMissionWidget->setMissionType(CMissionWidget::LIGHT_TYPE);
}

void CLightCtrlW::localConnect()
{

}
