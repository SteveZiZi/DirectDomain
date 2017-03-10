#include "circuitSelection.h"

#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>

#include "src/SmartBuilding/dataLevel/dataMgr.h"

#define MAX_CIRCUIT_COUNT           16
#define WIDGET_HEIGHT_H             30

CCircuitSelection::CCircuitSelection(QWidget *parent)
: QWidget(parent)
{
    init_();
}

CCircuitSelection::~CCircuitSelection()
{

}

void CCircuitSelection::setCircuitSelection(const S_WORD& circuitSelection)
{
    for(int i = 0;i < MAX_CIRCUIT_COUNT;i++)
    {
        if((1 << i) & circuitSelection)
            m_lCircuitCheckList[i]->setChecked(true);
    }
}

S_WORD CCircuitSelection::getCircuitSelection()
{
    S_WORD circuitSelection = 0;
    for(int i = 0; i < MAX_CIRCUIT_COUNT;i++)
    {
        if(m_lCircuitCheckList[i]->isChecked())
            circuitSelection |= 1 << (i);
    }

    return circuitSelection;
}

S_BYTE CCircuitSelection::getCircuitCount()
{
    S_BYTE circuitCount = 0;
    S_WORD circuitSelection = getCircuitSelection();
    while(circuitSelection){                                                                         // 获取回路个数
        circuitSelection = circuitSelection & (circuitSelection - 1);
        circuitCount++;
    }

    return circuitCount;
}

void CCircuitSelection::getCircuitList(QList<int>& circuitList)
{
    circuitList.clear();

    S_WORD circuitSelection = getCircuitSelection();
    for (int i = 1;i <= 16;i++)
    {
        if (circuitSelection & (0x0001 << (i - 1)))
            circuitList.push_back(i);
    }
//     if(circuitSelection & 0x0001)
//         circuitList.push_back(1);
//     if(circuitSelection & 0x0002)
//         circuitList.push_back(2);
//     if(circuitSelection & 0x0004)
//         circuitList.push_back(3);
//     if(circuitSelection & 0x0008)
//         circuitList.push_back(4);
//     if(circuitSelection & 0x0010)
//         circuitList.push_back(5);
//     if(circuitSelection & 0x0020)
//         circuitList.push_back(6);
//     if(circuitSelection & 0x0040)
//         circuitList.push_back(7);
//     if(circuitSelection & 0x0080)
//         circuitList.push_back(8);
//     if(circuitSelection & 0x0100)
//         circuitList.push_back(9);
//     if(circuitSelection & 0x0200)
//         circuitList.push_back(10);
//     if(circuitSelection & 0x0400)
//         circuitList.push_back(11);
//     if(circuitSelection & 0x0800)
//         circuitList.push_back(12);
//     if(circuitSelection & 0x1000)
//         circuitList.push_back(13);
//     if(circuitSelection & 0x2000)
//         circuitList.push_back(14);
//     if(circuitSelection & 0x4000)
//         circuitList.push_back(15);
//     if(circuitSelection & 0x8000)
//         circuitList.push_back(16);
}

void CCircuitSelection::setCircuitEnable(int index,bool enable)
{
    Q_ASSERT(index >= 0 && index < m_lCircuitCheckList.size());

    m_lCircuitCheckList[index]->setEnabled(enable);
}

void CCircuitSelection::init_()
{
    for (int i = 0;i < MAX_CIRCUIT_COUNT;i++)
    {
        QCheckBox* circuitCheck = new QCheckBox;
        circuitCheck->setText(QString("L#%1%2")
                    .arg(CDataMgr::getInstance()->getCPCNo())
                    .arg(i + 1, 2, 10, QChar('0')));
        circuitCheck->setFixedHeight(WIDGET_HEIGHT_H);
        m_lCircuitCheckList.append(circuitCheck);
    }

    layout_();
}

void CCircuitSelection::layout_()
{
    QGridLayout* gridLayout = new QGridLayout;
    for (int row = 0;row < (MAX_CIRCUIT_COUNT / 4);row++)
    {
        for (int column = 0;column < (MAX_CIRCUIT_COUNT / 4);column++)
        {
            gridLayout->addWidget(m_lCircuitCheckList[row * 4 +column],row,column);
        }
    }

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(new QLabel(tr("回路选择 :")));
    mainLayout->addLayout(gridLayout);
    this->setLayout(mainLayout);
}
