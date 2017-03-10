/****************************************************************************
**
** file: characterPad.h
**
** description:
**     ��ĸ����
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CHARACTERPAD_H
#define CHARACTERPAD_H

#include <QWidget>
#include "ui_characterPad.h"

class vKeyBoard;
class QPushButton;
class QTimer;
class CCharacterPad : public QWidget
{
    Q_OBJECT
public:
    CCharacterPad(QWidget *parent = 0);
    ~CCharacterPad();

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
    void changeToLower();
    void changeToUpper();
    bool dirKeyPress(int dirKey);

public:
    enum {KEY_NUM = 36};

private:
    Ui::CCharacterPad ui;
    QPushButton*                    m_lBtn[KEY_NUM];

    bool                            m_bCaps;                            // �Ƿ��д
    int                             m_iFocusItem;

    QTimer*                         m_pTimer;                           // ����ɾ����ʱ��
};

#endif // CHARACTERPAD_H
