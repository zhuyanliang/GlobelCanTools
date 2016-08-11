#include "TableView2Excel.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QCoreApplication>


TableView2Excel::TableView2Excel(QObject *parent) :
    QObject(parent)
{
}

bool TableView2Excel::ExportToExcel(QTableView *tableView,QStandardItemModel *model)
{
    int tableR = model->rowCount();
    int tableC = model->columnCount();

    QProgressBar probar;
    probar.setVisible(true);
    probar.setValue(0);
    probar.setMinimum(0);
    probar.setMaximum(tableR);
    probar.show();

    if ( NULL == tableView )
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","excelexport");
    if( !db.isValid())
    {
        qDebug() << "DB is inValid()";
        return false;   //! type error
    }

    QString dsn = "DRIVER={Microsoft Excel Driver (*.xls)};"
                "DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"test.xls\";DBQ=test.xls";
    db.setDatabaseName(dsn);
    if(!db.open())
    {
        qDebug() << "Open DB Error";
        QSqlDatabase::removeDatabase("excelexport");
    }

    QSqlQuery query(db);
    QString sSql = "";
    bool state;
    QString sheetName = "BMSData";
    sSql = QString("DROP TABLE [%1]").arg(sheetName);
    query.exec( sSql);

    //create the table (sheet in Excel file)
    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
    for(int i=0;i<model->columnCount()-1;i++)
    {
        sSql += "[" +model->horizontalHeaderItem(i)->text() + "] char(64),";
    }
    sSql += "[" +model->horizontalHeaderItem(model->columnCount()-1)->text() + "] char(64) " + ")";

    qDebug() << sSql;

    state = query.prepare( sSql);
    if( !query.exec())
    {
        qDebug() << "export2Excel failed: Create Excel sheet failed.";
        db.close();
        QSqlDatabase::removeDatabase("excelexport");
    }

    sSql = QString("INSERT INTO [%1] (").arg( sheetName);

    for(int i=0;i<model->columnCount()-1;i++)
    {
        sSql += model->horizontalHeaderItem(i)->text() + ",";
    }
    sSql += model->horizontalHeaderItem(model->columnCount()-1)->text() + ") VALUES(";
    for(int i=1;i<model->columnCount();i++)
        sSql += QString(" :data%1").arg(i) + ",";
    sSql += QString(" :data%1").arg(model->columnCount()) + ")";

    qDebug() << sSql;

    state = query.prepare( sSql);

    for(int i=0;i<tableR;i++)
    {
        for(int j=0;j<tableC;j++)
        {
            query.bindValue(QString(":data%1").arg(j+1), model->item(i,j)->text());
        }
        if( !query.exec())
        {
            qDebug() << "export2Excel failed: insert Rows " << i << " Error!";
             db.close();
             QSqlDatabase::removeDatabase("excelexport");
        }
        QCoreApplication::processEvents();
        probar.setValue(i);
    }

    db.close();
    QSqlDatabase::removeDatabase("excelexport");

    return true;
}

void TableView2Excel::printError( QSqlError error)
{
    QString sqlerr = error.text();
    qDebug( qPrintable(sqlerr));
}
