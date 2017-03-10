#include "recordWidget.h"

#include "src/SmartBuilding/main/content/common_ctl/logReportWidget.h"

CRecordWidget::CRecordWidget(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();
}

CRecordWidget::~CRecordWidget()
{

}

void CRecordWidget::init_()
{
    m_pLogReportW = new CLogReportW(CLogReportW::RECORD_TYPE,this);
}

void CRecordWidget::layout_()
{

}

void CRecordWidget::localConnect()
{

}

