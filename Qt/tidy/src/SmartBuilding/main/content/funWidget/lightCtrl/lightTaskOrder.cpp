#include "lightTaskOrder.h"

#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QRegExpValidator>

CLightTaskOrder::CLightTaskOrder(QWidget *parent)
: CBaseOrderWidget(parent)
{
    init_();
}

CLightTaskOrder::~CLightTaskOrder()
{

}

S_DWORD CLightTaskOrder::durationTime()
{
    return m_pDurationEdit->text().toUInt();
}

void CLightTaskOrder::setDurationTime(S_DWORD durationTime_)
{
    m_pDurationEdit->setText(QString::number(durationTime_));
}

void CLightTaskOrder::init_()
{
    m_pDurationEdit = new QLineEdit(this);
    m_pDurationEdit->setFixedSize(WIDGET_WIDTH_W - 20,WIDGET_HEIGHT_H);
    QRegExp regExp("[0-9]{0,3}");
    m_pDurationEdit->setValidator(new QRegExpValidator(regExp,this));

    layout_();
}

void CLightTaskOrder::layout_()
{
    QLabel* durationL = new QLabel(tr(" Ê±³¤ "),this);
    QLabel* minLabel = new QLabel(tr("·Ö  "),this);
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(durationL);
    mainLayout->addWidget(m_pDurationEdit);
    mainLayout->addWidget(minLabel);
    mainLayout->addWidget(m_pOrderCom);
    mainLayout->addStretch();
    mainLayout->addWidget(m_pDelBtn);

    this->setLayout(mainLayout);
}