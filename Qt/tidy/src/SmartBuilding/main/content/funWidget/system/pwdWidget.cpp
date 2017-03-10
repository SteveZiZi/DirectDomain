#include "pwdWidget.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTextStream>
#include <QLabel>
#include <QFrame>
#include <QFile>
#include <QDebug>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/content/common_ctl/toolTip.h"

#define PASSWORD_FILE           "./dat/password.dat"

CPwdWidget::CPwdWidget(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();
}

CPwdWidget::~CPwdWidget()
{

}

void CPwdWidget::savePwd()
{
    if(m_pOldPwdEdit->text().isEmpty() || m_pNewPwdEdit->text().isEmpty()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)),tr("密码不能为空!"));
        return;
    }

    QFile file(PASSWORD_FILE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "open password.dat fail";
        return;
    }
    QTextStream in(&file);
    QStringList passwordList;
    QString fileContent = in.readAll();
    passwordList += fileContent.split(" = ");
    file.close();

    if(passwordList[1] != m_pOldPwdEdit->text()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)), tr("旧密码输入错误!"));
        return;
    }
    else if(m_pNewPwdEdit->text() != m_pEnsurePwdEdit->text()){
        popToolTip(m_pSaveBtn->mapToGlobal(QPoint(0, 0)), tr("两次密码输入不一致!"));
        return;
    }
    else
    {
        QFile file(PASSWORD_FILE);
        QString newPwd = QString("password = %1").arg(m_pNewPwdEdit->text());
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "open password.dat fail";
            return;
        }
        QTextStream out(&file);
        out << newPwd;
        file.close();

        // 设置成功后清空输入的密码
        m_pOldPwdEdit->setText(tr(""));
        m_pNewPwdEdit->setText(tr(""));
        m_pEnsurePwdEdit->setText(tr(""));
    }
}

void CPwdWidget::init_()
{
    m_pFrame = new QFrame(this);
    m_pOldPwdEdit = new QLineEdit(m_pFrame);
    m_pOldPwdEdit->setEchoMode(QLineEdit::Password);
    m_pNewPwdEdit = new QLineEdit(m_pFrame);
    m_pNewPwdEdit->setEchoMode(QLineEdit::Password);
    m_pEnsurePwdEdit = new QLineEdit(m_pFrame);
    m_pEnsurePwdEdit->setEchoMode(QLineEdit::Password);

    m_pSaveBtn = new QPushButton(tr("保存"),this);

    layout_();
}

void CPwdWidget::layout_()
{
    m_pFrame->setGeometry(SYSTEM_FRAME_L,SYSTEM_FRAME_T,
                          SYSTEM_FRAME_W,SYSTEM_FRAME_H);
    m_pSaveBtn->setGeometry(SYSTEM_SAVE_BTN_L,SYSTEM_SAVE_BTN_T,
                            SYSTEM_SAVE_BTN_W,SYSTEM_SAVE_BTN_H);

    QLabel* oldPwdLabel = new QLabel(tr("请 输 入 旧 密 码 :"),m_pFrame);
    oldPwdLabel->setGeometry(OLD_PWD_LABEL_L,OLD_PWD_LABEL_T,
                             OLD_PWD_LABEL_W,OLD_PWD_LABEL_H);
    m_pOldPwdEdit->setGeometry(OLD_PWD_EDIT_L,OLD_PWD_EDIT_T,
                               OLD_PWD_EDIT_W,OLD_PWD_EDIT_H);

    QLabel* newPwdLabel = new QLabel(tr("请 输 入 新 密 码 :"),m_pFrame);
    newPwdLabel->setGeometry(NEW_PWD_LABEL_L,NEW_PWD_LABEL_T,
                             NEW_PWD_LABEL_W,NEW_PWD_LABEL_H);
    m_pNewPwdEdit->setGeometry(NEW_PWD_EDIT_L,NEW_PWD_EDIT_T,
                               NEW_PWD_EDIT_W,NEW_PWD_EDIT_H);

    QLabel* ensurePwdLabel = new QLabel(tr("请 确 认 新 密 码 :"),m_pFrame);
    ensurePwdLabel->setGeometry(ENSURE_PWD_LABEL_L,ENSURE_PWD_LABEL_T,
                                ENSURE_PWD_LABEL_W,ENSURE_PWD_LABEL_H);
    m_pEnsurePwdEdit->setGeometry(ENSURE_PWD_EDIT_L,ENSURE_PWD_EDIT_T,
                                  ENSURE_PWD_EDIT_W,ENSURE_PWD_EDIT_H);
}

void CPwdWidget::localConnect()
{
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(savePwd()));
}

