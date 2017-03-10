#include "topBar.h"

#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QState>
#include <QStateMachine>
#include <QHBoxLayout>

#include "src/SmartBuilding/dataLevel/dataMgr.h"

#define UPDATE_TIME             500
#define MAX_CIRCUIT_COUNT       16

CTopBar::CTopBar(QWidget *parent)
: QWidget(parent)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
}

CTopBar::~CTopBar()
{

}

void CTopBar::changeState(bool state)
{
    if (state)
        m_pStateLabel->setText(tr("自动"));
    else
        m_pStateLabel->setText(tr("手动"));
}

void CTopBar::updateTime()
{
    QDateTime sysTime = QDateTime::currentDateTime();
    m_pTimeLabel->setText(sysTime.toString("yyyy-MM-dd hh:mm:ss"));
}

void CTopBar::updateWarnState()
{
    for(int i = 0;i < MAX_CIRCUIT_COUNT;i++)
    {
        if(m_pDataMgr->getCircuitWarnState(i + 1)){
            m_pWarnLabel->show();
            break;
        }
        else{
            m_pWarnLabel->hide();
        }
    }
}

void CTopBar::init_()
{
    m_pTimeLabel = new QLabel;
    m_pGPRSLabel = new QLabel("GPRS");
    m_pGPRSLabel->setObjectName("gprsLabel");
    m_pStateLabel = new QLabel(tr("自动"));
    m_pLogoLabel = new QLabel;
    m_pLogoLabel->setObjectName("logoLabel");
    m_pWIFILabel = new QLabel;
    m_pWIFILabel->setObjectName("wifiLabel");
    m_pWarnLabel = new QLabel;
    m_pWarnLabel->setObjectName("warnLabel");
    m_pWarnLabel->hide();

    m_pTimeTimer = new QTimer(this);
    m_pTimeTimer->start(UPDATE_TIME);
    m_pWarnTimer = new QTimer(this);
    m_pWarnTimer->start(UPDATE_TIME);

    layout_();
}

void CTopBar::layout_()
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_pLogoLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(m_pWarnLabel);
    mainLayout->addWidget(m_pStateLabel);
    mainLayout->addWidget(m_pGPRSLabel);
    mainLayout->addWidget(m_pWIFILabel);
    mainLayout->addWidget(m_pTimeLabel);
    mainLayout->addSpacing(10);
    setLayout(mainLayout);
}

void CTopBar::localConnect()
{
    connect(m_pTimeTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    connect(m_pWarnTimer,SIGNAL(timeout()),this,SLOT(updateWarnState()));
}