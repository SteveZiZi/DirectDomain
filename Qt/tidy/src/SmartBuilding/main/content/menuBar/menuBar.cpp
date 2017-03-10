#include "menuBar.h"

#include <QToolButton>
#include <QVBoxLayout>

CMenuBar::CMenuBar(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();
}

CMenuBar::~CMenuBar()
{

}

void CMenuBar::updateMenus()
{
    for(int i = 0;i < m_lMenuBtn.size();i++){
        m_lMenuBtn[i]->setChecked(false);
    }

    QToolButton* toolBtn = dynamic_cast<QToolButton*>(sender());
    toolBtn->setChecked(true);
    int item = findBtn(toolBtn);
    if(-1 != item)
        emit menuSwitched(item);
}

void CMenuBar::init_()
{
    for (int i = 0;i < FUN_COUNT;i++){
        QToolButton* toolBtn = new QToolButton;
        toolBtn->setCheckable(true);
        toolBtn->setObjectName(QString("menuBtn_%1").arg(QString::number(i + 1)));
        toolBtn->setFixedSize(MENU_BTN_WIDTH,MENU_BTN_HEIGHT);
        m_lMenuBtn.append(toolBtn);
    }

    layout_();
    m_lMenuBtn[0]->setChecked(true);
}

void CMenuBar::layout_()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    for (int i = 0;i < m_lMenuBtn.size();i++){
        mainLayout->addWidget(m_lMenuBtn[i]);
    }
    setLayout(mainLayout);
}

void CMenuBar::localConnect()
{
    Q_FOREACH(QToolButton* btn,m_lMenuBtn){
        connect(btn,SIGNAL(clicked()),this,SLOT(updateMenus()));
    }
}

int CMenuBar::findBtn(QToolButton* btn)
{
    int item = -1;
    for(int i = 0;i < m_lMenuBtn.size();i++){
        if(btn->objectName() == m_lMenuBtn[i]->objectName()){
            item = i;
            break;
        }
    }

    return item;
}