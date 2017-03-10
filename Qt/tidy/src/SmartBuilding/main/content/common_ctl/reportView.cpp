#include "reportView.h"

#include <QHeaderView>

#include "src/SmartBuilding/main/content/common_ctl/nofocusItemdelegate.h"

CReportView::CReportView(QWidget *parent)
: QTableView(parent)
{
    init_();
}

CReportView::~CReportView()
{

}

void CReportView::init_()
{
    this->setEditTriggers(QAbstractItemView::CurrentChanged);           // 设置单击即可编辑
    this->verticalHeader()->setVisible(false);                          // 隐藏垂直列
    this->setAlternatingRowColors(true);                                // 设置行颜色交替使能
    this->horizontalHeader()->setClickable(false);                      // 设置表头不可点击
    this->setItemDelegate(new NoFocusItemDelegate(this));
}
