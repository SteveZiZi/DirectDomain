#include "numPad.h"

#include <QSignalMapper>
#include <QPushButton>
#include <QKeyEvent>
#include <QTimer>

enum {
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,

    KEY_POINT,
    KEY_COMMA,
    KEY_ADD,
    KEY_SUB,
    KEY_MULTIPLY,
    KEY_DIVIDE,
    KEY_EQUAL,
    KEY_PERCENT,
    KEY_AT,
    KEY_BACKSPACE,

    KEY_COLON,
    KEY_SEMICIOLON,
    KEY_EXCLAMATION,
    KEY_QUERY,
    KEY_LEFTBRACKET,
    KEY_RIGHTBRACKET,
    KEY_LESS,
    KEY_MORE,
    KEY_CANCEL,

    KEY_CHAR_PAD,
    KEY_SPACE,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER
};

struct KeyPadInfo
{
    int item;
    int key;
    QChar caption;
};

const KeyPadInfo S_KeyPadInfo[CNumPad::KEY_NUM] =
{
    { 0, Qt::Key_1, '1'},
    { 1, Qt::Key_2, '2'},
    { 2, Qt::Key_3, '3'},
    { 3, Qt::Key_4, '4'},
    { 4, Qt::Key_5, '5'},
    { 5, Qt::Key_6, '6'},
    { 6, Qt::Key_7, '7'},
    { 7, Qt::Key_8, '8'},
    { 8, Qt::Key_9, '9'},
    { 9, Qt::Key_0, '0'},

    { 10, Qt::Key_NumberSign, '.'},
    { 11, Qt::Key_Comma, ','},
    { 12, Qt::Key_Plus, '+'},
    { 13, Qt::Key_Minus, '-'},
    { 14, Qt::Key_Asterisk, '*'},
    { 15, Qt::Key_unknown, '/'},
    { 16, Qt::Key_Equal, '='},
    { 17, Qt::Key_unknown, '%'},
    { 18, Qt::Key_At, '@'},
    { 19, Qt::Key_Backspace, 0},

    { 20, Qt::Key_Colon, ':'},
    { 21, Qt::Key_Semicolon, ';'},
    { 22, Qt::Key_unknown, '!'},
    { 23, Qt::Key_unknown, '?'},
    { 24, Qt::Key_unknown, '('},
    { 25, Qt::Key_unknown, ')'},
    { 26, Qt::Key_Less, '<'},
    { 27, Qt::Key_Greater, '>'},
    { 28, Qt::Key_Cancel, 0},

    { 29, 0, 0},
    { 30, Qt::Key_Space, ' '},
    { 31, Qt::Key_Up, 0},
    { 32, Qt::Key_Down, 0},
    { 33, Qt::Key_Left, 0},
    { 34, Qt::Key_Right, 0 },
    { 35, Qt::Key_Enter, 0 },
};


CNumPad::CNumPad(QWidget *parent)
: QWidget(parent)
{
    ui.setupUi(this);
    init_();
    localConnect();
}

CNumPad::~CNumPad()
{

}

void CNumPad::resetWidget(void)
{
    m_iFocusItem = KEY_ENTER;
    m_lBtn[m_iFocusItem]->setFocus();
}

void CNumPad::clickButton(int index)
{
    Q_ASSERT(index < KEY_NUM);

    if (S_KeyPadInfo[index].caption != 0) {
        if (index != KEY_SPACE)
            emit charSend(S_KeyPadInfo[index].caption);
        else
            emit charSend(QChar(' '));
    }
    else {
        if (index == KEY_CHAR_PAD) {
            emit inputChanged(0);
        }
        else if (index == KEY_CANCEL) {
            emit commited(false);
        }
        else if (index == KEY_ENTER) {
            emit commited(true);
        }
        else {
            if (index == KEY_BACKSPACE){
                m_pTimer->stop();
            }
            emit keyValSend(S_KeyPadInfo[index].key);
        }
    }
}

void CNumPad::onTimerOut()
{
    m_pTimer->setInterval(100);
    emit keyValSend(S_KeyPadInfo[KEY_BACKSPACE].key);
}

bool CNumPad::eventFilter(QObject *watched, QEvent *event)
{
    int item = -1;
    for (int i = 0; i < KEY_NUM; i++) {
        if (m_lBtn[i] == watched) {
            item = i;
        }
    }

    if (item != -1) {
        if (event->type() == QEvent::FocusIn) {
            m_iFocusItem = item;
        }
        else if (event->type() == QEvent::KeyPress) {
            QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
            if (e) {
                if (dirKeyPress(e->key())) {
                    e->accept();
                    return true;
                }
            }
        }
        else if (event->type() == QEvent::MouseButtonPress){
            QMouseEvent *e = dynamic_cast<QMouseEvent*>(event);
            if (e) {
                if (item == KEY_BACKSPACE){
                    if(e->button() == Qt::LeftButton){
                        m_pTimer->start(1000);
                    }
                }
            }
        }
    }

    return QWidget::eventFilter(watched, event);     // 最后将事件交给上层对话框
}

void CNumPad::init_()
{
    m_lBtn[KEY_1] = ui.pushButton_1;
    m_lBtn[KEY_2] = ui.pushButton_2;
    m_lBtn[KEY_3] = ui.pushButton_3;
    m_lBtn[KEY_4] = ui.pushButton_4;
    m_lBtn[KEY_5] = ui.pushButton_5;
    m_lBtn[KEY_6] = ui.pushButton_6;
    m_lBtn[KEY_7] = ui.pushButton_7;
    m_lBtn[KEY_8] = ui.pushButton_8;
    m_lBtn[KEY_9] = ui.pushButton_9;
    m_lBtn[KEY_0] = ui.pushButton_0;

    m_lBtn[KEY_POINT] = ui.pushButton_Point;
    m_lBtn[KEY_COMMA] = ui.pushButton_Comma;
    m_lBtn[KEY_ADD] = ui.pushButton_Add;
    m_lBtn[KEY_SUB] = ui.pushButton_Sub;
    m_lBtn[KEY_MULTIPLY] = ui.pushButton_Multiply;
    m_lBtn[KEY_DIVIDE] = ui.pushButton_Divide;
    m_lBtn[KEY_EQUAL] = ui.pushButton_Equal;
    m_lBtn[KEY_PERCENT] = ui.pushButton_Percent;
    m_lBtn[KEY_AT] = ui.pushButton_At;
    m_lBtn[KEY_BACKSPACE] = ui.pushButton_Del;

    m_lBtn[KEY_COLON] = ui.pushButton_Colon;
    m_lBtn[KEY_SEMICIOLON] = ui.pushButton_Semicolon;
    m_lBtn[KEY_EXCLAMATION] = ui.pushButton_Exclamation;
    m_lBtn[KEY_QUERY] = ui.pushButton_Query;
    m_lBtn[KEY_LEFTBRACKET] = ui.pushButton_LeftBracket;
    m_lBtn[KEY_RIGHTBRACKET] = ui.pushButton_RightBracket;
    m_lBtn[KEY_LESS] = ui.pushButton_Less;
    m_lBtn[KEY_MORE] = ui.pushButton_More;
    m_lBtn[KEY_CANCEL] = ui.pushButton_Cancel;

    m_lBtn[KEY_CHAR_PAD] = ui.pushButton_CHAR;
    m_lBtn[KEY_SPACE] = ui.pushButton_Space;
    m_lBtn[KEY_UP] = ui.pushButton_Up;
    m_lBtn[KEY_DOWN] = ui.pushButton_Down;
    m_lBtn[KEY_LEFT] = ui.pushButton_Left;
    m_lBtn[KEY_RIGHT] = ui.pushButton_Right;
    m_lBtn[KEY_ENTER] = ui.pushButton_Enter;

    m_pTimer = new QTimer(this);
}

void CNumPad::localConnect()
{
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for (int i = 0; i < KEY_NUM; i++) {
        connect(m_lBtn[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(m_lBtn[i], i);
        m_lBtn[i]->installEventFilter(this);
    }

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(clickButton(int)));
    connect(m_pTimer, SIGNAL(timeout()),this,SLOT(onTimerOut()));
}

bool CNumPad::dirKeyPress(int dirKey)
{
    bool rc = true;
    switch (dirKey) {
    case Qt::Key_Left:
        if (m_iFocusItem > 0) {
            m_iFocusItem--;
        }
        else {
            m_iFocusItem = 0;
        }
        m_lBtn[m_iFocusItem]->setFocus();
        break;
    case Qt::Key_Right:
        m_iFocusItem++;
        if (m_iFocusItem >= KEY_NUM) {
            m_iFocusItem = KEY_NUM - 1;
        }
        m_lBtn[m_iFocusItem]->setFocus();
        break;
    case Qt::Key_Up:
        if (m_iFocusItem <= KEY_0) {

        }
        else if (m_iFocusItem <= KEY_BACKSPACE) {
            m_iFocusItem = m_iFocusItem - KEY_POINT + KEY_1;
        }
        else if (m_iFocusItem <= KEY_CANCEL) {
            m_iFocusItem = m_iFocusItem - KEY_COLON + KEY_POINT;
        }
        else {
            if (m_iFocusItem == KEY_CHAR_PAD)
                m_iFocusItem = KEY_COLON;
            else if (m_iFocusItem == KEY_SPACE)
                m_iFocusItem = KEY_EXCLAMATION;
            else if (m_iFocusItem == KEY_UP)
                m_iFocusItem = KEY_LEFTBRACKET;
            else if (m_iFocusItem == KEY_DOWN)
                m_iFocusItem = KEY_RIGHTBRACKET;
            else if (m_iFocusItem == KEY_LEFT)
                m_iFocusItem = KEY_LESS;
            else if (m_iFocusItem == KEY_RIGHT)
                m_iFocusItem = KEY_MORE;
            else if (m_iFocusItem == KEY_ENTER)
                m_iFocusItem = KEY_CANCEL;
        }
        m_lBtn[m_iFocusItem]->setFocus();
        break;
    case Qt::Key_Down:
        if (m_iFocusItem <= KEY_0) {
            m_iFocusItem = m_iFocusItem - KEY_1 + KEY_POINT;
            if (m_iFocusItem > KEY_BACKSPACE)
                m_iFocusItem = KEY_BACKSPACE;
        }
        else if (m_iFocusItem <= KEY_BACKSPACE) {
            m_iFocusItem = m_iFocusItem - KEY_POINT + KEY_COLON;
            if (m_iFocusItem > KEY_CANCEL)
                m_iFocusItem = KEY_CANCEL;
        }
        else if (m_iFocusItem <= KEY_CANCEL) {
            if (m_iFocusItem == KEY_COLON)
                m_iFocusItem = KEY_CHAR_PAD;
            else if (m_iFocusItem <= KEY_QUERY && m_iFocusItem >= KEY_SEMICIOLON)
                m_iFocusItem = KEY_SPACE;
            else if (m_iFocusItem == KEY_LEFTBRACKET)
                m_iFocusItem = KEY_UP;
            else if (m_iFocusItem == KEY_RIGHTBRACKET)
                m_iFocusItem = KEY_DOWN;
            else if (m_iFocusItem == KEY_LESS)
                m_iFocusItem = KEY_LEFT;
            else if (m_iFocusItem == KEY_MORE)
                m_iFocusItem = KEY_RIGHT;
            else if (m_iFocusItem == KEY_CANCEL)
                m_iFocusItem = KEY_ENTER;
        }
        else {
        }
        m_lBtn[m_iFocusItem]->setFocus();
        break;
    default:
        rc = false;
        break;
    }

    return rc;
}
