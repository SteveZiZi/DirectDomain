/****************************************************************************
**
** file: configWidget.h
**
** description:
**     ≈‰÷√ΩÁ√Ê
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QtGui/QTabWidget>

class CDimTableWidget;
class CDisposeWidget;
class CConfigeW : public QTabWidget
{
    Q_OBJECT

public:
    CConfigeW(QWidget *parent = 0);
    ~CConfigeW();

private:
    void init_();

private:
    CDimTableWidget*                m_pDimTableWidget;
    CDisposeWidget*                 m_pDisposeWidget;
};

#endif // CONFIGWIDGET_H
