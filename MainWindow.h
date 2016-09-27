#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
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

//系统的报警信息类型
typedef union
{
    uint32_t all;

    struct
    {
        unsigned COV :2;        // 单体电池过压
        unsigned CUV :2;        // 单体电池欠压
        unsigned COT :2;		// 单体充电高温
        unsigned CUT :2;		// 单体充电低温
        unsigned DOT :2;		// 单体放电高温
        unsigned DUT :2;		// 单体放电低温
        unsigned COC :2;		// 充电过流
        unsigned DOC :2;		// 放电过流
        unsigned CIB :2;     	// 单体电池一致性
        unsigned PCBOT :2;    	// PCB板高温
        unsigned PCBUT :2;    	// PCB板低温
        unsigned POV :2;		// 整包总压高
        unsigned PUV :2;		// 整包总压低
        unsigned TIB :2;		// 电池温差大
        unsigned ISO :2;		// 绝缘警告
        unsigned     :2;
    };
}SysWarningDef;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTimer *m_timer;
    BatterySoc* m_batterySoc;
    VoltageUI* m_voltageui;
    AmpereMeter * ampereMeter;
    IndictorLight* m_indictor_light[IndictorLightNum];
    QLabel *m_indictor_light_label[IndictorLightNum];
    IndictorLight *m_devCanLight;
    CellMinMaxInfo* m_cellMinMaxInfoUi;
    DevSetDialog* m_devsetdlg;
    DataProcess* m_dataProcess;
    SysPraModel* m_sysPraModel;
    QStandardItemModel *model;
    QLabel *statusLabel;
    QStandardItemModel *modelTestData;

protected:
     void closeEvent(QCloseEvent *event);
public:
    void initUI();
    explicit MainWindow(QWidget *parent = 0);

    void initModel();
    void initTestDataModel();

    void setWarnRadioButton(int level,QRadioButton *radio1,QRadioButton *radio2);

    ~MainWindow();
signals:
    void   openButtonClicked();
private slots:
    void timeUpdate();

    void on_actionSet_triggered();

    void on_pushButtonOpen_clicked();

    void on_actionBoardInfo_triggered();

    void dataReceived(VCI_CAN_OBJ &dat);

    void on_pushButtonWritePrograme_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
