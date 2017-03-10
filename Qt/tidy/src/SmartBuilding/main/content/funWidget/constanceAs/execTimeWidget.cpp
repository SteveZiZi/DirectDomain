#include "execTimeWidget.h"

#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QIntValidator>

#define LINE_EDIT_H             30
#define LINE_EDIT_W             32

CExecTimeWidget::CExecTimeWidget(QWidget *parent)
: QWidget(parent)
{
    init_();
}

CExecTimeWidget::~CExecTimeWidget()
{

}

S_BYTE CExecTimeWidget::startHour() 
{
    return m_pStartHourEdit->text().toUInt();
}

void CExecTimeWidget::setStartHour(S_BYTE startHour_)
{
    m_pStartHourEdit->setText(QString::number(startHour_));
}

S_BYTE CExecTimeWidget::startMin()
{
    return m_pStartMinEdit->text().toUInt();
}

void CExecTimeWidget::setStartMin(S_BYTE startMin_)
{
    m_pStartMinEdit->setText(QString::number(startMin_));
}

S_BYTE CExecTimeWidget::endHour()
{
    return m_pEndHourEdit->text().toUInt();
}

void CExecTimeWidget::setEndHour(S_BYTE endHour_)
{
    m_pEndHourEdit->setText(QString::number(endHour_));
}

S_BYTE CExecTimeWidget::endMin()
{
    return m_pEndMinEdit->text().toUInt();
}

void CExecTimeWidget::setEndMin(S_BYTE endMin_)
{
    m_pEndMinEdit->setText(QString::number(endMin_));
}

void CExecTimeWidget::init_()
{
    QIntValidator* houtValid = new QIntValidator(0,23,this);
    QIntValidator* minValid = new QIntValidator(0,59,this);

    m_pStartHourEdit = new QLineEdit;
    m_pStartMinEdit = new QLineEdit;
    m_pEndHourEdit = new QLineEdit;
    m_pEndMinEdit = new QLineEdit;

    m_pStartHourEdit->setValidator(houtValid);
    m_pEndHourEdit->setValidator(houtValid);
    m_pStartMinEdit->setValidator(minValid);
    m_pEndMinEdit->setValidator(minValid);

    m_pStartHourEdit->setFixedSize(LINE_EDIT_W,LINE_EDIT_H);
    m_pStartMinEdit->setFixedSize(LINE_EDIT_W,LINE_EDIT_H);
    m_pEndHourEdit->setFixedSize(LINE_EDIT_W,LINE_EDIT_H);
    m_pEndMinEdit->setFixedSize(LINE_EDIT_W,LINE_EDIT_H);

    layout_();
}

void CExecTimeWidget::layout_()
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_pStartHourEdit);
    mainLayout->addWidget(new QLabel(tr("时")));
    mainLayout->addWidget(m_pStartMinEdit);
    mainLayout->addWidget(new QLabel(tr("分 ")));
    mainLayout->addWidget(m_pEndHourEdit);
    mainLayout->addWidget(new QLabel(tr("时")));
    mainLayout->addWidget(m_pEndMinEdit);
    mainLayout->addWidget(new QLabel(tr("分")));

    this->setLayout(mainLayout);
}

