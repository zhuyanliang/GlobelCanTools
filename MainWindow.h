#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DataProcess.h"
#include <QCloseEvent>

#define IndictorLightNum 5 //通信指示灯数量

class QHBoxLayout;
class QVBoxLayout;
class VoltageUI;
class CellMinMaxInfo;
class AmpereMeter;
class BatterySoc;
class IndictorLight;
class QLabel;
class DevSetDialog;
class QStandardItemModel;
class ExcelEngine;
class SysPraModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTimer *m_timer;
    QTimer *m_timerSysPra;
    BatterySoc* m_batterySoc;
    VoltageUI* m_voltageui;
    AmpereMeter * ampereMeter;
    IndictorLight* m_indictor_light[IndictorLightNum];
    QLabel *m_indictor_light_label[IndictorLightNum];
    IndictorLight *m_devCanLight;
    CellMinMaxInfo* m_cellMinMaxInfoUi;
    DevSetDialog* m_devsetdlg;
    DataProcess* m_dataProcess;
    SysPraModel* sysPraModel;

    QStandardItemModel *model;

protected:
     void closeEvent(QCloseEvent *event);
public:
    void initUI();
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
signals:
    void   openButtonClicked();
private slots:
    void TimeUpdate();
    void on_actionSet_triggered();

    void on_pushButtonOpen_clicked();

    void on_actionBoardInfo_triggered();

    void dataReceived(VCI_CAN_OBJ &dat);

    void on_pushButtonOutputData_clicked();

    void on_pushButtonWritePrograme_clicked();

    void on_pushButtonWritePra_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
