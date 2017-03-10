#ifndef REPORTVIEW_H
#define REPORTVIEW_H

#include <QTableView>

class CReportView : public QTableView
{
    Q_OBJECT

public:
    CReportView(QWidget *parent = 0);
    ~CReportView();

private:
    void init_();
};

#endif // REPORTVIEW_H
