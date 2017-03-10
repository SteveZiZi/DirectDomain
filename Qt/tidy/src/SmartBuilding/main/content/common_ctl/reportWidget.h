/****************************************************************************
**
** file: reportWidget.h
**
** description:
**     ����ؼ�����������־���������ݵȽ���.
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QtGui/QTableWidget>

class CReportWidget : public QTableWidget
{
    Q_OBJECT

public:
    CReportWidget(QWidget *parent = 0);
    ~CReportWidget();

    /*
     *  @Func:      createReportWidget
     *  @Desc:      ��������
     *  @PaamIn:    
     *      row:����  column ����
     *  @Return:
     *      none
     */
    void createReportWidget(int row,int column);
    /*
     *  @Func:      showHorizontalHeader
     *  @Desc:      ���ú��ͷ�Ƿ���ʾ
     *  @PaamIn:    
     *      flag: �Ƿ���ʾ
     *  @Return:
     *      none
     */
    void showHorizontalHeader(bool flag);
    /*
     *  @Func:      setHeaderContent
     *  @Desc:      ���ú��ͷ����
     *  @PaamIn:    
     *      header: ��ͷ����
     *  @Return:
     *      none
     */
    void setHeaderContent(QStringList& header);
    /*
     *  @Func:      setColumnColor
     *  @Desc:      �����е���ɫ
     *  @PaamIn:    
     *      column: ������ color: ��ɫ
     *  @Return:
     *      none
     */
    void setColumnColor(int column,const QColor& color);
    /*
     *  @Func:      setColumnAlignment
     *  @Desc:      �����е���ʾλ��
     *  @PaamIn:    
     *      column: ������ color: alignment ��ʾλ��
     *  @Return:
     *      none
     */
    void setColumnAlignment(int column,int alignment);
    /*
     *  @Func:      setContentHeight
     *  @Desc:      �������ݵ��и�
     *  @PaamIn:    
     *      hight: �и�
     *  @Return:
     *      none
     */
    void setContentHeight(int hight);
    /*
     *  @Func:      clearReportContent
     *  @Desc:      ����������
     *  @PaamIn:    none
     *  @Return:
     *      none
     */
    void clearReportContent();
    /*
     *  @Func:      addOneRow
     *  @Desc:      ����һ��
     *  @PaamIn:    none
     *  @Return:
     *      none
     */
    bool addOneRow();
    /*
     *  @Func:      delOneRow
     *  @Desc:      ɾ��һ��
     *  @PaamIn:    row ����
     *  @Return:
     *      none
     */
    bool delOneRow(int row);

    void setSelfModel(QAbstractItemModel *model);
public slots:

private:
    void init_();

protected:
    int             m_iRrowH;
};

#endif // REPORTWIDGET_H
