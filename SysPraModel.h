#ifndef SYSPRAMODEL_H
#define SYSPRAMODEL_H

#include <QWidget>
#include "DataProcess.h"
#include <QTimer>

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

private slots:
    void onTime(void);

private:
    QTimer *m_timer;
    QStandardItemModel *m_modelSysPra;
    DataProcess* m_dataProcess;

    Ui::SysPraModel *ui;
};

#endif // SYSPRAMODEL_H
