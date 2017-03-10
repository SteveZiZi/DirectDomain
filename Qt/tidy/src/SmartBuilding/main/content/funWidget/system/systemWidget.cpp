#include "systemWidget.h"

#include "src/SmartBuilding/main/content/funWidget/system/baseSetWidget.h"
#include "src/SmartBuilding/main/content/funWidget/system/defendWidget.h"
#include "src/SmartBuilding/main/content/funWidget/system/pwdWidget.h"

CSystemW::CSystemW(QWidget *parent)
: QTabWidget(parent)
{
    init_();
}

CSystemW::~CSystemW()
{

}

void CSystemW::init_()
{
    m_pBaseSetW = new CBaseSetW(this);
    m_PDefendWidget = new CDefendWidget(this);
    m_pPwdWidget = new CPwdWidget(this);
    this->addTab(m_pBaseSetW,tr("基本设置"));
    this->addTab(m_PDefendWidget,tr("漏电保护"));
    this->addTab(m_pPwdWidget,tr("密码更改"));
}
