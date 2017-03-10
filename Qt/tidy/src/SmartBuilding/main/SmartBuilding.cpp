#include "SmartBuilding.h"

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/topBar/topBar.h"
#include "src/SmartBuilding/main/content/contentMgr.h"
#include "src/SmartBuilding/main/topBar/topBarEvent.h"

SmartBuilding::SmartBuilding(QWidget *parent, Qt::WFlags flags)
: QWidget(parent, flags)
{
    ui.setupUi(this);
    init_();
    localConnect();
}

SmartBuilding::~SmartBuilding()
{

}

bool SmartBuilding::event(QEvent *e)
{
    QEvent::Type type = e->type();
    if (type >= CCT_EVENT_TOP_BAR && type < CCT_MAX_EVENT) {
        switch(type) 
        {
        case CCT_EVENT_TOP_BAR:
            {
                CTopBarEvent* topBarEvent = dynamic_cast<CTopBarEvent*>(e);
                if (topBarEvent) {
                    changeState(topBarEvent->topBarState());
                }
            }
            break;
        case CCT_EVENT_BOTTOM_BAR:
            break;
        default:
            break;
        }
    }
    else {
        return QWidget::event(e);
    }

    return true;
}

void SmartBuilding::init_()
{
    this->setFixedSize(QSize(WIDGET_WIDTH,WIDGET_HEIGHT));
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_pTopBar = new CTopBar(this);
    m_pContentMgr = new CContentMgr(this);

    layout_();
}

void SmartBuilding::layout_()
{
    m_pTopBar->setGeometry(TOP_BAR_LEFT,TOP_BAR_TOP,
                           TOP_BAR_WIDTH,TOP_BAR_HEIGHT);

    m_pContentMgr->setGeometry(CONTENT_L,CONTENT_T,
                               CONTENT_W,CONTENT_H);
}

void SmartBuilding::localConnect()
{
    
}

void SmartBuilding::changeState(bool state)
{
    m_pTopBar->changeState(state);
}
