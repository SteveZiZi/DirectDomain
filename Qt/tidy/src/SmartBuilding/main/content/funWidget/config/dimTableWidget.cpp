#include "dimTableWidget.h"

#include <QPushButton>
#include <QHeaderView>
#include <QTableView>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/content/common_ctl/reportView.h"
#include "src/SmartBuilding/main/content/common_ctl/sqlTableModel.h"
#include "src/SmartBuilding/main/content/common_ctl/lineEditDelegate.h"


enum {
    DIM_E = 0,
    HUNDRED_E,
    NINETY_E,
    EIGHTY_E,
    SEVENTY_E,
    SIXTY_E,
    FIFTY_E,
    FORTY_E,
    THIRTY_E,
    TWENTY_E,
    TEN_E,
    ZERO_E,
    DIM_COLUMN_COUNT
};

CDimTableWidget::CDimTableWidget(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();
}

CDimTableWidget::~CDimTableWidget()
{

}

void CDimTableWidget::readData()
{
    m_pSqlTableModel->setTable("dim");
    m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pSqlTableModel->select();
    m_pSqlTableModel->setHeaderData(DIM_E,Qt::Horizontal,"Dim");
    m_pSqlTableModel->setHeaderData(HUNDRED_E,Qt::Horizontal,"100%");
    m_pSqlTableModel->setHeaderData(NINETY_E,Qt::Horizontal,"90%");
    m_pSqlTableModel->setHeaderData(EIGHTY_E,Qt::Horizontal,"80%");
    m_pSqlTableModel->setHeaderData(SEVENTY_E,Qt::Horizontal,"70%");
    m_pSqlTableModel->setHeaderData(SIXTY_E,Qt::Horizontal,"60%");
    m_pSqlTableModel->setHeaderData(FIFTY_E,Qt::Horizontal,"50%");
    m_pSqlTableModel->setHeaderData(FORTY_E,Qt::Horizontal,"40%");
    m_pSqlTableModel->setHeaderData(THIRTY_E,Qt::Horizontal,"30%");
    m_pSqlTableModel->setHeaderData(TWENTY_E,Qt::Horizontal,"20%");
    m_pSqlTableModel->setHeaderData(TEN_E,Qt::Horizontal,"10%");
    m_pSqlTableModel->setHeaderData(ZERO_E,Qt::Horizontal,"0%");
}

void CDimTableWidget::saveData()
{
    m_pSqlTableModel->database().transaction();                         //开始事务操作
    m_pSqlTableModel->submitAll();
    m_pSqlTableModel->database().commit();                              //提交
}

void CDimTableWidget::init_()
{
    m_pSqlTableModel = new CSqlTableModel(this);

    readData();

    m_pDimReport = new CReportView(this);
    CLineEditDelegate *lineEditDelegate = new CLineEditDelegate(this);
    m_pDimReport->setModel(m_pSqlTableModel);
    m_pDimReport->setColumnWidth(0,DIM_CIRCUIT_W);
    for (int column = 1;column < DIM_COLUMN_COUNT;column++){
        m_pDimReport->setItemDelegateForColumn(column,lineEditDelegate);
        m_pDimReport->setColumnWidth(column,DIM_PRECENT_W);
    }
    m_pDimReport->horizontalHeader()->setStyleSheet("background-color: #4378ac;color: white;");  // bug 不明

    m_pReadBtn = new QPushButton(tr("读取"),this);
    m_pSaveBtn = new QPushButton(tr("保存"),this);

    layout_();
}

void CDimTableWidget::layout_()
{
    m_pDimReport->setGeometry(DIM_REPORT_L,DIM_REPORT_T,
                              DIM_REPORT_W,DIM_REPORT_H);
    m_pReadBtn->setGeometry(CONFIG_READ_BTN_L,CONFIG_READ_BTN_T,
                            CONFIG_READ_BTN_W,CONFIG_READ_BTN_H);
    m_pSaveBtn->setGeometry(CONFIG_SAVE_BTN_L,CONFIG_SAVE_BTN_T,
                            CONFIG_SAVE_BTN_W,CONFIG_SAVE_BTN_H);
}

void CDimTableWidget::localConnect()
{
    connect(m_pReadBtn,SIGNAL(clicked()),this,SLOT(readData()));
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(saveData()));
}
