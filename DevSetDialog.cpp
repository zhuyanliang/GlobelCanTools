#include "DevSetDialog.h"
#include "ui_devsetdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QSettings>
//#include "DebugConfig.h"

DevSetDialog::DevSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevSetDialog)
{
    ui->setupUi(this);

    m_can = CanBus::getInstance();

    //从QSettings中都回上次的参数
    QSettings settings;
    ui->comboBoxDevType->setCurrentIndex(
            settings.value("devType",0).toInt());
    ui->comboBoxDecIndex->setCurrentIndex(
            settings.value("devIndex",0).toInt());
    ui->comboBoxCanIndex->setCurrentIndex(
            settings.value("canIndex",0).toInt());
    ui->comboBoxWorkModel1->setCurrentIndex(
            settings.value("workModel",0).toInt());
    ui->comboBoxBaudrate->setCurrentIndex(
            settings.value("baudRate",0).toInt());
    ui->lineEditTimer0->setText(
                settings.value("timing0","0x04").toString());
    ui->lineEditTimer1->setText(
                settings.value("timing1","0x1C").toString());

    getValueFrmUi();
}

DevSetDialog::~DevSetDialog()
{
    delete ui;
}

void DevSetDialog::on_comboBoxBaudrate_activated(const QString &arg1)
{
    if("100kbps" == arg1)
    {
        ui->lineEditTimer0->setText("0x04");
        ui->lineEditTimer1->setText("0x1C");
    }
    else if("250kbps" == arg1)
    {
        ui->lineEditTimer0->setText("0x01");
        ui->lineEditTimer1->setText("0x1C");
    }
    else if("500kbps" == arg1)
    {
        ui->lineEditTimer0->setText("0x00");
        ui->lineEditTimer1->setText("0x1C");
    }
}

//从参数设置ui界面获取CAN的参数
void DevSetDialog::getValueFrmUi(void)
{
    QSettings settings;

    VCI_INIT_CONFIG tempConfig;
    memset(&tempConfig,0,sizeof(VCI_INIT_CONFIG));

    int index = ui->comboBoxBaudrate->currentText().indexOf("k");
    QString strBaudrate = ui->comboBoxBaudrate->currentText().mid(0,index);
    int baudrate = strBaudrate.toInt();
    m_can->setBaudrate(baudrate);
    m_can->setDevIndex(ui->comboBoxDecIndex->currentText().toInt());

    if("USBCAN1" == ui->comboBoxDevType->currentText())
    {
        m_can->setDevType(3);
    }
    else if("USBCAN2" == ui->comboBoxDevType->currentText())
    {
        m_can->setDevType(4);
    }

    //初始化配置参数
    bool ok;
    m_can->setCanIndex(ui->comboBoxCanIndex->currentText().toInt());
    tempConfig.Timing0 = ui->lineEditTimer0->text().toInt(&ok,16);
    tempConfig.Timing1 = ui->lineEditTimer1->text().toInt(&ok,16);
    tempConfig.Mode = (ui->comboBoxWorkModel1->currentText() == "正常" ? 0 : 1);
    tempConfig.AccMask = 0xFFFFFFFF;
    m_can->setInitConfig(&tempConfig);

    settings.setValue("devType",ui->comboBoxDevType->currentIndex());
    settings.setValue("devIndex",ui->comboBoxDecIndex->currentIndex());
    settings.setValue("canIndex",ui->comboBoxCanIndex->currentIndex());
    settings.setValue("workModel",ui->comboBoxWorkModel1->currentIndex());
    settings.setValue("baudRate",ui->comboBoxBaudrate->currentIndex());
    settings.setValue("timing0",ui->lineEditTimer0->text());
    settings.setValue("timing1",ui->lineEditTimer1->text());
#ifdef DEBUG
    qDebug() << "DevType:" << m_can->getDevType();
    qDebug() << "DevIndex:" << m_can->getDevIndex();
    qDebug() << "CanIndex:" << m_can->getCanIndex();
    qDebug() << "Baudrate:" << m_can->getBaudrate();

    qDebug() << "Timing0:" << m_can->getInitConfig()->Timing0;
    qDebug() << "Timing1:" << m_can->getInitConfig()->Timing1;
    qDebug() << "Mode:" << m_can->getInitConfig()->Mode;
#endif
}

void DevSetDialog::on_pushButtonOK_clicked()
{
    getValueFrmUi();
    done(QDialog::Accepted);
}

void DevSetDialog::devSetButtonClicked()
{
    if(m_can->getIsOpen() && m_can->getIsStart())
    {
        ui->pushButtonOK->setEnabled(false);
        ui->comboBoxBaudrate->setEnabled(false);
        ui->comboBoxCanIndex->setEnabled(false);
        ui->comboBoxDecIndex->setEnabled(false);
        ui->comboBoxDevType->setEnabled(false);
        ui->comboBoxWorkModel1->setEnabled(false);
    }
    else
    {
        ui->pushButtonOK->setEnabled(true);
        ui->comboBoxBaudrate->setEnabled(true);
        ui->comboBoxCanIndex->setEnabled(true);
        ui->comboBoxDecIndex->setEnabled(true);
        ui->comboBoxDevType->setEnabled(true);
        ui->comboBoxWorkModel1->setEnabled(true);
    }
}

