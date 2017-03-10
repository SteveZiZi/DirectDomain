#include "configWidget.h"

#include "src/SmartBuilding/main/content/funWidget/config/dimTableWidget.h"
#include "src/SmartBuilding/main/content/funWidget/config/disposeWidget.h"

CConfigeW::CConfigeW(QWidget *parent)
: QTabWidget(parent)
{
    init_();
}

CConfigeW::~CConfigeW()
{

}

void CConfigeW::init_()
{
    m_pDimTableWidget = new CDimTableWidget(this);
    m_pDisposeWidget = new CDisposeWidget(this);
    this->addTab(m_pDimTableWidget,tr("Dim±í"));
    this->addTab(m_pDisposeWidget,tr("²¿Êð"));
}
