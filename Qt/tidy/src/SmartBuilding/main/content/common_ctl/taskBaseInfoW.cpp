#include "taskBaseInfoW.h"

#include <QLabel>
#include <QDateTime>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QPushButton>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/content/common_ctl/weekSelection.h"

CTaskBaseInfoW::CTaskBaseInfoW(QWidget *parent,bool exit)
: QWidget(parent)
, m_bExit(exit)
{
    init_();
    localConnect();
}

CTaskBaseInfoW::~CTaskBaseInfoW()
{

}

void CTaskBaseInfoW::init_()
{
    m_pFrame = new QFrame(this);
    m_pEnableCheck = new QCheckBox(tr("Ê¹ÄÜ"),m_pFrame);
    m_pIDCombo = new QComboBox(m_pFrame);
    m_pStartTime = new QDateTimeEdit(QDateTime::currentDateTime(),m_pFrame);
    m_pStartTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    m_pEndTime = new QDateTimeEdit(QDateTime::currentDateTime(),m_pFrame);
    m_pEndTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    m_pWeekSelection = new CWeekSelection(m_pFrame);
    m_pSaveBtn = new QPushButton(tr("±£´æ"),this);
}

void CTaskBaseInfoW::localConnect()
{

}
