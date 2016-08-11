#ifndef TABLEVIEW2EXCEL_H
#define TABLEVIEW2EXCEL_H

#include <QObject>
#include <QSqlError>
#include <QTableView>
#include <QStandardItemModel>

class TableView2Excel : public QObject
{
    Q_OBJECT
public:
    explicit TableView2Excel(QObject *parent=0);
    bool ExportToExcel(QTableView *tableView,QStandardItemModel *model);
    void printError(QSqlError error);
    
signals:
    
public slots:
private:
    
};

#endif // TABLEVIEW2EXCEL_H
