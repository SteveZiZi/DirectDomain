#ifndef POPINPUTPWD_H
#define POPINPUTPWD_H

#include <QtGui/QInputDialog>
#include <QLineEdit>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QFile>

#define PASSWORD_FILE           "./dat/password.dat"

/*
 *  @Func:          ensurePwd
 *  @Desc:          确认密码
 *  
 *  @Return:        true:验证ok false:失败
 */
inline bool ensurePwd(int& reg)
{
    bool ret = false;

    QInputDialog pwdDialog;
    pwdDialog.setOkButtonText(QObject::tr("确定"));
    pwdDialog.setCancelButtonText(QObject::tr("取消"));
    pwdDialog.setModal(true);
    pwdDialog.setLabelText(QObject::tr("请输入密码:"));
    pwdDialog.setInputMode(QInputDialog::TextInput);
    pwdDialog.setTextEchoMode(QLineEdit::Password);
    pwdDialog.setFocusPolicy(Qt::StrongFocus);
    pwdDialog.setWindowModality(Qt::WindowModal);                        // 需要设置模式，否则键盘无法输入
    reg = pwdDialog.exec();

    if (QInputDialog::Accepted == reg){
        QFile file(PASSWORD_FILE);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "open password.dat fail";
            return ret;
        }
        QTextStream in(&file);
        QStringList passwordList;
        QString temp = in.readAll();
        if (!temp.isNull()){
            passwordList = temp.split(" = ");
            file.close();
        }
        else 
            return ret;

        ret = (pwdDialog.textValue() == passwordList[1]) ? true : false;
    }

    return ret;
}

#endif // POPINPUTPWD_H
