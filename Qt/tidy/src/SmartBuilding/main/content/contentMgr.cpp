#include "contentMgr.h"

#include <QStackedWidget>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/content/menuBar/menuBar.h"
#include "src/SmartBuilding/main/content/funWidget/monitor/monitorWidget.h"
#include "src/SmartBuilding/main/content/funWidget/timeCtrl/timeCtrlWidget.h"
#include "src/SmartBuilding/main/content/funWidget/lightCtrl/lightCtrlWidget.h"
#include "src/SmartBuilding/main/content/funWidget/constanceAs/constanceAsWidget.h"
#include "src/SmartBuilding/main/content/funWidget/system/systemWidget.h"
#include "src/SmartBuilding/main/content/funWidget/log/logWidget.h"
#include "src/SmartBuilding/main/content/funWidget/config/configWidget.h"

CContentMgr::CContentMgr(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();
}

CContentMgr::~CContentMgr()
{

}

void CContentMgr::updateFunWidget(int index)
{
    Q_ASSERT(index >=0 && index < m_pFunWidget->count());

    m_pFunWidget->setCurrentIndex(index);
}

void CContentMgr::init_()
{
    m_pFunWidget = new QStackedWidget(this);
    m_pMenuBar = new CMenuBar(this);

    m_pMonitorW = new CMonitorW(this);
    m_pTimeCtrlW = new CTimeCtrlW(this);
    m_pLightCtrlW = new CLightCtrlW(this);
    m_pConstanceAsW = new CConstanceAsW(this);
    m_pLogWidget = new CLogWidget(this);
    m_pSystemW = new CSystemW(this);
    m_pConfigW = new CConfigeW(this);

    m_pFunWidget->addWidget(m_pMonitorW);
    m_pFunWidget->addWidget(m_pTimeCtrlW);
    m_pFunWidget->addWidget(m_pLightCtrlW);
    m_pFunWidget->addWidget(m_pConstanceAsW);
    m_pFunWidget->addWidget(m_pLogWidget);
    m_pFunWidget->addWidget(m_pSystemW);
    m_pFunWidget->addWidget(m_pConfigW);

    layout_();
}

void CContentMgr::layout_()
{
    m_pMenuBar->setGeometry(MENU_BAR_LEFT,MENU_BAR_TOP,
                            MENU_BAR_WIDTH,MENU_BAR_HEIGHT);

    m_pFunWidget->setGeometry(FUN_WIDGET_LEFT,FUN_WIDGET_TOP,
                              FUN_WIDGET_WIDTH,FUN_WIDGET_HEIGHT);
}

void CContentMgr::localConnect()
{
    connect(m_pMenuBar,SIGNAL(menuSwitched(int)),this,SLOT(updateFunWidget(int)));
}
