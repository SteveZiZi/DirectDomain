/****************************************************************************
**
** file: pwdWidget.h
**
** description:
**     密码更改界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef PWDWIDGET_H
#define PWDWIDGET_H

#include <QtGui/QWidget>

class QLineEdit;
class QPushButton;
class QFrame;
class CPwdWidget : public QWidget
{
    Q_OBJECT

public:
    CPwdWidget(QWidget *parent = 0);
    ~CPwdWidget();

private slots:
    void savePwd();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    QFrame*                     m_pFrame;
    QPushButton*                m_pSaveBtn;

    QLineEdit*                  m_pOldPwdEdit;
    QLineEdit*                  m_pNewPwdEdit;
    QLineEdit*                  m_pEnsurePwdEdit;
};

#endif // PWDWIDGET_H
