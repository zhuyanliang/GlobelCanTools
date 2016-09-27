#ifndef TABLEVIEW2EXCEL_H
#define TABLEVIEW2EXCEL_H

#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableView>
#include <QStandardItemModel>

class TableView2Excel : public QObject
{
    Q_OBJECT
public:
    explicit TableView2Excel(QStandardItemModel *mod,QObject *parent=0);
    ~TableView2Excel();
    bool ExportToExcel();
    void printError(QSqlError error);
    
signals:
    
public slots:
private:
    QSqlDatabase    db;
    QSqlQuery       *query;
    QString         sSql;
    QString         dsn;
    QString         sheetName;
    QStandardItemModel *model;
};

#endif // TABLEVIEW2EXCEL_H
