#include <QGridLayout>
#include <QLabel>
#include <QThread>
#include <QTime>
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "VoltageUI.h"
#include "CellMinMaxInfo.h"
#include "AmpereMeter.h"
#include "BatterySoc.h"
#include "IndictorLight.h"
#include "DevSetDialog.h"
#include "ControlCAN.h"
#include "ExcelEngine.h"
#include "TableView2Excel.h"
#include "DownLoadUi.h"
#include "SysPraModel.h"



const char* IndictorLabelText[IndictorLightNum] = {
    "控制器通信","BMS间通信","温度模块通信",
    "仪表通信","用户界面通信"
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    model(NULL),
    modelTestData(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();

    //打开关闭按钮信号
    connect(ui->pushButtonOpen,SIGNAL(clicked()),m_devsetdlg,SLOT(devSetButtonClicked()));
    //
    connect(m_dataProcess,SIGNAL(datRecSignal(VCI_CAN_OBJ&)),
            this,SLOT(dataReceived(VCI_CAN_OBJ&)),Qt::DirectConnection);

    //电池信息用的定时器
    m_timer = new QTimer(this);
    m_timer->setInterval(500);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    m_timer->start();
}

void MainWindow::initUI()
{
    statusLabel = new QLabel(this);
    ui->statusBar->addWidget(statusLabel);
    model = new QStandardItemModel();

    //CAN设备设置界面初始化
    m_devsetdlg = new DevSetDialog(this);
    m_devsetdlg->setWindowTitle(tr("选择打开CAN设备"));
    m_devsetdlg->resize(320,280);
    m_devsetdlg->setFixedSize(m_devsetdlg->width(),m_devsetdlg->height());
    m_devsetdlg->getValueFrmUi();//获取CAN的默认参数

    //绘制设备CAN打开指示灯
    m_devCanLight = new IndictorLight(this);
    m_devCanLight->move(20,20);
    m_devCanLight->resize(35,35);
    m_devCanLight->setColor(Qt::red);
    m_devCanLight->show();
    ui->labelDevStatus->move(58,6);

    ui->pushButtonOpen->resize(38,38);
    ui->pushButtonOpen->setFlat(true);
    //根据CAn设备状态设置打开按钮
    if(m_devsetdlg->getCan()->getIsOpen() && m_devsetdlg->getCan()->getIsStart())
    {
        QIcon  icon(":/src/Close.jpg");
        m_devCanLight->setColor(Qt::green);
        ui->pushButtonOpen->setIcon(icon);
        ui->pushButtonOpen->setIconSize(QSize(36,36));
        ui->pushButtonWritePrograme->setEnabled(true);
    }
    else
    {
        m_devCanLight->setColor(Qt::red);
        QIcon  icon(":/src/Open.jpg");
        ui->pushButtonOpen->setIcon(icon);
        ui->pushButtonOpen->setIconSize(QSize(40,40));
        ui->pushButtonWritePrograme->setEnabled(false);
    }

    //绘制电压表
    m_voltageui = new VoltageUI(ui->groupBoxMianInfo);
    m_voltageui->resize(220,220);
    m_voltageui->setVisible(true);
    m_voltageui->move(20,20);
    m_voltageui->show();

    //绘制电流表
    ampereMeter = new AmpereMeter(ui->groupBoxMianInfo);
    ampereMeter->resize(220,220);
    ampereMeter->setVisible(true);
    ampereMeter->move(280,20);
    ampereMeter->show();

    //绘制电量显示器
    m_batterySoc = new BatterySoc(ui->groupBoxMianInfo);
    m_batterySoc->resize(80,220);
    m_batterySoc->move(540,20);
    m_batterySoc->show();

    //绘制电芯极值信息
    m_cellMinMaxInfoUi = new CellMinMaxInfo(ui->groupBoxMianInfo);
    m_cellMinMaxInfoUi->move(15,300);
    m_cellMinMaxInfoUi->show();

    //绘制通信指示灯
    for(uchar i=0;i<IndictorLightNum;i++)
    {
        m_indictor_light[i] = new IndictorLight(this);
        m_indictor_light_label[i] = new QLabel(this);

        m_indictor_light[i]->move(20+i*120,600);
        m_indictor_light[i]->resize(30,30);
        if(i%2 == 0)
        {
            m_indictor_light[i]->setColor(Qt::red);
        }
        else
        {
            m_indictor_light[i]->setColor(Qt::green);
        }
        m_indictor_light[i]->show();

        m_indictor_light_label[i]->setText(IndictorLabelText[i]);
        m_indictor_light_label[i]->move(20+i*120+m_indictor_light[i]->width()+5,600);
    }

    //tableView的设置
    initModel();

    //表头信息显示居中
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    //设置列宽不可变
    for(int i=0;i<6;i++)
    {
        ui->tableView->horizontalHeader()->setResizeMode(i,QHeaderView::Fixed);
        ui->tableView->setColumnWidth(i,130);
    }
    ui->tableView->horizontalHeader()->setResizeMode(6,QHeaderView::Fixed);
    ui->tableView->setColumnWidth(6,200);

    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置电芯电压只读
    ui->lineEditCell01->setReadOnly(true);
    ui->lineEditCell02->setReadOnly(true);
    ui->lineEditCell03->setReadOnly(true);
    ui->lineEditCell04->setReadOnly(true);
    ui->lineEditCell05->setReadOnly(true);
    ui->lineEditCell06->setReadOnly(true);
    ui->lineEditCell07->setReadOnly(true);
    ui->lineEditCell08->setReadOnly(true);
    ui->lineEditCell09->setReadOnly(true);
    ui->lineEditCell10->setReadOnly(true);
    ui->lineEditCell11->setReadOnly(true);
    ui->lineEditCell12->setReadOnly(true);
    ui->lineEditCell13->setReadOnly(true);
    ui->lineEditCell14->setReadOnly(true);
    ui->lineEditCell15->setReadOnly(true);
    ui->lineEditCell16->setReadOnly(true);
    ui->lineEditCell17->setReadOnly(true);
    ui->lineEditCell18->setReadOnly(true);
    ui->lineEditCell19->setReadOnly(true);
    ui->lineEditCell20->setReadOnly(true);

    ui->lineEditCOC->setReadOnly(true);
    ui->lineEditDOC->setReadOnly(true);
    ui->lineEditCOV->setReadOnly(true);
    ui->lineEditCUV->setReadOnly(true);
    ui->lineEditCOT->setReadOnly(true);
    ui->lineEditCUT->setReadOnly(true);
    ui->lineEditDOT->setReadOnly(true);
    ui->lineEditDUT->setReadOnly(true);
    ui->lineEditLTC_COM->setReadOnly(true);
    ui->lineEditVDIF->setReadOnly(true);
    ui->lineEditTDIF->setReadOnly(true);
    ui->lineEditPOV->setReadOnly(true);
    ui->lineEditPUV->setReadOnly(true);

    // 设置导出数据的model
    modelTestData = new QStandardItemModel();
    modelTestData->setColumnCount(46);
    modelTestData->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("时间"));
    modelTestData->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("电池包状态"));
    modelTestData->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("总电压"));
    modelTestData->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("总电流"));
    modelTestData->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("SOC"));
    modelTestData->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("最大电压"));
    modelTestData->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("编号VMax"));
    modelTestData->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("最小电压"));
    modelTestData->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("编号VMin"));
    modelTestData->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("温度1"));
    modelTestData->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("温度2"));
    modelTestData->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("温度3"));
    modelTestData->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("温度4"));
    modelTestData->setHeaderData(13,Qt::Horizontal,QString::fromLocal8Bit("循环次数"));
    modelTestData->setHeaderData(14,Qt::Horizontal,QString::fromLocal8Bit("电压差"));
    modelTestData->setHeaderData(15,Qt::Horizontal,QString::fromLocal8Bit("温度差"));
    modelTestData->setHeaderData(16,Qt::Horizontal,QString::fromLocal8Bit("单体过压警告等级"));
    modelTestData->setHeaderData(17,Qt::Horizontal,QString::fromLocal8Bit("单体欠压警告等级"));
    modelTestData->setHeaderData(18,Qt::Horizontal,QString::fromLocal8Bit("充电高温警告等级"));
    modelTestData->setHeaderData(19,Qt::Horizontal,QString::fromLocal8Bit("充电低温警告等级"));
    modelTestData->setHeaderData(20,Qt::Horizontal,QString::fromLocal8Bit("放电高温警告等级"));
    modelTestData->setHeaderData(21,Qt::Horizontal,QString::fromLocal8Bit("放电低温警告等级"));
    modelTestData->setHeaderData(22,Qt::Horizontal,QString::fromLocal8Bit("充电过流警告等级"));
    modelTestData->setHeaderData(23,Qt::Horizontal,QString::fromLocal8Bit("放电过流警告等级"));
    modelTestData->setHeaderData(24,Qt::Horizontal,QString::fromLocal8Bit("温差过大警告等级"));
    modelTestData->setHeaderData(25,Qt::Horizontal,QString::fromLocal8Bit("压差过大警告等级"));

    for(int i=0;i<20;i++)
    {
        modelTestData->setHeaderData(26+i,Qt::Horizontal,
                                     "Cell" + QString::number(i+1));
    }

    //CAN数据处理器
    m_dataProcess = new DataProcess(modelTestData);
    m_dataProcess->setTimerInterval(50); //50ms接收一次数据
    m_sysPraModel = new SysPraModel(m_dataProcess,ui->tabSet);
    m_sysPraModel->move(0,5);
    m_sysPraModel->show();

    //默认不记录数据
    ui->radioButtonNo->setChecked(false);
    ui->radioButtonYes->setChecked(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSet_triggered()
{
    if(m_devsetdlg->exec() == QDialog::Accepted)
    {
        m_devsetdlg->getValueFrmUi();
    }
}

void MainWindow::on_pushButtonOpen_clicked()
{
    int ret = 0;
    int devType = m_devsetdlg->getCan()->getDevType();
    int devIndex = m_devsetdlg->getCan()->getDevIndex();
    int canIndex = m_devsetdlg->getCan()->getCanIndex();
    PVCI_INIT_CONFIG pInitConfig = m_devsetdlg->getCan()->getInitConfig();

    //如果设备是打开的就关闭
    if(m_devsetdlg->getCan()->getIsOpen() &&
            m_devsetdlg->getCan()->getIsStart())
    {
        m_devsetdlg->getCan()->CanResetCAN(devType,devIndex,canIndex);//CAN复位
        if(m_devsetdlg->getCan()->CanClose(devType,devIndex))
        {
            m_devCanLight->setColor(Qt::red);
            QIcon  icon(":/src/Open.jpg");
            ui->pushButtonOpen->setIconSize(QSize(42,42));
            ui->pushButtonOpen->setIcon(icon);

            m_devsetdlg->getCan()->setStartorStop(false);//关闭CAN
            m_dataProcess->timerStop();
        }

        ui->pushButtonWritePrograme->setEnabled(false);

    }
    else //打开
    {
        if(!m_devsetdlg->getCan()->getIsOpen())
            m_devsetdlg->getCan()->CanOpen(devType,devIndex,0);

        if(m_devsetdlg->getCan()->getIsOpen())
        {
            ret = m_devsetdlg->getCan()->CanInit(devType,devIndex,canIndex,pInitConfig);
            if(ret)
                m_devsetdlg->getCan()->CanStartCAN(devType,devIndex,canIndex);
        }
        if(m_devsetdlg->getCan()->getIsStart())
        {
            m_devCanLight->setColor(Qt::green);
            QIcon  icon(":/src/Close.jpg");
            ui->pushButtonOpen->setIconSize(QSize(36,36));
            ui->pushButtonOpen->setIcon(icon);
            //启动CAN发送接收
            m_dataProcess->timerStart();
            ui->pushButtonWritePrograme->setEnabled(true);
        }
    }
}



