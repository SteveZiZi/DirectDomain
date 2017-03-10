#include "disposeWidget.h"

#include <QList>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>

#include "src/SmartBuilding/inc/xtDef.h"
#include "src/SmartBuilding/dataLevel/dataMgr.h"
#include "src/SmartBuilding/dataLevel/cabinet.h"
#include "src/SmartBuilding/dataLevel/circuit.h"
#include "src/SmartBuilding/dataLevel/rectifier.h"
#include "src/SmartBuilding/main/content/funWidget/config/colorWidget.h"

#define MAX_RECTIFIER_COUNT         16

const QString colorArr[MAX_RECTIFIER_COUNT] = 
{
    "green",
    "blue",
    "red",
    "violet",
    "cyan",
    "silver",
    "pink",
    "khaki",
    "brown",
    "black",
    "olive",
    "lime",
    "gold",
    "darkviolet",
    "darkcyan",
    "coral"
};

bool compareColorInfo(CColorWidgetInfo& color1,CColorWidgetInfo& color2)
{
    if (color1.deviceNo() < color2.deviceNo())
        return true;
    else
        return false;
}

CDisposeWidget::CDisposeWidget(QWidget *parent)
: QWidget(parent)
, m_pDataMgr(CDataMgr::getInstance())
{
    init_();
    localConnect();
}

CDisposeWidget::~CDisposeWidget()
{

}

void CDisposeWidget::flushDispos()
{
    QList<CColorWidgetInfo> colorWidgetInfoList;

    CCabinet *cabinet = m_pDataMgr->getIpcData();
    for (int circuitNo = 0;circuitNo < cabinet->circuitCount();circuitNo++)
    {
        CCircuit* circuit = cabinet->findCircuit(circuitNo);
        for (int rectifierNo = 0;rectifierNo < circuit->rectifierCount();rectifierNo++)
        {
            CRectifier* rectifier = circuit->findRectifier(rectifierNo);
            CColorWidgetInfo colorInfo;
            colorInfo.setCircuitNo(circuit->circuitNo());
            colorInfo.setDeviceNo(rectifier->deviceNo());
            colorWidgetInfoList.append(colorInfo);
        }
    }

    qSort(colorWidgetInfoList.begin(),colorWidgetInfoList.end(),compareColorInfo);

    int count = (MAX_RECTIFIER_COUNT > colorWidgetInfoList.size()) ? colorWidgetInfoList.size() : MAX_RECTIFIER_COUNT;

    for (int i = 0;i < count;i++)
    {
        m_lColorWidgetList[i]->setCircuitNo(colorWidgetInfoList[i].circuitNo());
        m_lColorWidgetList[i]->setRectiFierNo(colorWidgetInfoList[i].deviceNo());
        m_lColorWidgetList[i]->setColor(colorArr[colorWidgetInfoList[i].circuitNo() - 1]);
        m_lColorWidgetList[i]->update();
    }
}

void CDisposeWidget::init_()
{
    m_pFrame = new QFrame(this);
    m_pFlushBtn = new QPushButton(tr("Ë¢ÐÂ"),this);

    for (int i = 0;i < MAX_RECTIFIER_COUNT;i++){
        CColorWidget* colorWidget = new CColorWidget;
        m_lColorWidgetList.append(colorWidget);
    }

    layout_();
}

void CDisposeWidget::layout_()
{
    m_pFrame->setGeometry(CONFIG_FRAME_L,CONFIG_FRAME_T,
                          CONFIG_FRAME_W,CONFIG_FRAME_H);
    m_pFlushBtn->setGeometry(CONFIG_FLUSH_BTN_L,CONFIG_FLUSH_BTN_T,
                             CONFIG_FLUSH_BTN_W,CONFIG_FLUSH_BTN_H);
    QGridLayout* mainLayout = new QGridLayout;
    for (int row = 0;row < (MAX_RECTIFIER_COUNT / 4);row++){
        for (int column = 0;column < (MAX_RECTIFIER_COUNT / 4);column++){
            mainLayout->addWidget(m_lColorWidgetList[row * 4 +column],row,column);
        }
    }

    m_pFrame->setLayout(mainLayout);
}

void CDisposeWidget::localConnect()
{
    connect(m_pFlushBtn,SIGNAL(clicked()),this,SLOT(flushDispos()));
}