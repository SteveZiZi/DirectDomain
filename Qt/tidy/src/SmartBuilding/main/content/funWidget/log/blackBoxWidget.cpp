#include "blackBoxWidget.h"

#include <QPushButton>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/main/content/common_ctl/logReportWidget.h"
#include "src/SmartBuilding/main/content/common_ctl/popInputPwd.h"
#include "src/SmartBuilding/main/content/common_ctl/popSureWidget.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"

CBlackBoxW::CBlackBoxW(QWidget *parent)
: QWidget(parent)
, m_bDealing(false)
{
    init_();
    localConnect();
}

CBlackBoxW::~CBlackBoxW()
{

}

void CBlackBoxW::clearLog()
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
            INT8U databuf[4] = {0};
            CDataMgr::getInstance()->sendCommands(CLEAR_BLACK_BOX_CMD,databuf);
            m_pLogReportW->clearLogContent();
        }
        else
        {
            popEnsureDlg(tr("ÌáÊ¾"),tr("ÃÜÂë´íÎó£¡"));
        }
    }

    m_bDealing = false;
}

void CBlackBoxW::init_()
{
    m_pLogReportW = new CLogReportW(CLogReportW::BLACKBOX_TYPE,this);
    m_pClearLogBtn = new QPushButton(tr("Çå³ý"),this);

    layout_();
}

void CBlackBoxW::layout_()
{
    m_pClearLogBtn->setGeometry(LOG_FLUSH_BTN_L - 150,LOG_FLUSH_BTN_T,
                                LOG_FLUSH_BTN_W,LOG_FLUSH_BTN_H);
}

void CBlackBoxW::localConnect()
{
    connect(m_pClearLogBtn,SIGNAL(clicked()),this,SLOT(clearLog()));
}

