#include "characterPad.h"

#include <QSignalMapper>
#include <QPushButton>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

enum {
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,

    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_BACKSPACE,

    KEY_CAPS,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_CANCEL,

    KEY_NUM_PAD,
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
    QChar capsCaption;
};

const KeyPadInfo S_KeyPadInfo[CCharacterPad::KEY_NUM] =
{
    { 0, Qt::Key_Q, 'q', 'Q' },
    { 1, Qt::Key_W, 'w', 'W' },
    { 2, Qt::Key_E, 'e', 'E' },
    { 3, Qt::Key_R, 'r', 'R' },
    { 4, Qt::Key_T, 't', 'T' },
    { 5, Qt::Key_Y, 'y', 'Y' },
    { 6, Qt::Key_U, 'u', 'U' },
    { 7, Qt::Key_I, 'i', 'I' },
    { 8, Qt::Key_O, 'o', 'O' },
    { 9, Qt::Key_P, 'p', 'P' },

    { 10, Qt::Key_A, 'a', 'A' },
    { 11, Qt::Key_S, 's', 'S' },
    { 12, Qt::Key_D, 'd', 'D' },
    { 13, Qt::Key_F, 'f', 'F' },
    { 14, Qt::Key_G, 'g', 'G' },
    { 15, Qt::Key_H, 'h', 'H' },
    { 16, Qt::Key_J, 'j', 'J' },
    { 17, Qt::Key_K, 'k', 'K' },
    { 18, Qt::Key_L, 'l', 'L' },
    { 19, Qt::Key_Backspace, 0, 0 },

    { 20, Qt::Key_CapsLock, 0, 0 },
    { 21, Qt::Key_Z, 'z', 'Z' },
    { 22, Qt::Key_X, 'x', 'X' },
    { 23, Qt::Key_C, 'c', 'C' },
    { 24, Qt::Key_V, 'v', 'V' },
    { 25, Qt::Key_B, 'b', 'B' },
    { 26, Qt::Key_N, 'n', 'N' },
    { 27, Qt::Key_M, 'm', 'M' },
    { 28, Qt::Key_Cancel, 0, 0 },

    { 29, 0, 0, 0 },
    { 30, Qt::Key_Space, ' ', ' ' },
    { 31, Qt::Key_Up, 0, 0 },
    { 32, Qt::Key_Down, 0, 0 },
    { 33, Qt::Key_Left, 0, 0 },
    { 34, Qt::Key_Right, 0, 0 },
    { 35, Qt::Key_Enter, 0, 0 },
};

CCharacterPad::CCharacterPad(QWidget *parent)
: QWidget(parent)
, m_bCaps(false)
{
    ui.setupUi(this);

    init_();
    localConnect();
}

CCharacterPad::~CCharacterPad()
{

}

void CCharacterPad::resetWidget(void)
{
    m_iFocusItem = KEY_ENTER;
    m_lBtn[m_iFocusItem]->setFocus();
}

void CCharacterPad::clickButton(int index)
{
    Q_ASSERT(index < KEY_NUM);

    if (S_KeyPadInfo[index].caption != 0) {
        if (index != KEY_SPACE){
            if (m_bCaps)
                emit charSend(S_KeyPadInfo[index].capsCaption);
            else
                emit charSend(S_KeyPadInfo[index].caption);
        }
        else
            emit charSend(QChar(' '));
    }
    else {
        if (index == KEY_CAPS) {
            m_bCaps = !m_bCaps;
            if (m_bCaps) {
                changeToUpper();
                m_lBtn[index]->setText("CAPS");
            }
            else {
                changeToLower();
                m_lBtn[index]->setText("Caps");
            }
        }
        else
        {
            if (index == KEY_NUM_PAD) {
                emit inputChanged(1);
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
}

void CCharacterPad::onTimerOut()
{
    m_pTimer->setInterval(100);
    emit keyValSend(S_KeyPadInfo[KEY_BACKSPACE].key);
}

bool CCharacterPad::eventFilter(QObject *watched, QEvent *event)
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

void CCharacterPad::init_()
{
    m_lBtn[KEY_Q] = ui.pushButton_Q;
    m_lBtn[KEY_W] = ui.pushButton_W;
    m_lBtn[KEY_E] = ui.pushButton_E;
    m_lBtn[KEY_R] = ui.pushButton_R;
    m_lBtn[KEY_T] = ui.pushButton_T;
    m_lBtn[KEY_Y] = ui.pushButton_Y;
    m_lBtn[KEY_U] = ui.pushButton_U;
    m_lBtn[KEY_I] = ui.pushButton_I;
    m_lBtn[KEY_O] = ui.pushButton_O;
    m_lBtn[KEY_P] = ui.pushButton_P;

    m_lBtn[KEY_A] = ui.pushButton_A;
    m_lBtn[KEY_S] = ui.pushButton_S;
    m_lBtn[KEY_D] = ui.pushButton_D;
    m_lBtn[KEY_F] = ui.pushButton_F;
    m_lBtn[KEY_G] = ui.pushButton_G;
    m_lBtn[KEY_H] = ui.pushButton_H;
    m_lBtn[KEY_J] = ui.pushButton_J;
    m_lBtn[KEY_K] = ui.pushButton_K;
    m_lBtn[KEY_L] = ui.pushButton_L;
    m_lBtn[KEY_BACKSPACE] = ui.pushButton_Del;

    m_lBtn[KEY_CAPS] = ui.pushButton_Caps;
    m_lBtn[KEY_Z] = ui.pushButton_Z;
    m_lBtn[KEY_X] = ui.pushButton_X;
    m_lBtn[KEY_C] = ui.pushButton_C;
    m_lBtn[KEY_V] = ui.pushButton_V;
    m_lBtn[KEY_B] = ui.pushButton_B;
    m_lBtn[KEY_N] = ui.pushButton_N;
    m_lBtn[KEY_M] = ui.pushButton_M;
    m_lBtn[KEY_CANCEL] = ui.pushButton_Cancel;

    m_lBtn[KEY_NUM_PAD] = ui.pushButton_NUM;
    m_lBtn[KEY_SPACE] = ui.pushButton_Space;
    m_lBtn[KEY_UP] = ui.pushButton_Up;
    m_lBtn[KEY_DOWN] = ui.pushButton_Down;
    m_lBtn[KEY_LEFT] = ui.pushButton_Left;
    m_lBtn[KEY_RIGHT] = ui.pushButton_Right;
    m_lBtn[KEY_ENTER] = ui.pushButton_Enter;

    m_pTimer = new QTimer(this);
}

void CCharacterPad::localConnect()
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

void CCharacterPad::changeToLower()
{
    for (int i = 0; i < KEY_NUM; i++) {
        if (S_KeyPadInfo[i].caption != 0) {
            m_lBtn[i]->setText(S_KeyPadInfo[i].caption);
        }
    }
}

void CCharacterPad::changeToUpper()
{
    for (int i = 0; i < KEY_NUM; i++) {
        if (S_KeyPadInfo[i].capsCaption != 0) {
            m_lBtn[i]->setText(S_KeyPadInfo[i].capsCaption);
        }
    }
}

bool CCharacterPad::dirKeyPress(int dirKey)
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
        if (m_iFocusItem <= KEY_P) {

        }
        else if (m_iFocusItem <= KEY_BACKSPACE) {
            m_iFocusItem = m_iFocusItem - KEY_A + KEY_Q;
        }
        else if (m_iFocusItem <= KEY_CANCEL) {
            m_iFocusItem = m_iFocusItem - KEY_CAPS + KEY_A;
        }
        else {
            if (m_iFocusItem == KEY_NUM_PAD)
                m_iFocusItem = KEY_CAPS;
            else if (m_iFocusItem == KEY_SPACE)
                m_iFocusItem = KEY_X;
            else if (m_iFocusItem == KEY_UP)
                m_iFocusItem = KEY_V;
            else if (m_iFocusItem == KEY_DOWN)
                m_iFocusItem = KEY_B;
            else if (m_iFocusItem == KEY_LEFT)
                m_iFocusItem = KEY_N;
            else if (m_iFocusItem == KEY_RIGHT)
                m_iFocusItem = KEY_M;
            else if (m_iFocusItem == KEY_ENTER)
                m_iFocusItem = KEY_CANCEL;
        }
        m_lBtn[m_iFocusItem]->setFocus();
        break;
    case Qt::Key_Down:
        if (m_iFocusItem <= KEY_P) {
            m_iFocusItem = m_iFocusItem - KEY_Q + KEY_A;
            if (m_iFocusItem > KEY_BACKSPACE)
                m_iFocusItem = KEY_BACKSPACE;
        }
        else if (m_iFocusItem <= KEY_BACKSPACE) {
            m_iFocusItem = m_iFocusItem - KEY_A + KEY_CAPS;
            if (m_iFocusItem > KEY_CANCEL)
                m_iFocusItem = KEY_CANCEL;
        }
        else if (m_iFocusItem <= KEY_CANCEL) {
            if (m_iFocusItem == KEY_CAPS)
                m_iFocusItem = KEY_NUM_PAD;
            else if (m_iFocusItem <= KEY_Z && m_iFocusItem >= KEY_C)
                m_iFocusItem = KEY_SPACE;
            else if (m_iFocusItem == KEY_V)
                m_iFocusItem = KEY_UP;
            else if (m_iFocusItem == KEY_B)
                m_iFocusItem = KEY_DOWN;
            else if (m_iFocusItem == KEY_N)
                m_iFocusItem = KEY_LEFT;
            else if (m_iFocusItem == KEY_M)
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

