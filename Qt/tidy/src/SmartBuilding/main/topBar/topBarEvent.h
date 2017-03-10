#ifndef TOPBAREVENT_H
#define TOPBAREVENT_H

#include <QEvent>

enum E_MANUAL_EVENT{
    CCT_EVENT_TOP_BAR = QEvent::User+100,       //  card status changed event
    CCT_EVENT_BOTTOM_BAR,

    CCT_MAX_EVENT
};

class CTopBarEvent : public QEvent
{
public:
    CTopBarEvent(bool state)
        : QEvent(static_cast<Type>(CCT_EVENT_TOP_BAR))
    {
        m_bState = state;
    }

    bool topBarState() {return m_bState;}

private:
    bool                    m_bState;                                   // true 自动 false 手动
};

#endif // TOPBAREVENT_H
