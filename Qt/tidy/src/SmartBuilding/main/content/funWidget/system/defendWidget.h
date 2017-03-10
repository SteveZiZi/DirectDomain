/****************************************************************************
**
** file: defendWidget.h
**
** description:
**     漏电保护界面
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef DEFENDWIDGET_H
#define DEFENDWIDGET_H

#include <QtGui/QWidget>

class QPushButton;
class CReportView;
class CSqlTableModel;
class CDataMgr;
class QTableView;
class CLineEditDelegate;
class CLineEditRealDelegate;
class CComboBoxDelegate;
class CDefendWidget : public QWidget
{
    Q_OBJECT

public:
    CDefendWidget(QWidget *parent = 0);
    ~CDefendWidget();

private slots:
    void readData();
    void saveData();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CDataMgr*                   m_pDataMgr;

    CReportView*                m_pDefendReport;

    CSqlTableModel*             m_pSqlTableModel;

    QPushButton*                m_pReadBtn;
    QPushButton*                m_pSaveBtn;

    bool                        m_bDealing;
};

#endif // DEFENDWIDGET_H
