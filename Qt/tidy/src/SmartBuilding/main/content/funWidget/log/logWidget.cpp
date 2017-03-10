#include "logWidget.h"

#include "src/SmartBuilding/main/content/funWidget/log/blackBoxWidget.h"
#include "src/SmartBuilding/main/content/funWidget/log/recordWidget.h"

CLogWidget::CLogWidget(QWidget *parent)
: QTabWidget(parent)
{
    init_();
    localConnect();
}

CLogWidget::~CLogWidget()
{

}

void CLogWidget::init_()
{
    m_pBlackBoxW = new CBlackBoxW(this);
    m_pRecordWidget = new CRecordWidget(this);

    this->addTab(m_pRecordWidget,tr("хуж╬"));
    this->addTab(m_pBlackBoxW,tr("╨зо╩вс"));
}

void CLogWidget::localConnect()
{

}

