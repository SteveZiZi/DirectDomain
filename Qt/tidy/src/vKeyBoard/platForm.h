/****************************************************************************
**
** file: platForm.h
**
** description:
**     Êý×Ö¼üÅÌºÍ×ÖÄ¸¼üÅÌµÄ³ÐÔØ´°¿Ú
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtGui/QWidget>

class vKeyBoard;
class CCharacterPad;
class CNumPad;
class CPlatForm : public QWidget
{
    Q_OBJECT

    enum INPUT_TYPE
    {
        INPUT_CHARACTER,
        INPUT_NUMBER
    };

public:
    CPlatForm(vKeyBoard* keyBoard,QWidget *parent = 0);
    ~CPlatForm();

    QWidget* getFocusWidget() {return m_pLastFocusedWidget;}

protected:
    bool event(QEvent *e);

private slots:
    void changeInput(int type);
    void saveFocusWidget(QWidget *oldFocus,QWidget *newFocus);

private:
    void init_();
    void localConnect();

private:
    QWidget*                            m_pLastFocusedWidget;
    vKeyBoard*                          m_pKeyBoard;
    CCharacterPad*                      m_pCharacterPad;
    CNumPad*                            m_pNumPad;
};

#endif // PLATFORM_H
