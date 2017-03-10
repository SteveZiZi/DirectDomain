/****************************************************************************
**
** file: database.h
**
** description:
**     ���ݿ������
**
** change logs:
**      2016/12/12:   ShengZeMa   create file
**
****************************************************************************/
#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>
#include <QSqlError>
#include <QDebug>
#include <QString>

#define MAX_LINE_COUNT              16

bool createConnection(QString& strCpcNo)
{
    QString sql;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if(!db.open())
        return false;

    QSqlQuery query; // ����Dim��
    query.exec("CREATE TABLE dim("
        "circuit varchar primary key,"
        "precentHundred integer,"
        "precentNinty integer,"
        "precentEighty integer,"
        "precentSeventy integer,"
        "precentSixty integer,"
        "precentFifty integer,"
        "precentForty integer,"
        "precentThirty integer,"
        "precentTwenty integer,"
        "precentTen integer,"
        "precentZero integer)");
    for(int i = 1;i <= MAX_LINE_COUNT;i++)
    {
        sql.clear();
        sql = "INSERT INTO dim(circuit,precentHundred,precentNinty,precentEighty,precentSeventy,"
              "precentSixty,precentFifty,precentForty,precentThirty,precentTwenty,precentTen,precentZero) VALUES (\'";
        sql += strCpcNo;
        if(i < 10)
            sql += "0";
        sql += QString::number(i);
        sql += "\',300,289,278,267,256,245,234,223,212,201,190)";
        query.exec(sql);
    }
    query.next(); // �����..

    // �����ڶ������ݿ�
    QSqlDatabase db2 = QSqlDatabase::addDatabase("QSQLITE","db2");
    db2.setDatabaseName("database.db");
    if(!db2.open())
        return false;
    QSqlQuery query2; // ����©�翪�ر�
    query2.exec("CREATE TABLE defend("
        "circuit varchar primary key,"
        "channel integer,"
        "threshold double,"
        "delayTime integer,"
        "switch varchar,"
        "execute varchar)");
    for(int i = 1;i <= MAX_LINE_COUNT;i++)
    {
        sql.clear();
        sql = "INSERT INTO defend(circuit,channel,threshold,delayTime,switch,execute) VALUES (\'";
        sql += strCpcNo;
        if(i < 10)
            sql += "0";
        sql += QString::number(i);
        sql += "\',";
        sql += QString::number(i);
        sql += QObject::tr(",3,10,\'�ر�\',\'�ر�\')");
        query2.exec(sql);
    }
    query2.next(); // �����..

    // �������������ݿ� ����·�ѱ�ѡ����selected����Ϊ1��belong�У�ʱ��Ϊ1�����Ϊ2������Ϊ3
    QSqlDatabase db3 = QSqlDatabase::addDatabase("QSQLITE","db3");
    db3.setDatabaseName("database.db");
    if(!db3.open())
        return false;
    QSqlQuery query3; // ������·��ѡ��
    query3.exec("CREATE TABLE line("
        "circuit varchar primary key,"
        "selected integer,"
        "belong interger)");
    for(int i = 1;i <= MAX_LINE_COUNT;i++)
    {
        sql.clear();
        sql = "INSERT INTO line(circuit,selected,belong) VALUES (\'";
        sql += strCpcNo;
        if(i < 10)
            sql += "0";
        sql += QString::number(i);
        sql += "\',0,0)";
        query3.exec(sql);
    }

    query3.next(); // �����..

    return true;
}

#endif // DATABASE_H
