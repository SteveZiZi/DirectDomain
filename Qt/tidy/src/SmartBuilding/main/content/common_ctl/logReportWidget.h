/****************************************************************************
**
** file: logReportWidget.h
**
** description:
**     日志界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef LOGREPORTWIDGET_H
#define LOGREPORTWIDGET_H

#include <QtGui/QWidget>
#include <QMap>
#include "src/SmartBuilding/main/content/funWidget/log/logContent.h"

class CReportWidget;
class QLabel;
class QPushButton;
class CBlackBoxW;
class CRecordWidget;
class CDataMgr;
class CLogReportW : public QWidget
{
    Q_OBJECT

    enum S_LOGTYPE{
        RECORD_TYPE,
        BLACKBOX_TYPE,
        NONE_TYPE
    };

public:
    CLogReportW(S_LOGTYPE type,QWidget *parent = 0);
    ~CLogReportW();

    void setLogType(const S_LOGTYPE& type) {m_sLogType = type;}
    void clearLogContent();

private slots:
    void changePage(int action);
    void updateCount(int row,int,int,int);
    void readLog();

private:
    void init_();
    void layout_();
    void localConnect();
    void parseConfig();
    void parselogContent(QByteArray& content);                          // 根据读到的日志文件对日志进行分类
    QString analyzeContent(QString& typeValue,QByteArray& contentArr);  // 解析每条日志的内容
    void showLogContent(QList<CLogContent>& contentList);

private:
    CDataMgr*                   m_pDataMgr;

    CReportWidget*              m_pLogWidget;
    QLabel*                     m_pPageLabel;
    QPushButton*                m_pFlashBtn;

    QMap<QString,QString>       m_mConifgMap;
    QList<CLogContent>          m_lLogList;
    S_LOGTYPE                   m_sLogType;
    QString                     m_sCpcNo;
    int                         m_iLogCount;

private:
    friend class CBlackBoxW;
    friend class CRecordWidget;
};

#endif // LOGREPORTWIDGET_H
