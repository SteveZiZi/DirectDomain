/****************************************************************************
**
** file: blackBoxWidget.h
**
** description:
**     ��ϻ��
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef BLACKBOXWIDGET_H
#define BLACKBOXWIDGET_H

#include <QtGui/QWidget>

class CLogReportW;
class QPushButton;
class CBlackBoxW : public QWidget
{
    Q_OBJECT

public:
    CBlackBoxW(QWidget *parent = 0);
    ~CBlackBoxW();

private slots:
    void clearLog();

private:
    void init_();
    void layout_();
    void localConnect();

private:
    CLogReportW*                m_pLogReportW;
    QPushButton*                m_pClearLogBtn;

    bool                        m_bDealing;                             // �Ƿ�����ɾ����־
};

#endif // BLACKBOXWIDGET_H
