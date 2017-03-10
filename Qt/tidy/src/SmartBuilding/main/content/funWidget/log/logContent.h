/****************************************************************************
**
** file: logContent.h
**
** description:
**     »’÷æ Ù–‘
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LOGCONTENT_H
#define LOGCONTENT_H

#include <QString>
#include <QDateTime>

class CLogContent
{
public:
    CLogContent();
    ~CLogContent();

    QDateTime& time() {return m_sTime;}
    void setTime(const QDateTime& time_) {m_sTime = time_;}
    QString& type() {return m_sType;}
    void setType(const QString& type_) {m_sType = type_;}
    QString& content() {return m_sContent;}
    void setContent(const QString& content_) {m_sContent = content_;}

private:
    QDateTime                   m_sTime;
    QString                     m_sType;
    QString                     m_sContent;
};

#endif // LOGCONTENT_H
