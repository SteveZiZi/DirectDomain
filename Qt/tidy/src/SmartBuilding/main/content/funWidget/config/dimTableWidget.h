/****************************************************************************
**
** file: dimTableWidget.h
**
** description:
**     Dim±í
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef DIMTABLEWIDGET_H
#define DIMTABLEWIDGET_H

#include <QtGui/QWidget>

class CReportView;
class QPushButton;
class CSqlTableModel;
class CDimTableWidget : public QWidget
{
    Q_OBJECT

public:
    CDimTableWidget(QWidget *parent = 0);
    ~CDimTableWidget();

private slots:
    void readData();
    void saveData();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CReportView*                m_pDimReport;

    CSqlTableModel*             m_pSqlTableModel;

    QPushButton*                m_pReadBtn;
    QPushButton*                m_pSaveBtn;
};

#endif // DIMTABLEWIDGET_H
