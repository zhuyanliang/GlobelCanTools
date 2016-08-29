#include "SysPraModel.h"
#include "ui_SysPraModel.h"
#include <QStandardItemModel>
#include "DataProcess.h"
#include "Header.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QElapsedTimer>
#include <QColor>

#define CONFIGNUM   12
#define TIMEECLIPSE 500

SysPraModel::SysPraModel(DataProcess *datpress,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysPraModel),
    m_dataProcess(datpress),
    xmlconfig(NULL)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTime()));
    m_timer->start();

    m_modelSysPra = new QStandardItemModel();
    m_can = CanBus::getInstance();

    ui->pushButtonWritePra->setEnabled(false);
    ui->textEditConfig->setFontPointSize(16);
    ui->textEditConfig->setFontWeight(75);
    ui->textEditConfig->setReadOnly(true);
    InitUI();
}

SysPraModel::~SysPraModel()
{
    delete ui;
}

void SysPraModel::InitUI(void)
{
    //系统参数tableView的设置
    m_modelSysPra->setColumnCount(3);
    m_modelSysPra->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("名称"));
    m_modelSysPra->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("值"));
    m_modelSysPra->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("单位"));
    ui->tableViewSysPra->setModel(m_modelSysPra);
    ui->tableViewSysPra->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //设置列自适应宽度
    ui->tableViewSysPra->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    //设置选中时为整行选中
    ui->tableViewSysPra->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表格的单元为只读属性，即不能编辑
    ui->tableViewSysPra->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewSysPra->horizontalHeader()->setStretchLastSection(true);

    m_modelSysPra->setItem(0,0,new QStandardItem("单体过压一级警告点"));
    m_modelSysPra->item(0,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(0,2,new QStandardItem("mV"));
    m_modelSysPra->item(0,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(1,0,new QStandardItem("单体过压二级警告点"));
    m_modelSysPra->item(1,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(1,2,new QStandardItem("mV"));
    m_modelSysPra->item(1,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(2,0,new QStandardItem("单体欠压一级警告点"));
    m_modelSysPra->item(2,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(2,2,new QStandardItem("mV"));
    m_modelSysPra->item(2,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(3,0,new QStandardItem("单体欠压二级警告点"));
    m_modelSysPra->item(3,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(3,2,new QStandardItem("mV"));
    m_modelSysPra->item(3,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(4,0,new QStandardItem("电池包充电高温一级警告点"));
    m_modelSysPra->item(4,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(4,2,new QStandardItem("℃"));
    m_modelSysPra->item(4,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(5,0,new QStandardItem("电池包充电高温二级警告点"));
    m_modelSysPra->item(5,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(5,2,new QStandardItem("℃"));
    m_modelSysPra->item(5,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(6,0,new QStandardItem("电池包充电低温一级警告点"));
    m_modelSysPra->item(6,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(6,2,new QStandardItem("℃"));
    m_modelSysPra->item(6,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(7,0,new QStandardItem("电池包充电低温二级警告点"));
    m_modelSysPra->item(7,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(7,2,new QStandardItem("℃"));
    m_modelSysPra->item(7,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(8,0,new QStandardItem("电池包放电高温一级警告点"));
    m_modelSysPra->item(8,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(8,2,new QStandardItem("℃"));
    m_modelSysPra->item(8,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(9,0,new QStandardItem("电池包放电高温二级警告点"));
    m_modelSysPra->item(9,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(9,2,new QStandardItem("℃"));
    m_modelSysPra->item(9,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(10,0,new QStandardItem("电池包放电低温一级警告点"));
    m_modelSysPra->item(10,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(10,2,new QStandardItem("℃"));
    m_modelSysPra->item(10,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(11,0,new QStandardItem("电池包放电低温二级警告点"));
    m_modelSysPra->item(11,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(11,2,new QStandardItem("℃"));
    m_modelSysPra->item(11,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(12,0,new QStandardItem("电池包充电过流一级警告点"));
    m_modelSysPra->item(12,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(12,2,new QStandardItem("A"));
    m_modelSysPra->item(12,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(13,0,new QStandardItem("电池包充电过流二级警告点"));
    m_modelSysPra->item(13,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(13,2,new QStandardItem("A"));
    m_modelSysPra->item(13,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(14,0,new QStandardItem("电池包放电过流一级警告点"));
    m_modelSysPra->item(14,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(14,2,new QStandardItem("A"));
    m_modelSysPra->item(14,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(15,0,new QStandardItem("电池包放电过流二级警告点"));
    m_modelSysPra->item(15,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(15,2,new QStandardItem("A"));
    m_modelSysPra->item(15,2)->setTextAlignment(Qt::AlignCenter);


    m_modelSysPra->setItem(16,0,new QStandardItem("电池包温差一级警告点"));
    m_modelSysPra->item(16,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(16,2,new QStandardItem("℃"));
    m_modelSysPra->item(16,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(17,0,new QStandardItem("电池包温差二级警告点"));
    m_modelSysPra->item(17,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(17,2,new QStandardItem("℃"));
    m_modelSysPra->item(17,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(18,0,new QStandardItem("电池包过压一级警告点"));
    m_modelSysPra->item(18,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(18,2,new QStandardItem("V"));
    m_modelSysPra->item(18,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(19,0,new QStandardItem("电池包过压二级警告点"));
    m_modelSysPra->item(19,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(19,2,new QStandardItem("V"));
    m_modelSysPra->item(19,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(20,0,new QStandardItem("电池包欠压一级警告点"));
    m_modelSysPra->item(20,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(20,2,new QStandardItem("V"));
    m_modelSysPra->item(20,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(21,0,new QStandardItem("电池包欠压二级警告点"));
    m_modelSysPra->item(21,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(21,2,new QStandardItem("V"));
    m_modelSysPra->item(21,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(22,0,new QStandardItem("单体电压一致性一级警告点"));
    m_modelSysPra->item(22,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(22,2,new QStandardItem("mV"));
    m_modelSysPra->item(22,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(23,0,new QStandardItem("单体电压二致性一级警告点"));
    m_modelSysPra->item(23,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(23,2,new QStandardItem("mV"));
    m_modelSysPra->item(23,2)->setTextAlignment(Qt::AlignCenter);
}

void SysPraModel::onTime(void)
{
    unsigned char readData[8] = {0};
    short tempData = 0;

    //PACKPRA
    m_dataProcess->getPackPra(readData,8);
    ui->labelSeriesNum->setText(QString::number(readData[0]));
    ui->labelSeriesNum->setFont(QFont("Times",18,QFont::Bold));
    ui->labelParallelNum->setText(QString::number(readData[1]));
    ui->labelParallelNum->setFont(QFont("Times",18,QFont::Bold));
    unsigned short capacity = ((readData[3]<<8)|readData[2])/10;
    ui->labelCapacity->setText(QString::number(capacity)+"Ah");
    ui->labelCapacity->setFont(QFont("Times",18,QFont::Bold));

    // OUV 电芯高低压
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getCellOUV(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(0,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(1,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[5]<<8)+readData[4]);
    m_modelSysPra->setItem(2,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[7]<<8)+readData[6]);
    m_modelSysPra->setItem(3,1,new QStandardItem(QString::number(tempData)));

    //COT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackCOT(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(4,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(5,1,new QStandardItem(QString::number(tempData)));

    // CUT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackCUT(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(6,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(7,1,new QStandardItem(QString::number(tempData)));

    // DOT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDOT(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(8,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(9,1,new QStandardItem(QString::number(tempData)));

    // DUT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDUT(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(10,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(11,1,new QStandardItem(QString::number(tempData)));

    // COC
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackCOC(readData,8);
    tempData = (readData[1]<<8)+readData[0];
    tempData /= 10;
    m_modelSysPra->setItem(12,1,new QStandardItem(QString::number(tempData)));
    tempData = (readData[3]<<8)+readData[2];
    tempData /= 10;
    m_modelSysPra->setItem(13,1,new QStandardItem(QString::number(tempData)));

    // DOC
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDOC(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(14,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2])/10;
    m_modelSysPra->setItem(15,1,new QStandardItem(QString::number(tempData)));

    // PDLT 电池包温差
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDLT(readData,8);
    tempData = (readData[1]<<8)+readData[0];
    m_modelSysPra->setItem(16,1,new QStandardItem(QString::number(tempData)));
    tempData = (readData[3]<<8)+readData[2];
    m_modelSysPra->setItem(17,1,new QStandardItem(QString::number(tempData)));

    // 电池包过压
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackOV(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(18,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2])/10;
    m_modelSysPra->setItem(19,1,new QStandardItem(QString::number(tempData)));

    // 电池包欠压
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackUV(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(20,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2])/10;
    m_modelSysPra->setItem(21,1,new QStandardItem(QString::number(tempData)));

    // IBM 单体一致性
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getCellDLV(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(22,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(23,1,new QStandardItem(QString::number(tempData)));


    if(m_can->getIsStart() && (xmlconfig!=NULL))
    {
        if(xmlconfig->readXmlok)
            ui->pushButtonWritePra->setEnabled(true);
    }
    else
    {
        ui->pushButtonWritePra->setEnabled(false);
    }
}

uint32_t CAN_GenerateID(uint8_t msg_fc)
{
    uint32_t temp = 0;

    temp = ((uint32_t)0x18 << 24) + ((uint32_t)msg_fc << 16) +
        (0xF4 << 8) + GUI;
    return temp;
}

void SysPraModel::on_pushButtonImportConfig_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("导入配置文件"),"",tr("配置文件(*.xml);;所有(*.*)"));

    if(!fileName.isEmpty())
    {
        xmlconfig = new XmlConfig();

        if(xmlconfig->readFile(fileName))
        {
            ui->lineEditConfigFilePath->setText(QFileInfo(fileName).canonicalFilePath());
            ui->pushButtonWritePra->setEnabled(true);

            QMapIterator<QString, short> i(xmlconfig->getValue());
            while (i.hasNext())
            {
                i.next();
                QString text = i.key();
                text += ": ";
                text += QString::number(i.value());
                ui->textEditConfig->append(text);
            }
        }

    }
}

// 写入设置的参数
void SysPraModel::on_pushButtonWritePra_clicked()
{
    int devType = m_can->getDevType();
    int devIndex = m_can->getDevIndex();
    int canIndex = m_can->getCanIndex();
    unsigned int sendData = 0;
    VCI_CAN_OBJ sendFrame = {0};
    QElapsedTimer t;
    unsigned char configNum = 0;

    QProgressDialog progress("系统参数配置中...", "终止配置", 0, CONFIGNUM, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(configNum++);
    progress.show();

    sendFrame.SendType = 0;
    sendFrame.RemoteFlag = 0;
    sendFrame.ExternFlag = 1;
    sendFrame.DataLen = 8;

    if(xmlconfig == NULL)
        return;

    // COV
    if(xmlconfig->getValue().contains("cov1"))
    {
        sendData = xmlconfig->getValue().value("cov1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("cov2"))
    {
        sendData = xmlconfig->getValue().value("cov2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_COV_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }

    t.start();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();
    // CUV
    if(xmlconfig->getValue().contains("cuv1"))
    {
        sendData = xmlconfig->getValue().value("cuv1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("cuv2"))
    {
        sendData = xmlconfig->getValue().value("cuv2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_CUV_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PCOT
    if(xmlconfig->getValue().contains("pcot1"))
    {
        sendData = xmlconfig->getValue().value("pcot1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pcot2"))
    {
        sendData = xmlconfig->getValue().value("pcot2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_COT_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PCUT
    if(xmlconfig->getValue().contains("pcut1"))
    {
        sendData = xmlconfig->getValue().value("pcut1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pcut2"))
    {
        sendData = xmlconfig->getValue().value("pcut2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_CUT_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PDOT
    if(xmlconfig->getValue().contains("pdot1"))
    {
        sendData = xmlconfig->getValue().value("pdot1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pdot2"))
    {
        sendData = xmlconfig->getValue().value("pdot2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_DOT_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PDUT
    if(xmlconfig->getValue().contains("pdut1"))
    {
        sendData = xmlconfig->getValue().value("pdut1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pdut2"))
    {
        sendData = xmlconfig->getValue().value("pdut2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_DUT_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PCOC
    if(xmlconfig->getValue().contains("pcoc1"))
    {
        sendData = xmlconfig->getValue().value("pcoc1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pcoc2"))
    {
        sendData = xmlconfig->getValue().value("pcoc2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_COC_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PDOC
    if(xmlconfig->getValue().contains("pdoc1"))
    {
        sendData = xmlconfig->getValue().value("pdoc1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pdoc2"))
    {
        sendData = xmlconfig->getValue().value("pdoc2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_DOC_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PDLV
    if(xmlconfig->getValue().contains("dlv1"))
    {
        sendData = xmlconfig->getValue().value("dlv1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("dlv2"))
    {
        sendData = xmlconfig->getValue().value("dlv2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_DLV_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PDLT
    if(xmlconfig->getValue().contains("pdlt1"))
    {
        sendData = xmlconfig->getValue().value("pdlt1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pdlt2"))
    {
        sendData = xmlconfig->getValue().value("pdlt2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_DLT_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // POV
    if(xmlconfig->getValue().contains("pov1"))
    {
        sendData = xmlconfig->getValue().value("pov1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("pov2"))
    {
        sendData = xmlconfig->getValue().value("pov2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_POV_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

    // PUV
    if(xmlconfig->getValue().contains("puv1"))
    {
        sendData = xmlconfig->getValue().value("puv1");
    }
    sendFrame.Data[0] = sendData&0xFF;
    sendFrame.Data[1] = (sendData>>8)&0xFF;
    if(xmlconfig->getValue().contains("puv2"))
    {
        sendData = xmlconfig->getValue().value("puv2");
    }
    sendFrame.Data[2] = sendData&0xFF;
    sendFrame.Data[3] = (sendData>>8)&0xFF;

    sendFrame.ID = CAN_GenerateID(CAN_GUI_CONFIG_PUV_TH);
    if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1))
    {
        qDebug() << "Write Success";
        progress.setValue(configNum++);
    }
    t.restart();
    while(t.elapsed()<TIMEECLIPSE)
        QCoreApplication::processEvents();

}
