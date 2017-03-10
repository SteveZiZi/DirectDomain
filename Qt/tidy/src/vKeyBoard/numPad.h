/****************************************************************************
**
** file: numPad.h
**
** description:
**     数字和特殊符号键盘界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef NUMPAD_H
#define NUMPAD_H

#include <QWidget>
#include "ui_numPad.h"

class vKeyBoard;
class QPushButton;
class QTimer;
class CNumPad : public QWidget
{
    Q_OBJECT

public:
    CNumPad(QWidget *parent = 0);
    ~CNumPad();

    void resetWidget(void);

signals:
    void charSend(const QChar& ch);
    void keyValSend(int key);
    void inputChanged(int type);
    void commited(bool result);

private slots:
    void clickButton(int index);
    void onTimerOut();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    void init_();
    void localConnect();
    bool dirKeyPress(int dirKey);

public:
    enum {KEY_NUM = 36};

private:
    Ui::CNumPad ui;
    QPushButton*                    m_lBtn[KEY_NUM];

    int                             m_iFocusItem;

    QTimer*                         m_pTimer;                           // 连续删除定时器
};

#endif // NUMPAD_H
