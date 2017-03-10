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
    this->setEditTriggers(QAbstractItemView::CurrentChanged);           // ���õ������ɱ༭
    this->verticalHeader()->setVisible(false);                          // ���ش�ֱ��
    this->setAlternatingRowColors(true);                                // ��������ɫ����ʹ��
    this->horizontalHeader()->setClickable(false);                      // ���ñ�ͷ���ɵ��
    this->setItemDelegate(new NoFocusItemDelegate(this));
}
