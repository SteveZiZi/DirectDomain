/****************************************************************************
**
** file: colorWidget.h
**
** description:
**     部署界面最小单元
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QtGui/QWidget>

#define COLOR_WIDGET_W          175
#define COLOR_WIDGET_H          80
#define CIRCUIT_NO_LABEL_L      67
#define CIRCUIT_NO_LABEL_T      15
#define RECTIFIER_NO_LABEL_L    67
#define RECTIFIER_NO_LABEL_T    45

class CColorWidgetInfo
{
public:
    CColorWidgetInfo();
    ~CColorWidgetInfo();

    void setCircuitNo(int circuitNo_) {m_iCircuitNo = circuitNo_;}
    void setDeviceNo(int deviceNo_) {m_iDeviceNo = deviceNo_;}
    int circuitNo() {return m_iCircuitNo;}
    int deviceNo() {return m_iDeviceNo;}

private:
    int                   m_iCircuitNo;
    int                   m_iDeviceNo;
};

class QLabel;
class QColor;
class CColorWidget : public QWidget
{
    Q_OBJECT

public:
    CColorWidget(QWidget *parent = 0);
    ~CColorWidget();

    void setCircuitNo(int circuitNo);
    void setRectiFierNo(int rectifierNo);
    void setColor(const QString& color);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void init_();
    void layout_();

private:
    QLabel*                 m_pCircuitNo;
    QLabel*                 m_pRectiFierNo;
    QColor                  m_cColor;
};

#endif // COLORWIDGET_H
