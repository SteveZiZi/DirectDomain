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
 *  @Desc:          ȷ������
 *  
 *  @Return:        true:��֤ok false:ʧ��
 */
inline bool ensurePwd(int& reg)
{
    bool ret = false;

    QInputDialog pwdDialog;
    pwdDialog.setOkButtonText(QObject::tr("ȷ��"));
    pwdDialog.setCancelButtonText(QObject::tr("ȡ��"));
    pwdDialog.setModal(true);
    pwdDialog.setLabelText(QObject::tr("����������:"));
    pwdDialog.setInputMode(QInputDialog::TextInput);
    pwdDialog.setTextEchoMode(QLineEdit::Password);
    pwdDialog.setFocusPolicy(Qt::StrongFocus);
    pwdDialog.setWindowModality(Qt::WindowModal);                        // ��Ҫ����ģʽ����������޷�����
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
