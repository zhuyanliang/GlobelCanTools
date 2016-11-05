#include <QGridLayout>
#include <QLabel>
#include <QThread>
#include <QTime>
#include <QMap>
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

#define ROWNUM 10000

QString statusArray[5] = {
    "IDLE","PRECHARGE","DISCHARGE",
    "CHARGE","PROTECTION"
};

void MainWindow::initModel()
{
    if(model != NULL)
        delete model;

    model = new QStandardItemModel();
    //tableView的设置
    model->setColumnCount(7);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("传输方向"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间标识"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("帧ID"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("帧格式"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("帧类型"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("数据长度"));
    model->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("数据"));
    ui->tableView->setModel(model);
}

void MainWindow::initTestDataModel()
{
    if(modelTestData != NULL)
        delete modelTestData;

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
    m_dataProcess->setModel(modelTestData);
}

void MainWindow::timeUpdate(void)
{
    if(!m_devsetdlg->getCan()->getIsOpen() ||
            !m_devsetdlg->getCan()->getIsStart())
    {
        return;
    }
    float maxV  = m_dataProcess->getMaxCellVoltage();
    float minV = m_dataProcess->getMinCellVoltage();
    uchar maxVP = m_dataProcess->getMaxCellVolNum();
    uchar minVP = m_dataProcess->getMinCellVolNum();
    float vDlt = maxV - minV;

    char maxT = m_dataProcess->getMaxCellTemp();
    char minT = m_dataProcess->getMinCellTemp();
    uchar maxTP = m_dataProcess->getMaxCellTempNum();
    uchar minTP = m_dataProcess->getMinCellTempNum();
    char tDlt = maxT - minT;

    float totalVoltage = m_dataProcess->getTotalVoltage();
    float current = 0.0;
    ushort currentTemp = m_dataProcess->getCurrent();
    if( currentTemp&0x8000)
    {
        short temp = 0xFFFF - currentTemp;
        current = temp * (-0.1);
    }
    else
    {
        current = currentTemp * 0.1;
    }

    uchar soc = m_dataProcess->getSoc();

    //表盘更新
    m_voltageui->setValue(totalVoltage);
    ampereMeter->setValue(current);
    m_batterySoc->setValue(soc);

    m_cellMinMaxInfoUi->lineEditMaxCellVol->setText(QString::number(maxV));
    m_cellMinMaxInfoUi->lineEditMaxVPos->setText(QString::number(maxVP));
    m_cellMinMaxInfoUi->lineEditMinCellVol->setText(QString::number(minV));
    m_cellMinMaxInfoUi->lineEditMinVPos->setText(QString::number(minVP));
    m_cellMinMaxInfoUi->lineEditDltV->setText(QString::number(vDlt));

    m_cellMinMaxInfoUi->lineEditMaxCellTemp->setText(QString::number(maxT));
    m_cellMinMaxInfoUi->lineEditMinCellTemp->setText(QString::number(minT));
    m_cellMinMaxInfoUi->lineEditMaxTPos->setText(QString::number(maxTP));
    m_cellMinMaxInfoUi->lineEditMinTPos->setText(QString::number(minTP));
    m_cellMinMaxInfoUi->lineEditDltT->setText(QString::number(tDlt));

    float cellsValt[20] = {0};
    m_dataProcess->getCellsVoltage(cellsValt,20);
    //所有Cell的电压
    ui->lineEditCell01->setText(QString::number(cellsValt[0]));
    ui->lineEditCell02->setText(QString::number(cellsValt[1]));
    ui->lineEditCell03->setText(QString::number(cellsValt[2]));
    ui->lineEditCell04->setText(QString::number(cellsValt[3]));
    ui->lineEditCell05->setText(QString::number(cellsValt[4]));
    ui->lineEditCell06->setText(QString::number(cellsValt[5]));
    ui->lineEditCell07->setText(QString::number(cellsValt[6]));
    ui->lineEditCell08->setText(QString::number(cellsValt[7]));
    ui->lineEditCell09->setText(QString::number(cellsValt[8]));
    ui->lineEditCell10->setText(QString::number(cellsValt[9]));
    ui->lineEditCell11->setText(QString::number(cellsValt[10]));
    ui->lineEditCell12->setText(QString::number(cellsValt[11]));
    ui->lineEditCell13->setText(QString::number(cellsValt[12]));
    ui->lineEditCell14->setText(QString::number(cellsValt[13]));
    ui->lineEditCell15->setText(QString::number(cellsValt[14]));
    ui->lineEditCell16->setText(QString::number(cellsValt[15]));
    ui->lineEditCell17->setText(QString::number(cellsValt[16]));
    ui->lineEditCell18->setText(QString::number(cellsValt[17]));
    ui->lineEditCell19->setText(QString::number(cellsValt[18]));
    ui->lineEditCell20->setText(QString::number(cellsValt[19]));

    char temp[4] = {0};
    m_dataProcess->getCellTemp(temp,8);
    ui->lineEditTemp01->setText(QString::number(temp[0]));
    ui->lineEditTemp02->setText(QString::number(temp[1]));
    ui->lineEditTemp03->setText(QString::number(temp[2]));
    ui->lineEditTemp04->setText(QString::number(temp[3]));

    //系统参数刷新
    unsigned char readData[8] = {0};
    short tempData = 0;

    //获取系统中的警告和错误信息
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getBattWarnError(readData,8);
    SysWarningDef warn;
    warn = *(reinterpret_cast<SysWarningDef*>(readData));
    setWarnRadioButton((int)warn.COV,ui->COVL1,ui->COVL2);
    setWarnRadioButton((int)warn.CUV,ui->CUVL1,ui->CUVL2);
    setWarnRadioButton((int)warn.COT,ui->COTL1,ui->COTL2);
    setWarnRadioButton((int)warn.CUT,ui->CUTL1,ui->CUTL2);
    setWarnRadioButton((int)warn.DOT,ui->DOTL1,ui->DOTL2);
    setWarnRadioButton((int)warn.DUT,ui->DUTL1,ui->DUTL2);
    setWarnRadioButton((int)warn.COC,ui->COCL1,ui->COCL2);
    setWarnRadioButton((int)warn.DOC,ui->DOCL1,ui->DOCL2);
    setWarnRadioButton((int)warn.CIB,ui->VDIFL1,ui->VDIFL2);
    setWarnRadioButton((int)warn.TIB,ui->TDIFL1,ui->TDIFL2);
    setWarnRadioButton((int)warn.POV,ui->POVL1,ui->POVL2);
    setWarnRadioButton((int)warn.PUV,ui->PUVL1,ui->PUVL2);

    // NOR_REC SOH和最近的三次错误信息
    memset(readData,0,8);
    unsigned short times = 0;
    m_dataProcess->getNorRec(readData,8);
    times = readData[1];
    times <<= 8;
    times |= readData[0];
    m_cellMinMaxInfoUi->lineEditCycTimes->setText(QString::number(times));

    ui->labelFault1->setText("1." + FaultInfo.value(readData[2]));
    ui->labelFault2->setText("2." + FaultInfo.value(readData[3]));
    ui->labelFault3->setText("3." + FaultInfo.value(readData[4]));

    // FALT_1
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getFaultRec1(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditCOC->setText(QString::number(tempData));
    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditDOC->setText(QString::number(tempData));

    //FALT_2
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getFaultRec2(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditCOV->setText(QString::number(tempData));
    tempData = readData[3];
    tempData <<= 8;
    tempData += readData[2];
    ui->lineEditPOV->setText(QString::number(tempData));
    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditCUV->setText(QString::number(tempData));
    tempData = readData[7];
    tempData <<= 8;
    tempData += readData[6];
    ui->lineEditPUV->setText(QString::number(tempData));

    // FALT_3
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getFaultRec3(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditCOT->setText(QString::number(tempData));
    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditCUT->setText(QString::number(tempData));

    // FALT_4
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getFaultRec4(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditDOT->setText(QString::number(tempData));
    tempData = readData[3];
    tempData <<= 8;
    tempData += readData[2];
    ui->lineEditDUT->setText(QString::number(tempData));

    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditVDIF->setText(QString::number(tempData));
    tempData = readData[7];
    tempData <<= 8;
    tempData += readData[6];
    ui->lineEditTDIF->setText(QString::number(tempData));

    // FALT_5  LTC6803
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getFaultRec5(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditLTC_COM->setText(QString::number(tempData));

    // 电池状态
    uchar index = (uchar)m_sysPraModel->getStatus();
    if(index >= 0 && index < 5)
        statusLabel->setText(statusArray[index]);
    else
        statusLabel->setText("UNKNOW");

    // 提取要导出的测试数据
    if(ui->radioButtonNo->isChecked())
        return;
    // 提取数据
    int rows = modelTestData->rowCount();
    QTime currenttime = QTime::currentTime();

    modelTestData->setItem(rows,0,new QStandardItem(currenttime.toString()));
    modelTestData->setItem(rows,1,new QStandardItem(statusLabel->text()));
    modelTestData->setItem(rows,2,new QStandardItem(QString::number(totalVoltage)));
    modelTestData->setItem(rows,3,new QStandardItem(QString::number(current)));
    modelTestData->setItem(rows,4,new QStandardItem(QString::number(soc)));
    modelTestData->setItem(rows,5,new QStandardItem(QString::number(maxV)));
    modelTestData->setItem(rows,6,new QStandardItem(QString::number(maxVP)));
    modelTestData->setItem(rows,7,new QStandardItem(QString::number(minV)));
    modelTestData->setItem(rows,8,new QStandardItem(QString::number(minVP)));
    modelTestData->setItem(rows,9,new QStandardItem(QString::number(temp[0])));
    modelTestData->setItem(rows,10,new QStandardItem(QString::number(temp[1])));
    modelTestData->setItem(rows,11,new QStandardItem(QString::number(temp[2])));
    modelTestData->setItem(rows,12,new QStandardItem(QString::number(temp[3])));
    modelTestData->setItem(rows,13,new QStandardItem(QString::number(times)));
    modelTestData->setItem(rows,14,new QStandardItem(QString::number(vDlt)));
    modelTestData->setItem(rows,15,new QStandardItem(QString::number(tDlt)));
    modelTestData->setItem(rows,16,new QStandardItem(QString::number(warn.COV)));
    modelTestData->setItem(rows,17,new QStandardItem(QString::number(warn.CUV)));
    modelTestData->setItem(rows,18,new QStandardItem(QString::number(warn.COT)));
    modelTestData->setItem(rows,19,new QStandardItem(QString::number(warn.CUT)));
    modelTestData->setItem(rows,20,new QStandardItem(QString::number(warn.DOT)));
    modelTestData->setItem(rows,21,new QStandardItem(QString::number(warn.DUT)));
    modelTestData->setItem(rows,22,new QStandardItem(QString::number(warn.COC)));
    modelTestData->setItem(rows,23,new QStandardItem(QString::number(warn.DOC)));
    modelTestData->setItem(rows,24,new QStandardItem(QString::number(warn.TIB)));
    modelTestData->setItem(rows,25,new QStandardItem(QString::number(warn.CIB)));
    for(int i=0;i<20;i++)
    {
        modelTestData->setItem(rows,26+i,new QStandardItem(QString::number(cellsValt[i])));
    }

    if(modelTestData->rowCount() > 5)
    {
        m_dataProcess->dataStore();
        initTestDataModel();
    }
}

void MainWindow::dataReceived(VCI_CAN_OBJ &data)
{
    int rows = model->rowCount();
    if(rows > ROWNUM)
    {
       initModel();
       rows = 0;
    }
    //更新tableView控件的数据
    model->setItem(rows,0,new QStandardItem("接收"));
    //设置字符颜色
    model->item(rows,0)->setForeground(QBrush(QColor(0, 0, 255)));
    //设置字符位置
    model->item(rows,0)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,1,new QStandardItem(QString::number(QDateTime::currentDateTime().toTime_t())));
    model->item(rows,1)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,2,new QStandardItem(QString::number(data.ID,16).toUpper()));
    model->item(rows,2)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,4,new QStandardItem(QString::fromLocal8Bit("数据帧")));
    model->item(rows,4)->setTextAlignment(Qt::AlignCenter);

    if(data.ExternFlag)
        model->setItem(rows,3,new QStandardItem(QString::fromLocal8Bit("扩展帧")));
    else
        model->setItem(rows,3,new QStandardItem(QString::fromLocal8Bit("标准帧")));
    if(data.RemoteFlag)
        data.DataLen = 0;
    model->item(rows,3)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,5,new QStandardItem(QString::number(data.DataLen,10)));
    model->item(rows,5)->setTextAlignment(Qt::AlignCenter);

    model->setItem(rows,6,new QStandardItem(QString("%1").arg(data.Data[7]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[6]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[5]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[4]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[3]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[2]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[1]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[0]&0xff,2,16,QLatin1Char('0')).toUpper()));
    model->item(rows,6)->setTextAlignment(Qt::AlignCenter);
    model->item(rows,6)->setForeground(QBrush(QColor(255, 0, 0)));

    ui->tableView->selectRow(rows);
    QCoreApplication::processEvents();
}

void MainWindow::setWarnRadioButton(int level,QCheckBox *checkbox1,QCheckBox *checkbox2)
{
    if(0 == level)
    {
        checkbox1->setChecked(false);
        checkbox2->setChecked(false);
    }
    else if(1 == level)
    {
        checkbox1->setChecked(true);
        checkbox2->setChecked(false);
    }
    else if(2 == level)
    {
        checkbox1->setChecked(false);
        checkbox2->setChecked(true);
    }
}


//查看设备信息
void MainWindow::on_actionBoardInfo_triggered()
{
    QDialog boardInfoWidget(this);
    VCI_BOARD_INFO boardInfo;
    QLabel label(&boardInfoWidget);
    QPushButton button(&boardInfoWidget);
    CanBus *can = CanBus::getInstance();

    connect(&button,SIGNAL(clicked()),&boardInfoWidget,SLOT(close()));

    int devType = m_devsetdlg->getCan()->getDevType();
    int devIndex = m_devsetdlg->getCan()->getDevIndex();

    boardInfoWidget.setWindowTitle(QObject::tr("设备信息"));

    if(!can->getIsOpen())
        can->CanOpen(devType,devIndex,0);

    if(can->getIsOpen() &&
            (!can->CanReadBoardInfo(devType,devIndex,&boardInfo)))
    {
        QMessageBox::warning(this,QObject::tr("读取失败"),
                             QObject::tr("获取设备信息失败"),QMessageBox::Ok);
    }
    else
    {
        label.setText(QString("\n")+
                    QObject::tr("hw_Version:")+QString::number(boardInfo.fw_Version)+"\n"
                            +QObject::tr("fw_Version:") + QString::number(boardInfo.fw_Version)+"\n"
                            +QObject::tr("dr_Version:") + QString::number(boardInfo.dr_Version)+"\n"
                            +QObject::tr("in_Version:") + QString::number(boardInfo.in_Version)+"\n"
                            +QObject::tr("irq_Num:") + QString::number(boardInfo.irq_Num)+"\n"
                            +QObject::tr("can_Num:") + QString::number(boardInfo.can_Num)+"\n"
                            +QObject::tr("str_hw_Type:") + boardInfo.str_hw_Type+"\n"
                            +"\n");
        label.resize(150,115);
        label.move(10,6);
        button.setText(QObject::tr("确定"));
        button.resize(40,30);
        boardInfoWidget.resize(150,160);
        button.move(110,125);
        boardInfoWidget.exec();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int devType = m_devsetdlg->getCan()->getDevType();
    int devIndex = m_devsetdlg->getCan()->getDevIndex();
    int canIndex = m_devsetdlg->getCan()->getCanIndex();

    //TODO: 在退出窗口之前，实现希望做的操作
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
    }
    QMainWindow::closeEvent(event);
    qDebug() << "Close";
}


void MainWindow::on_pushButtonWritePrograme_clicked()
{
    m_timer->stop();
    m_dataProcess->timerStop();

    DownLoadUi downloadui;
    downloadui.exec();

    m_timer->start();
    m_dataProcess->timerStart();
}
