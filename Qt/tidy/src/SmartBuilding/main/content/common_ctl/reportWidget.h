/****************************************************************************
**
** file: reportWidget.h
**
** description:
**     报告控件，可用于日志、运行数据等界面.
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
     *  @Desc:      创建报表
     *  @PaamIn:    
     *      row:行数  column 列数
     *  @Return:
     *      none
     */
    void createReportWidget(int row,int column);
    /*
     *  @Func:      showHorizontalHeader
     *  @Desc:      设置横表头是否显示
     *  @PaamIn:    
     *      flag: 是否显示
     *  @Return:
     *      none
     */
    void showHorizontalHeader(bool flag);
    /*
     *  @Func:      setHeaderContent
     *  @Desc:      设置横表头内容
     *  @PaamIn:    
     *      header: 表头内容
     *  @Return:
     *      none
     */
    void setHeaderContent(QStringList& header);
    /*
     *  @Func:      setColumnColor
     *  @Desc:      设置列的颜色
     *  @PaamIn:    
     *      column: 列索引 color: 颜色
     *  @Return:
     *      none
     */
    void setColumnColor(int column,const QColor& color);
    /*
     *  @Func:      setColumnAlignment
     *  @Desc:      设置列的显示位置
     *  @PaamIn:    
     *      column: 列索引 color: alignment 显示位置
     *  @Return:
     *      none
     */
    void setColumnAlignment(int column,int alignment);
    /*
     *  @Func:      setContentHeight
     *  @Desc:      设置内容的行高
     *  @PaamIn:    
     *      hight: 行高
     *  @Return:
     *      none
     */
    void setContentHeight(int hight);
    /*
     *  @Func:      clearReportContent
     *  @Desc:      清理报表内容
     *  @PaamIn:    none
     *  @Return:
     *      none
     */
    void clearReportContent();
    /*
     *  @Func:      addOneRow
     *  @Desc:      增加一行
     *  @PaamIn:    none
     *  @Return:
     *      none
     */
    bool addOneRow();
    /*
     *  @Func:      delOneRow
     *  @Desc:      删除一行
     *  @PaamIn:    row 行数
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
