#include "vKeyBoard.h"

#include <QPointer>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>

#include "src/vKeyBoard/platForm.h"

vKeyBoard::vKeyBoard(QObject *parent)
: QInputContext(parent)
{
    init_();
}

vKeyBoard::~vKeyBoard()
{
    delete m_pPlatForm;
}

void vKeyBoard::sendChar(const QChar& ch)
{
    QWidget* widget = m_pPlatForm->getFocusWidget();
    if(!widget)
        return;

    QKeyEvent keyPress(QEvent::KeyPress,ch.unicode(),Qt::NoModifier,QString(ch));
    QApplication::sendEvent(widget,&keyPress);

    QKeyEvent keyRelease(QEvent::KeyPress,ch.unicode(),Qt::NoModifier,QString());
    QApplication::sendEvent(widget,&keyRelease);
}

void vKeyBoard::sendKeyVal(int val)
{
    QPointer<QWidget> widget = focusWidget();
    if(!widget)
        return;

    QKeyEvent keyPress(QEvent::KeyPress,val,Qt::NoModifier,QString());
    QApplication::sendEvent(widget,&keyPress);
}

void vKeyBoard::commit(bool result)
{
    m_pPlatForm->hide();
}

QString vKeyBoard::identifierName()
{
    return "vKeyBoard";
}

QString vKeyBoard::language()
{
    return "en_US";
}

void vKeyBoard::reset()
{

}

bool vKeyBoard::isComposing() const
{
    return true;
}

bool vKeyBoard::filterEvent(const QEvent *event)
{
    if(event->type() == QEvent::RequestSoftwareInputPanel)
    {
        updatePosition();
        m_pPlatForm->show();
        return true;
    }
    else if(event->type() == QEvent::CloseSoftwareInputPanel)
    {
        m_pPlatForm->hide();
        return true;
    }

    return false;
}

void vKeyBoard::init_()
{
    m_pPlatForm = new CPlatForm(this);
}

void vKeyBoard::updatePosition()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();


    int y = screenRect.height() - m_pPlatForm->height();
    int x = (screenRect.width()- m_pPlatForm->width())/2;

    QPoint topP = m_pPlatForm->getFocusWidget()->mapToGlobal(QPoint(0, 0));
    if (topP.y()>= y) {
        y = 0;
    }

    m_pPlatForm->move(x,y);
}
