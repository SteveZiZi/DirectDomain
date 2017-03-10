#include "platForm.h"

#include "src/vKeyBoard/characterPad.h"
#include "src/vKeyBoard/numPad.h"
#include "src/vKeyBoard/vKeyBoard.h"

#define KEYBOARD_WIDGET_W       800
#define KEYBOARD_WIDGET_H       200

CPlatForm::CPlatForm(vKeyBoard* keyBoard,QWidget *parent)
: QWidget(parent,Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
, m_pKeyBoard(keyBoard)
{
    init_();
    localConnect();
}

CPlatForm::~CPlatForm()
{

}

bool CPlatForm::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::WindowActivate:
        if(m_pLastFocusedWidget)
            m_pLastFocusedWidget->activateWindow();
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

void CPlatForm::changeInput(int type)
{
    switch(type)
    {
    case INPUT_CHARACTER:
        m_pNumPad->hide();
        m_pCharacterPad->show();
        break;
    case INPUT_NUMBER:
        m_pCharacterPad->hide();
        m_pNumPad->show();
        break;
    default:
        break;
    }
}

void CPlatForm::saveFocusWidget(QWidget *oldFocus,QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus))
        m_pLastFocusedWidget = newFocus;
}

void CPlatForm::init_()
{
    this->resize(KEYBOARD_WIDGET_W,KEYBOARD_WIDGET_H);
    m_pCharacterPad = new CCharacterPad(this);
    m_pNumPad = new CNumPad(this);
    changeInput(INPUT_NUMBER);
}

void CPlatForm::localConnect()
{
    connect(qApp,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(saveFocusWidget(QWidget*,QWidget*)));
    connect(m_pCharacterPad,SIGNAL(charSend(const QChar&)),m_pKeyBoard,SLOT(sendChar(const QChar&)));
    connect(m_pCharacterPad,SIGNAL(keyValSend(int)),m_pKeyBoard,SLOT(sendKeyVal(int)));
    connect(m_pCharacterPad,SIGNAL(inputChanged(int)),this,SLOT(changeInput(int)));
    connect(m_pCharacterPad,SIGNAL(commited(bool)),m_pKeyBoard,SLOT(commit(bool)));
    connect(m_pNumPad,SIGNAL(charSend(const QChar&)),m_pKeyBoard,SLOT(sendChar(const QChar&)));
    connect(m_pNumPad,SIGNAL(keyValSend(int)),m_pKeyBoard,SLOT(sendKeyVal(int)));
    connect(m_pNumPad,SIGNAL(inputChanged(int)),this,SLOT(changeInput(int)));
    connect(m_pNumPad,SIGNAL(commited(bool)),m_pKeyBoard,SLOT(commit(bool)));
}
