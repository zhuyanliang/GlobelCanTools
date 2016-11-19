#include "TableView2Excel.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QStandardItemModel>
#include <QCoreApplication>
#include <QProgressBar>

TableView2Excel::TableView2Excel(QStandardItemModel *mod,QObject *parent) :
    QObject(parent),sheetcnt(1),rowsCnt(0), model(mod)
{
    sSql = "";
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","excelexport");
    if( !db.isValid())
    {
        qDebug() << "DB is inValid()";
    }

    QString date = QDate::currentDate().toString("yyyyMMdd");
    QString time = QTime::currentTime().toString();
    time.remove(QChar(':'));
    time.trimmed();
    dsn = "DRIVER={Microsoft Excel Driver (*.xls)};"
                  "DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\""+ date + time + ".xls\";DBQ="
                   + date + time +".xls";

    db.setDatabaseName(dsn);
    if(!db.open())
    {
        qDebug() << "Open DB Error";
        QSqlDatabase::removeDatabase("excelexport");
    }

    query = new QSqlQuery(db);

    initSheet();
}

void TableView2Excel::initSheet()
{
    sheetName = "BMSData" + QString::number(sheetcnt);
    sheetcnt++;
    sSql = QString("DROP TABLE [%1]").arg(sheetName);
    query->exec(sSql);

    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);

    for(int i=0;i<model->columnCount()-1;i++)
    {
        sSql += "[" +model->horizontalHeaderItem(i)->text() + "] char(64),";
    }
    sSql += "[" +model->horizontalHeaderItem(model->columnCount()-1)->text() + "] char(64) " + ")";

    query->prepare( sSql);
    if( !query->exec())
    {
        qDebug() << "export2Excel failed: Create Excel sheet failed.";
        db.close();
        QSqlDatabase::removeDatabase("excelexport");
    }
}

TableView2Excel::~TableView2Excel()
{
    delete query;
    db.close();
    QSqlDatabase::removeDatabase("excelexport");
}

bool TableView2Excel::ExportToExcel()
{
    int tableR = model->rowCount();
    int tableC = model->columnCount();
    rowsCnt += tableR;

    if(rowsCnt > 65500)
    {
        rowsCnt = 0;
        initSheet();
    }

    sSql = QString("INSERT INTO [%1] (").arg(sheetName);

    for(int i=0;i<model->columnCount()-1;i++)
    {
        sSql += model->horizontalHeaderItem(i)->text() + ",";
    }
    sSql += model->horizontalHeaderItem(model->columnCount()-1)->text() + ") VALUES(";
    for(int i=1;i<model->columnCount();i++)
        sSql += QString(" :data%1").arg(i) + ",";
    sSql += QString(" :data%1").arg(model->columnCount()) + ")";

    query->prepare( sSql);

    for(int i=0;i<tableR;i++)
    {
        for(int j=0;j<tableC;j++)
        {
            query->bindValue(QString(":data%1").arg(j+1), model->item(i,j)->text());
        }
        if( !query->exec())
        {
            qDebug() << "export2Excel failed: insert Rows " << i << " Error!";
             db.close();
             QSqlDatabase::removeDatabase("excelexport");
        }
        QCoreApplication::processEvents();
    }

    return true;
}

