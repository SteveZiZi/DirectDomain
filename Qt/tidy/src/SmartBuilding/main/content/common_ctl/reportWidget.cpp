#include "reportWidget.h"

#include <QHeaderView>
#include <QScrollBar>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/content/common_ctl/noFocusItemDelegate.h"

CReportWidget::CReportWidget(QWidget *parent)
: QTableWidget(parent)
, m_iRrowH(REPORT_ROW_HEIGHT)
{
    init_();
}

CReportWidget::~CReportWidget()
{

}

void CReportWidget::createReportWidget(int row,int column)
{
    this->setRowCount(row);
    this->setColumnCount(column);

    for (int row = 0; row < this->rowCount(); ++row) {
        for (int column = 0; column < this->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem();
            newItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            this->setItem(row, column, newItem);
        }
        this->setRowHeight(row, m_iRrowH);
    }
}

void CReportWidget::showHorizontalHeader(bool flag)
{
    if (flag)
        this->horizontalHeader()->show();
    else
        this->horizontalHeader()->hide();
}

void CReportWidget::setHeaderContent(QStringList& header)
{
    this->setHorizontalHeaderLabels(header);
}

void CReportWidget::setColumnColor(int column,const QColor& color)
{
    Q_ASSERT(column >= 0 && column < this->columnCount());

    for(int row = 0;row < this->rowCount();row++){
        this->item(row,column)->setBackgroundColor(color);
    }
}

void CReportWidget::setColumnAlignment(int column,int alignment)
{
    Q_ASSERT(column >= 0 && column < this->columnCount());

    for(int row = 0;row < this->rowCount();row++){
        this->item(row,column)->setTextAlignment(alignment);
    }
}

void CReportWidget::setContentHeight(int hight)
{
    m_iRrowH = hight;
}

void CReportWidget::clearReportContent()
{
    this->setRowCount(0);
    this->clearContents();                                              // 删除原有的内容
}

bool CReportWidget::addOneRow()
{
    int rowCount = this->rowCount();
    rowCount += 1;
    this->setRowCount(rowCount);                                        // 总行数增加1

    for (int column = 0;column < this->columnCount();column++){
        QTableWidgetItem *newItem = new QTableWidgetItem();
        newItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        this->setItem(rowCount - 1, column, newItem);
    }

    return true;
}

bool CReportWidget::delOneRow(int row)
{
    if(row < 0 || row > this->rowCount())
        return false;

    this->removeRow(row);
    return true;
}

void CReportWidget::setSelfModel(QAbstractItemModel *model)
{
    return QTableView::setModel(model);
}

void CReportWidget::init_()
{
    this->setItemDelegate(new NoFocusItemDelegate(this));               // 去掉Item选中时的虚框

    this->verticalHeader()->hide();
    this->horizontalScrollBar()->hide();

    this->setAlternatingRowColors(true);                                // 设置行间颜色交差
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);           // 设置不可编辑
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);      // 设置每行内容自动适应宽度
    this->horizontalHeader()->setFixedHeight(35);
    this->horizontalHeader()->setClickable(false);                      // 设置表头不可点击
}
