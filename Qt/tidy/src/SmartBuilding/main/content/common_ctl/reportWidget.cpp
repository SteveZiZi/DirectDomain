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
    this->clearContents();                                              // ɾ��ԭ�е�����
}

bool CReportWidget::addOneRow()
{
    int rowCount = this->rowCount();
    rowCount += 1;
    this->setRowCount(rowCount);                                        // ����������1

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
    this->setItemDelegate(new NoFocusItemDelegate(this));               // ȥ��Itemѡ��ʱ�����

    this->verticalHeader()->hide();
    this->horizontalScrollBar()->hide();

    this->setAlternatingRowColors(true);                                // �����м���ɫ����
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);           // ���ò��ɱ༭
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);      // ����ÿ�������Զ���Ӧ���
    this->horizontalHeader()->setFixedHeight(35);
    this->horizontalHeader()->setClickable(false);                      // ���ñ�ͷ���ɵ��
}
