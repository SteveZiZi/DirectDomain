#include "colorWidget.h"

#include <QLabel>
#include <QPainter>

CColorWidgetInfo::CColorWidgetInfo()
{

}

CColorWidgetInfo::~CColorWidgetInfo()
{

}

CColorWidget::CColorWidget(QWidget *parent)
: QWidget(parent)
, m_cColor("gray")
{
    init_();
}

CColorWidget::~CColorWidget()
{

}

void CColorWidget::setCircuitNo(int circuitNo)
{
    m_pCircuitNo->setText(QString("L#%1").arg(circuitNo, 2, 10, QChar('0')));
}

void CColorWidget::setRectiFierNo(int rectifierNo)
{
    m_pRectiFierNo->setText(QString("R#%1").arg(rectifierNo, 2, 10, QChar('0')));
}

void CColorWidget::setColor(const QString& color)
{
    m_cColor.setNamedColor(color);
}

void CColorWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(m_cColor);
    QPen pen(m_cColor);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(this->rect());
}

void CColorWidget::init_()
{
    m_pCircuitNo = new QLabel("L#XX",this);
    m_pRectiFierNo = new QLabel("R#XX",this);

    layout_();
}

void CColorWidget::layout_()
{
    m_pCircuitNo->move(CIRCUIT_NO_LABEL_L,CIRCUIT_NO_LABEL_T);
    m_pRectiFierNo->move(RECTIFIER_NO_LABEL_L,RECTIFIER_NO_LABEL_T);

    this->resize(COLOR_WIDGET_W,COLOR_WIDGET_H);
}
