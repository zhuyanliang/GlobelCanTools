#include "DownLoadUi.h"
#include "ui_DownLoadUi.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "ParseHex.h"
#include "CanBus.h"
#include "Header.h"
#include <QDebug>

/*-------------- CRC lookup table --------------*/
static uint8_t const Crc8Table[256] =
{
   0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,
   0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
   0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,
   0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
   0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,
   0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
   0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,
   0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
   0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,
   0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
   0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,
   0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
   0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,
   0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
   0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,
   0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
   0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,
   0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
   0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,
   0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
   0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,
   0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
   0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,
   0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
   0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,
   0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
   0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,
   0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
   0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,
   0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
   0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,
   0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};

//----------------------------------------------------------------------------
// Function:    calculate_crc8
// Description: 计算8位长度len字符串的CRC
// Parameters:  *ptr,len
// Returns:     crc value
//----------------------------------------------------------------------------
uint8_t calculate_crc8(uint8_t* ptr, uint8_t len)
{
   uint8_t crc = 0;

   while(len--)
      crc = Crc8Table[crc ^ *ptr++];
   return crc;
}

static unsigned int CAN_Generate_ID(unsigned char msg_fc, unsigned char crc)
{
    unsigned int temp = 0;

    temp = ((unsigned int)msg_fc << 16) + ((unsigned int)crc << 8) + GUI;
    return(temp);
}

//----------------------------------------------------------------------------
// Function:    calculate_char_crc8
// Description:  计算单个字符数值的CRC
// Parameters:  crc,nbyte
// Returns:     crc value
//----------------------------------------------------------------------------
uint8_t calculate_char_crc8(uint8_t crc, uint8_t nbyte)
{
   return(Crc8Table[crc ^ nbyte]);
}

DownLoadUi::DownLoadUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownLoadUi)
{
    ui->setupUi(this);
    setFixedSize(width(),height());
    m_can = CanBus::getInstance();
    filledIndex = 0;
    m_timer = NULL;
    m_progress = NULL;
    m_parse = NULL;
}

DownLoadUi::~DownLoadUi()
{
    delete ui;
}

void DownLoadUi::on_chooseHexPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"),"",tr("Hexfile(*.hex);;Allfile(*.*)"));

    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        //以只读方式打开文件，如果打开失败则返回，弹出对话框
        if(!file.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::warning(this,tr("读取文件"),
              tr("无法读取文件 %1:\n %2.").arg(fileName).arg(file.errorString()));
        }

        ui->chooseFilePathLineEdit->setText(QFileInfo(fileName).canonicalFilePath());
        //将文本设置为可见，此句话必须放在return true前，放在其后就见不到了，因为return语句返回了，此句执行不到
        ui->chooseFilePathLineEdit->setVisible(true);
    }

}

void DownLoadUi::on_downLoadPushButton_clicked()
{
    char erasedata[8] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
    int devType = m_can->getDevType();
    int devIndex = m_can->getDevIndex();
    int canIndex = m_can->getCanIndex();

    const QString name = ui->chooseFilePathLineEdit->text();
    m_parse = new ParseHex(name,":020000040020DA",":020000040030CA",":00000001FF");

    if(!m_parse->StartParse()) //解析
    {
        return;
    }
    else
    {
        qDebug() << "Hex文件解析成功";

    }

    m_timer = new QTimer;

    m_timer->setInterval(2);
    m_timer->start();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimerout()));

    m_progress = new QProgressDialog("Download files...", "Cancel", 0, m_parse->GetByteNums(), this);
    m_progress->setWindowModality(Qt::WindowModal);
    connect(m_progress, SIGNAL(canceled()), this, SLOT(close()));
    m_progress->show();

    //给单片机发送擦除应用程序头命令
    m_canData.SendType = 0;
    m_canData.RemoteFlag = 0;
    m_canData.ExternFlag = 1;
    m_canData.DataLen = 0x08;
    m_canData.ID = CAN_Generate_ID(CAN_MSG_IMAGE_ERASE,BMS);
    for(int i=0;i<8;i++)
    {
        m_canData.Data[i] = erasedata[i];
    }

    if(!m_can->CanTransmit(devType,devIndex,canIndex,&m_canData,1))
        QMessageBox::warning(this,tr("发送下载命令失败"),
          tr("发送下载命令 %1.").arg(ui->chooseFilePathLineEdit->text()));
    else
        qDebug() << "发送下载命令成功";
}

void DownLoadUi::onTimerout()
{
    unsigned int Id = 0x00;
    VCI_CAN_OBJ dataTemp;
    static int devType = m_can->getDevType();
    static int devIndex = m_can->getDevIndex();
    static int canIndex = m_can->getCanIndex();

    unsigned int ret = m_can->CanReceive(devType,devIndex,canIndex,&dataTemp,1,100);
    m_can->CanClearBuffer(devType,devIndex,canIndex);
    if((ret != 0xFFFFFFFF) && (ret != 0))
    {
        qDebug() << "Receive Data";
        if(dataTemp.RemoteFlag == 1)
        {
            Id = dataTemp.ID;
            qDebug() << "Removete data";
        }

        QCoreApplication::processEvents();
    }
    else
    {
        VCI_ERR_INFO errinfo;
        m_can->CanReadErrInfo(devType,devIndex,canIndex,&errinfo);
        qDebug() << "Can Receive Error: " << errinfo.ErrCode;
        return;
    }
    qDebug() << "dataTemp.ID: " << hex << dataTemp.ID;
    qDebug() << "Id: " << hex << Id;

    switch((Id>>16)&0xFF)
    {
    case CAN_MSG_IMAGE_REQUEST:  //请求App下载数据
        //发送擦除App命令
        FillFuncData(CAN_MSG_IMAGE_ERASE);
        qDebug() << "Request data";
        break;
    case CAN_MSG_REQ_NEXT:  // 请求下一帧数据
        FillCodeData();
        qDebug() << "Next";
        break;
    case CAN_MSG_REQ_AGAIN: // 请求重新发送该帧数据
        qDebug() << "Again";
        break;
    default:
        qDebug() << "default";
        break;
    }
    if(!m_can->CanTransmit(devType,devIndex,canIndex,&m_canData,1))
        QMessageBox::warning(this,tr("发送失败"),
          tr("发送Hex文件数据失败 %1.").arg(ui->chooseFilePathLineEdit->text()));
    m_progress->setValue(filledIndex);

}

void DownLoadUi::FillFuncData(unsigned char dat)
{
    if(CAN_MSG_OVER_LOAD == dat)
    {
        unsigned short nums = m_parse->GetByteNums();
        unsigned short crcTotal = m_parse->GetCrc();
        unsigned char crc = 0;
        m_canData.ID = 0;
        m_canData.DataLen = 0x8;
        m_canData.ExternFlag = 1;
        m_canData.RemoteFlag = 0;
        m_canData.SendType = 0;
        // 代码中字符个数
        m_canData.Data[0] = nums&0xff;
        m_canData.Data[1] = (nums>>8)&0xff;
        // 所有代码的CRC
        m_canData.Data[2] = crcTotal&0xff;
        m_canData.Data[3] = (crcTotal>>8)&0xff;

        crc = calculate_crc8((unsigned char*)&m_canData.Data[0],m_canData.DataLen);
        m_canData.ID = CAN_Generate_ID(CAN_MSG_OVER_LOAD,crc);
    }
    else
    {
        m_canData.SendType = 0;
        m_canData.ID = 0;
        m_canData.DataLen = 0;
        m_canData.ExternFlag = 1;
        m_canData.RemoteFlag = 1;
        m_canData.ID = CAN_Generate_ID(dat,'C');
    }
}

void DownLoadUi::FillCodeData()
{
    static unsigned long lastNums;
    static unsigned char crc = 0;
    unsigned long i;

    lastNums = m_parse->GetByteNums() - filledIndex;
    if(lastNums >=8)
    {
        for(i=0;i<8;i++)
        {
            m_canData.Data[i] = m_parse->GetBinData(filledIndex++);
        }
        m_canData.DataLen = 8;
    }
    else if(lastNums > 0)
    {
        for(i=0;i<lastNums;i++)
        {
            m_canData.Data[i] = m_parse->GetBinData(filledIndex++);
        }
        m_canData.DataLen = lastNums;
    }
    else
    {
        // 数据发送完毕后，发送结束标志
        FillFuncData(CAN_MSG_OVER_LOAD);
        filledIndex = 0;
        this->close();
        return;
    }

    //发送数据的CRC
    crc = calculate_crc8((unsigned char*)&m_canData.Data[0],m_canData.DataLen);

    m_canData.ExternFlag = 1;
    m_canData.RemoteFlag = 0;
    m_canData.ID = CAN_Generate_ID(CAN_MSG_IMAGE_LOAD,crc);
}
