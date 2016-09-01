#ifndef SYSPRAMODEL_H
#define SYSPRAMODEL_H

#include <QWidget>
#include "DataProcess.h"
#include <QTimer>
#include "CanBus.h"
#include "XmlConfig.h"

namespace Ui {
class SysPraModel;
}

class QStandardItemModel;

class SysPraModel : public QWidget
{
    Q_OBJECT
    
public:
    explicit SysPraModel(DataProcess *datpress,QWidget *parent = 0);
    ~SysPraModel();
    
    void InitUI(void);
    char getStatus(){return status;}

private slots:
    void onTime(void);

    void on_pushButtonWritePra_clicked();

    void on_pushButtonImportConfig_clicked();

private:
    Ui::SysPraModel *ui;
    QTimer *m_timer;
    QStandardItemModel *m_modelSysPra;
    QStandardItemModel *m_modelSysSetPra;
    DataProcess* m_dataProcess;
    CanBus* m_can;
    XmlConfig *xmlconfig;
    char status;
};

#endif // SYSPRAMODEL_H
