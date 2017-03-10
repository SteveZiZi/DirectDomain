#include "baseOrderWidget.h"

#include <QPushButton>
#include <QComboBox>

CBaseOrderWidget::CBaseOrderWidget(QWidget *parent)
: QWidget(parent)
{
    init_();
    localConnect();
}

CBaseOrderWidget::~CBaseOrderWidget()
{

}

S_BYTE CBaseOrderWidget::orderIndex()
{
    return m_pOrderCom->currentIndex();
}

void CBaseOrderWidget::setOrder(S_BYTE orderIndex_)
{
    m_pOrderCom->setCurrentIndex(orderIndex_);
}

void CBaseOrderWidget::init_()
{
    m_pDelBtn = new QPushButton(tr("É¾³ý"),this);
    m_pDelBtn->setFixedSize(WIDGET_HEIGHT_H + 20,WIDGET_HEIGHT_H);
    m_pDelBtn->setObjectName("orderDelBtn");
    m_pOrderCom = new QComboBox(this);
    m_pOrderCom->setFixedSize(80,WIDGET_HEIGHT_H);

    QStringList orders;
    orders << "100%" << "90%" << "80%" << "70%" << "60%"
           << "50%" << "40%" << "30%" << "20%" << "10%" << "0 %" << "OFF";
    m_pOrderCom->addItems(orders);
}

void CBaseOrderWidget::delSelf()
{
    emit orderDeled();
    this->hide();
}

void CBaseOrderWidget::localConnect()
{
    connect(m_pDelBtn,SIGNAL(clicked()),this,SLOT(delSelf()));
}
