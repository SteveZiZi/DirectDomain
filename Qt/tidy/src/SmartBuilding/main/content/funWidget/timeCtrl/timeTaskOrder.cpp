#include "timeTaskOrder.h"

#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QIntValidator>

CTimeTaskOrder::CTimeTaskOrder(QWidget *parent)
: CBaseOrderWidget(parent)
{
    init_();
}

CTimeTaskOrder::~CTimeTaskOrder()
{

}

S_BYTE CTimeTaskOrder::hour()
{
    return m_pHourEdit->text().toUInt();
}

void CTimeTaskOrder::setHour(S_BYTE hour_)
{
    m_pHourEdit->setText(QString::number(hour_));
}

S_BYTE CTimeTaskOrder::minute()
{
    return m_pMinEdit->text().toUInt();
}

void CTimeTaskOrder::setMinute(S_BYTE minute_)
{
    m_pMinEdit->setText(QString::number(minute_));
}

void CTimeTaskOrder::init_()
{
    m_pHourEdit = new QLineEdit(this);
    m_pMinEdit = new QLineEdit(this);
    m_pHourEdit->setFixedSize(WIDGET_WIDTH_W / 3,WIDGET_HEIGHT_H);
    m_pMinEdit->setFixedSize(WIDGET_WIDTH_W / 3,WIDGET_HEIGHT_H);
    m_pHourEdit->setValidator(new QIntValidator(0,23,this));
    m_pMinEdit->setValidator(new QIntValidator(0,59,this));

    layout_();
}

void CTimeTaskOrder::layout_()
{
    QLabel* hourL = new QLabel(tr(" Ê±  "),this);
    QLabel* minL = new QLabel(tr(" ·Ö "),this);
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_pHourEdit);
    mainLayout->addWidget(hourL);
    mainLayout->addWidget(m_pMinEdit);
    mainLayout->addWidget(minL);
    mainLayout->addWidget(m_pOrderCom);
    mainLayout->addStretch();
    mainLayout->addWidget(m_pDelBtn);

    this->setLayout(mainLayout);
}
