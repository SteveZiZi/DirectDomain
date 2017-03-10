#include "defendWidget.h"

#include <QComboBox>
#include <QPushButton>
#include <QHeaderView>
#include <QTableView>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/main/content/common_ctl/popInputPwd.h"
#include "src/SmartBuilding/main/content/common_ctl/popSureWidget.h"
#include "src/SmartBuilding/main/content/common_ctl/reportView.h"
#include "src/SmartBuilding/main/content/common_ctl/comboBoxDelegate.h"
#include "src/SmartBuilding/main/content/common_ctl/sqlTableModel.h"
#include "src/SmartBuilding/main/content/common_ctl/lineEditDelegate.h"
#include "src/SmartBuilding/main/content/common_ctl/lineEditRealDelegate.h"

#define MAX_CIRCUIT_COUNT                   16

enum {
    CIRCUIT_E = 0,
    CHANNEL_E,
    THRESHOLD_E,
    DELAY_E,
    SWITCH_E,
    SUPPLY_SWITCH_E
};

CDefendWidget::CDefendWidget(QWidget *parent)
: QWidget(parent)
, m_bDealing(false)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
}

CDefendWidget::~CDefendWidget()
{

}

void CDefendWidget::readData()
{
    m_pSqlTableModel->select();
    m_pSqlTableModel->setHeaderData(CIRCUIT_E,Qt::Horizontal,tr("回路"));
    m_pSqlTableModel->setHeaderData(CHANNEL_E,Qt::Horizontal,tr("通道"));
    m_pSqlTableModel->setHeaderData(THRESHOLD_E,Qt::Horizontal,tr("阀值(V)"));
    m_pSqlTableModel->setHeaderData(DELAY_E,Qt::Horizontal,tr("延迟时间(ms)"));
    m_pSqlTableModel->setHeaderData(SWITCH_E,Qt::Horizontal,tr("漏电开关"));
    m_pSqlTableModel->setHeaderData(SUPPLY_SWITCH_E,Qt::Horizontal,tr("电源投切"));
}

void CDefendWidget::saveData()
{
    if (m_bDealing)
        return;

    m_bDealing = true;
    int reg;
    bool ret;
    ret = ensurePwd(reg);
    if (reg != QInputDialog::Accepted){
        m_bDealing = false;
        return;
    }
    else
    {
        if (ret){
            for (int i = 0;i < MAX_CIRCUIT_COUNT;i++)
            {
                m_pDataMgr->setChannel(i,m_pSqlTableModel->data(m_pSqlTableModel->index(i - 1,1),Qt::DisplayRole).toInt());
                m_pDataMgr->setDtVol(i,(int)m_pSqlTableModel->data(m_pSqlTableModel->index(i - 1,2),Qt::DisplayRole).toDouble() * 1000);
                m_pDataMgr->setDelayMs(i,m_pSqlTableModel->data(m_pSqlTableModel->index(i - 1,3),Qt::DisplayRole).toInt());
                if(tr("关闭") == m_pSqlTableModel->data(m_pSqlTableModel->index(i - 1,4),Qt::DisplayRole).toString())
                    m_pDataMgr->setDefendEnable(i,0);
                else
                    m_pDataMgr->setDefendEnable(i,1);
                if(tr("关闭") == m_pSqlTableModel->data(m_pSqlTableModel->index(i - 1,5),Qt::DisplayRole).toString())
                    m_pDataMgr->setCutEnable(i,0);
                else
                    m_pDataMgr->setCutEnable(i,1);
            }
            
            INT8U databuf[4] = {0};
            m_pDataMgr->sendCommands(SAVE_DEFEND_INFO_CMD,databuf);
            m_pSqlTableModel->database().transaction();                         //开始事务操作
            m_pSqlTableModel->submitAll();
            m_pSqlTableModel->database().commit();                              //提交
        }
        else
        {
            popEnsureDlg(tr("提示"),tr("密码错误！"));
        }
    }

    m_bDealing = false;
}

void CDefendWidget::init_()
{
    CLineEditDelegate *lineEditDelegate = new CLineEditDelegate(this);
    CLineEditRealDelegate *lineEditRealDelegate = new CLineEditRealDelegate(this);
    CComboBoxDelegate *comboBoxDelegate = new CComboBoxDelegate(this);

    m_pSqlTableModel = new CSqlTableModel(this);
    m_pSqlTableModel->setTable("defend");
    m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    readData();

    m_pDefendReport = new CReportView(this);
    m_pDefendReport->setModel(m_pSqlTableModel);
    m_pDefendReport->horizontalHeader()->setResizeMode(QHeaderView::Stretch); // 设置每行内容自动适应宽度
    m_pDefendReport->horizontalHeader()->setStyleSheet("background-color: #4378ac;color: white;");  // bug 不明

    m_pDefendReport->setItemDelegateForColumn(CHANNEL_E,lineEditDelegate);
    m_pDefendReport->setItemDelegateForColumn(THRESHOLD_E,lineEditRealDelegate);
    m_pDefendReport->setItemDelegateForColumn(DELAY_E,lineEditDelegate);
    m_pDefendReport->setItemDelegateForColumn(SWITCH_E,comboBoxDelegate);
    m_pDefendReport->setItemDelegateForColumn(SUPPLY_SWITCH_E,comboBoxDelegate);

    m_pReadBtn = new QPushButton(tr("读取"),this);
    m_pSaveBtn = new QPushButton(tr("保存"),this);

    layout_();
}

void CDefendWidget::layout_()
{
    m_pDefendReport->setGeometry(DEFEND_REPORT_L,DEFEND_REPORT_T,
                                 DEFEND_REPORT_W,DEFEND_REPORT_H);

    m_pReadBtn->setGeometry(SYSTEM_READ_BTN_L,SYSTEM_READ_BTN_T,
                            SYSTEM_READ_BTN_W,SYSTEM_READ_BTN_H);
    m_pSaveBtn->setGeometry(SYSTEM_SAVE_BTN_L,SYSTEM_SAVE_BTN_T,
                            SYSTEM_SAVE_BTN_W,SYSTEM_SAVE_BTN_H);
}

void CDefendWidget::localConnect()
{
    connect(m_pReadBtn,SIGNAL(clicked()),this,SLOT(readData()));
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(saveData()));
}
