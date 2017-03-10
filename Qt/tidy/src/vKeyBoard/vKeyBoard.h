/****************************************************************************
**
** file: vKeyBoard.h
**
** description:
**     ÐéÄâ¼üÅÌ
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef VKEYBOARD_H
#define VKEYBOARD_H

#include <QInputContext>

class CPlatForm;
class vKeyBoard : public QInputContext
{
    Q_OBJECT;

public:
    vKeyBoard(QObject *parent = 0);
    ~vKeyBoard();

public slots:
    void sendChar(const QChar& ch);
    void sendKeyVal(int val);
    void commit(bool result);

public:
    virtual QString identifierName();
    virtual QString language();
    virtual void reset();
    virtual bool isComposing() const;

protected:
    virtual bool filterEvent(const QEvent *event);

private:
    void init_();
    void updatePosition();

private:
    CPlatForm*                      m_pPlatForm;
};

#endif // VKEYBOARD_H
