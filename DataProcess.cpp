#include "DataProcess.h"
#include <QTimer>
#include <QDebug>
#include "CanBus.h"
#include "QVector"
#include <QStandardItemModel>
#include <QDateTime>
#include <QMutex>
#include <QTableView>
#include <QStandardItemModel>
#include "TableView2Excel.h"
#include "Header.h"
#include <QMessageBox>
#include <QCoreApplication>

DataProcess::DataProcess()
{
    m_timer = new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimeout()));

    m_can = CanBus::getInstance();
    m_requestNum = 0;

    m_tableView2excel = new TableView2Excel();
}

void DataProcess::onTimeout()
{
    sendGetDataRequest();
    //接收数据
    receiveData();
}

int DataProcess::receiveData(void)
{
    static VCI_CAN_OBJ dataTemp[64];
    static int devType = m_can->getDevType();
    static int devIndex = m_can->getDevIndex();
    static int canIndex = m_can->getCanIndex();

    int num = m_can->CanGetReceiveNum(devType,devIndex,canIndex);

    unsigned int ret = m_can->CanReceive(devType,devIndex,canIndex,dataTemp,(num>64)?64:num,100);
    m_can->CanClearBuffer(devType,devIndex,canIndex);
    if(ret != 0xFFFFFFFF && ret != 0)
    {
        for(unsigned int i=0;i<ret && i<64;i++)
        {
            m_dataRecv.insert(dataTemp[i].ID,dataTemp[i]);
            emit datRecSignal(dataTemp[i]);
        }
        QCoreApplication::processEvents();

    }
    else
    {
        VCI_ERR_INFO errinfo;
        m_can->CanReadErrInfo(devType,devIndex,canIndex,&errinfo);
    }
    return ret;
}

void DataProcess::sendGetDataRequest()
{
    static int devType = m_can->getDevType();
    static int devIndex = m_can->getDevIndex();
    static int canIndex = m_can->getCanIndex();
    static VCI_CAN_OBJ sendFrame;

    sendFrame.SendType = 0;
    sendFrame.RemoteFlag = 1;
    sendFrame.ExternFlag = 1;
    sendFrame.DataLen = 0;

    switch(m_requestNum++)
    {
    case 0:
        sendFrame.ID = REQ_PACK_PRA;
        break;
    case 1:
        sendFrame.ID = REQ_READ_COC;
        break;
    case 2:
        sendFrame.ID = REQ_READ_DOC;
        break;
    case 3:
        sendFrame.ID = REQ_READ_COT;
        break;
    case 4:
        sendFrame.ID = REQ_READ_DOT;
        break;
    case 5:
        sendFrame.ID = REQ_READ_CUT;
        break;
    case 6:
        sendFrame.ID = REQ_READ_DUT;
        break;
    case 7:
        sendFrame.ID = REQ_READ_OUC;
        break;
    case 8:
        sendFrame.ID = REQ_READ_IMB;
        break;
    case 9:
        sendFrame.ID = REQ_READ_NOR;
        break;
    case 10:
        sendFrame.ID = REQ_FALT_OC;
        break;
    case 11:
        sendFrame.ID = REQ_FALT_OUV;
        break;
    case 12:
        sendFrame.ID = REQ_FALT_COUT;
        break;
    case 13:
        sendFrame.ID = REQ_FALT_DOUT;
        break;
    case 14:
        sendFrame.ID = REQ_FALT_HARD;
        break;
    default:
        m_requestNum = 0;
    }

    m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame,1);
}

int DataProcess::dataStore(QTableView *tableView,QStandardItemModel *model)
{
    if(m_tableView2excel->ExportToExcel(tableView,model))
        return model->rowCount();
    else
        return 0;
}

short DataProcess::getCurrent()
{
    if (m_dataRecv.contains(0x180250F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180250F4);
        ushort ret = (ushort)((dat.Data[3]<<8) | dat.Data[2]);

        return ret;
    }
    else
        return 0;
}

uint16_t DataProcess::getTotalVoltage()
{
    if (m_dataRecv.contains(BRO_BATT_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_BATT_INFO);
        uint16_t ret = 0x00;
        ret = (uint8_t)dat.Data[1];
        ret = (ret << 8);
        ret |= (uint8_t)dat.Data[0];
        ret /= 10;

        return ret;
    }
    else
        return 0;
}

uchar DataProcess::getSoc()
{
    if (m_dataRecv.contains(BRO_BATT_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_BATT_INFO);

        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        return data[4];
    }
    else
        return 0;
}

void DataProcess::getCellsVoltage(float cellVol[],int len)
{
    if(m_dataRecv.contains(BRO_CELL_VOLT1) && m_dataRecv.contains(BRO_CELL_VOLT2)
            && m_dataRecv.contains(BRO_CELL_VOLT3) && m_dataRecv.contains(BRO_CELL_VOLT4)
            && m_dataRecv.contains(BRO_CELL_VOLT5))
    {
        VCI_CAN_OBJ dat1 = m_dataRecv.value(BRO_CELL_VOLT1);
        VCI_CAN_OBJ dat2 = m_dataRecv.value(BRO_CELL_VOLT2);
        VCI_CAN_OBJ dat3 = m_dataRecv.value(BRO_CELL_VOLT3);
        VCI_CAN_OBJ dat4 = m_dataRecv.value(BRO_CELL_VOLT4);
        VCI_CAN_OBJ dat5 = m_dataRecv.value(BRO_CELL_VOLT5);

        unsigned char data1[8] = {0};
        unsigned char data2[8] = {0};
        unsigned char data3[8] = {0};
        unsigned char data4[8] = {0};
        unsigned char data5[8] = {0};

        memcpy(data1,dat1.Data,dat1.DataLen);
        memcpy(data2,dat2.Data,dat2.DataLen);
        memcpy(data3,dat3.Data,dat3.DataLen);
        memcpy(data4,dat4.Data,dat4.DataLen);
        memcpy(data5,dat5.Data,dat5.DataLen);

        if(20 == len)
        {
            cellVol[0] =  (float)((data1[1]<<8)+(data1[0]))*0.001;
            cellVol[1] =  (float)((data1[3]<<8)+(data1[2]))*0.001;
            cellVol[2] =  (float)((data1[5]<<8)+(data1[4]))*0.001;
            cellVol[3] =  (float)((data1[7]<<8)+(data1[6]))*0.001;

            cellVol[4] =  (float)((data2[1]<<8)+(data2[0]))*0.001;
            cellVol[5] =  (float)((data2[3]<<8)+(data2[2]))*0.001;
            cellVol[6] =  (float)((data2[5]<<8)+(data2[4]))*0.001;
            cellVol[7] =  (float)((data2[7]<<8)+(data2[6]))*0.001;

            cellVol[8] =  (float)((data3[1]<<8)+(data3[0]))*0.001;
            cellVol[9] =  (float)((data3[3]<<8)+(data3[2]))*0.001;
            cellVol[10] = (float)((data3[5]<<8)+(data3[4]))*0.001;
            cellVol[11] = (float)((data3[7]<<8)+(data3[6]))*0.001;

            cellVol[12] = (float)((data4[1]<<8)+(data4[0]))*0.001;
            cellVol[13] = (float)((data4[3]<<8)+(data4[2]))*0.001;
            cellVol[14] = (float)((data4[5]<<8)+(data4[4]))*0.001;
            cellVol[15] = (float)((data4[7]<<8)+(data4[6]))*0.001;

            cellVol[16] = (float)((data5[1]<<8)+(data5[0]))*0.001;
            cellVol[17] = (float)((data5[3]<<8)+(data5[2]))*0.001;
            cellVol[18] = (float)((data5[5]<<8)+(data5[4]))*0.001;
            cellVol[19] = (float)((data5[7]<<8)+(data5[6]))*0.001;
        }
    }
    else
    {
        if(20 == len)
        {
            cellVol[0] = 0;
            cellVol[1] = 0;
            cellVol[2] = 0;
            cellVol[3] = 0;
            cellVol[4] = 0;
            cellVol[5] = 0;
            cellVol[6] = 0;
            cellVol[7] = 0;
            cellVol[8] = 0;
            cellVol[9] = 0;
            cellVol[10] = 0;
            cellVol[11] = 0;
            cellVol[12] = 0;
            cellVol[13] = 0;
            cellVol[14] = 0;
            cellVol[15] = 0;
            cellVol[16] = 0;
            cellVol[17] = 0;
            cellVol[18] = 0;
            cellVol[19] = 0;
        }
    }
}

float DataProcess::getMaxCellVoltage()
{
    if (m_dataRecv.contains(BRO_CELLV_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLV_INFO);
        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);
        float ret = (float)((data[1] << 8) + (data[0])) * 0.001;

        return ret;
    }
    else
        return 0;
}

float DataProcess::getMinCellVoltage()
{
    if (m_dataRecv.contains(BRO_CELLV_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLV_INFO);
        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        float ret = (float)((data[3] << 8) + (data[2]))*0.001;

        return ret;
    }
    else
        return 0;
}

char DataProcess::getMaxCellTemp()
{
    if (m_dataRecv.contains(BRO_CELLT_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLT_INFO);

        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        return data[0];
    }
    else
        return 0;
}

char DataProcess::getMinCellTemp()
{
    if (m_dataRecv.contains(BRO_CELLT_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLT_INFO);

        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        return data[1];
    }
    else
        return 0;
}


uchar DataProcess::getMaxCellVolNum()
{
    if (m_dataRecv.contains(BRO_CELLV_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLV_INFO);

        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        return data[4];
    }
    else
        return 0;
}

uchar DataProcess::getMinCellVolNum()
{
    if (m_dataRecv.contains(BRO_CELLV_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLV_INFO);
        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);
        return data[5];
    }
    else
        return 0;

}

ushort DataProcess::getCellVolt(uchar num)
{
    if( num<20 && num>=0)
    {
        getCellsVoltage(m_cellsVolt,20);
        return m_cellsVolt[num];
    }
    else
    {
        QMessageBox::warning(NULL,QString("Get Cell Voltage"),
                             QString("Get Cell Voltage Error,Parameter num Out of Range"),
                             QMessageBox::Ok);
        return 0;
    }
}

char DataProcess::getCellTemp(uchar num)
{
    if(num<4 && num>=0)
    {
        if(m_dataRecv.contains(BRO_TEMP_01))
        {
            static VCI_CAN_OBJ dat = m_dataRecv.value(BRO_TEMP_01);

            unsigned char data[8] = {0};
            memcpy(data,dat.Data,dat.DataLen);

            return data[num];
        }
        else
        {
            return 0;
        }
    }
    else
    {
        QMessageBox::warning(NULL,QString("Get Cell Temp"),
                             QString("Get Cell Temp Error,Parameter num Out of Range"),
                             QMessageBox::Ok);
        return 0;
    }
}


uchar DataProcess::getMinCellTempNum()
{
    if (m_dataRecv.contains(BRO_CELLT_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLT_INFO);

        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        return data[5];
    }
    else
        return 0;

}

uchar DataProcess::getMaxCellTempNum()
{
    if (m_dataRecv.contains(BRO_CELLT_INFO))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(BRO_CELLT_INFO);

        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        return data[3];
    }
    else
        return 0;
}

ushort DataProcess::getChgCircNum()//充放电循环次数
{
    if (m_dataRecv.contains(REC_PACK_SOH))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_PACK_SOH);

        unsigned char data[8] = {0};
        memcpy(data,dat.Data,dat.DataLen);

        return (data[1]<<8)|(data[0]);
    }
    else
        return 0;
}

void DataProcess::getPackPra(unsigned char *buf,int len)
{
    if(m_dataRecv.contains(REC_PACK_PRA) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_PACK_PRA);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getPackCOC(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_COC) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_COC);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getPackDOC(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_DOC) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_DOC);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getPackCOT(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_COT) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_COT);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getPackDOT(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_DOT) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_DOT);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getPackCUT(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_CUT) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_CUT);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getPackDUT(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_DUT) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_DUT);

        memcpy(buf,dat.Data,len);
    }
}

// 电芯的高低压设置
void DataProcess::getCellOUC(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_OUC) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_OUC);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getCellIBM(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_IMB) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_IMB);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getPackDIT(unsigned char *buf, int len)
{

}

void DataProcess::getPackOV(unsigned char *buf, int len)
{

}
void DataProcess::DataProcess::getPackUV(unsigned char *buf, int len)
{

}

void DataProcess::getNorRec(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_READ_NOR) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_READ_NOR);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getOCRec(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_FALT_OC) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_FALT_OC);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getOUVRec(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_FALT_OUV) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_FALT_OUV);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getCOUTRec(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_FALT_COUT) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_FALT_COUT);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getDOUTRec(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_FALT_DOUT) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_FALT_DOUT);

        memcpy(buf,dat.Data,len);
    }
}

void DataProcess::getLtcRec(unsigned char *buf, int len)
{
    if(m_dataRecv.contains(REC_FALT_HARD) && (8 == len))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(REC_FALT_HARD);

        memcpy(buf,dat.Data,len);
    }
}

