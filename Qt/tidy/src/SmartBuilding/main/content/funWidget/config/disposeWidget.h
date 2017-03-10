/****************************************************************************
**
** file: disposeWidget.h
**
** description:
**     ²¿Êð½çÃæ
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef DISPOSEWIDGET_H
#define DISPOSEWIDGET_H

#include <QtGui/QWidget>

class QPushButton;
class QFrame;
class CColorWidget;
class CDataMgr;
class CDisposeWidget : public QWidget
{
    Q_OBJECT

public:
    CDisposeWidget(QWidget *parent = 0);
    ~CDisposeWidget();

private slots:
    void flushDispos();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CDataMgr*                   m_pDataMgr;

    QFrame*                     m_pFrame;
    QPushButton*                m_pFlushBtn;

    QList<CColorWidget*>        m_lColorWidgetList;
};

#endif // DISPOSEWIDGET_H
