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
    //sendGetDataRequest();
    receiveData();
}

int DataProcess::receiveData()
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
        //qDebug() << errinfo.ErrCode;
    }
    return ret;
}

void DataProcess::sendGetDataRequest()
{
    static int devType = m_can->getDevType();
    static int devIndex = m_can->getDevIndex();
    static int canIndex = m_can->getCanIndex();
    static VCI_CAN_OBJ sendFrame[1];

    sendFrame[0].SendType = 0;
    sendFrame[0].RemoteFlag = 1;
    sendFrame[0].ExternFlag = 1;
    sendFrame[0].DataLen = 0;

    switch(m_requestNum++)
    {
    case 0:
        sendFrame[0].ID = BMS_INFO;
        break;
    case 1:
        sendFrame[0].ID = CELL_INFO;
        break;
    case 2:
        sendFrame[0].ID = TEMPERTURE;
        break;
    case 3:
        sendFrame[0].ID = BMS_VERSION;
        break;
    case 4:
        sendFrame[0].ID = PACK_INFO;
        break;
    case 5:
        sendFrame[0].ID = CCURRENT_OVER;
        break;
    case 6:
        sendFrame[0].ID = PCURRENT_OVET;
        break;
    case 7:
        sendFrame[0].ID = CTEMP_OVER;
        break;
    case 8:
        sendFrame[0].ID = PTEMP_OVER;
        break;
    case 9:
        sendFrame[0].ID = CTEMP_LOW;
        break;
    case 10:
        sendFrame[0].ID = PTEMP_LOW;
        break;
    case 11:
        sendFrame[0].ID = CELLVOLO;
        break;
    case 12:
        sendFrame[0].ID = WRITEMCU;
        break;
    case 13:
        sendFrame[0].ID = READMCU;
        break;
    case 14:
        sendFrame[0].ID = READDATA;
        break;
    default:
        m_requestNum = 0;
        break;
    }
    if(m_can->CanTransmit(devType,devIndex,canIndex,sendFrame,1))
        qDebug() << "sendGetDataRequest success";
    else
        qDebug() << "sendGetDataRequest error";
}


int DataProcess::dataAnalyze()
{
    return 0;
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
    if (m_dataRecv.contains(0x180250F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180250F4);
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
    if (m_dataRecv.contains(0x180250F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180250F4);

        return dat.Data[4];
    }
    else
        return 0;
}

void DataProcess::getCellsVoltage(float cellVol[],int len)
{
    if(m_dataRecv.contains(0x181050F4) && m_dataRecv.contains(0x181150F4)
            && m_dataRecv.contains(0x181250F4) && m_dataRecv.contains(0x181350F4)
            && m_dataRecv.contains(0x181450F4))
    {
        VCI_CAN_OBJ dat1 = m_dataRecv.value(0x181050F4);
        VCI_CAN_OBJ dat2 = m_dataRecv.value(0x181150F4);
        VCI_CAN_OBJ dat3 = m_dataRecv.value(0x181250F4);
        VCI_CAN_OBJ dat4 = m_dataRecv.value(0x181350F4);
        VCI_CAN_OBJ dat5 = m_dataRecv.value(0x181450F4);
        if(20 == len)
        {
            cellVol[0] =  (float)((dat1.Data[1]<<8)+(dat1.Data[0]))*0.001;
            cellVol[1] =  (float)((dat1.Data[3]<<8)+(dat1.Data[2]))*0.001;
            cellVol[2] =  (float)((dat1.Data[5]<<8)+(dat1.Data[4]))*0.001;
            cellVol[3] =  (float)((dat1.Data[7]<<8)+(dat1.Data[6]))*0.001;

            cellVol[4] =  (float)((dat2.Data[1]<<8)+(dat2.Data[0]))*0.001;
            cellVol[5] =  (float)((dat2.Data[3]<<8)+(dat2.Data[2]))*0.001;
            cellVol[6] =  (float)((dat2.Data[5]<<8)+(dat2.Data[4]))*0.001;
            cellVol[7] =  (float)((dat2.Data[7]<<8)+(dat2.Data[6]))*0.001;

            cellVol[8] =  (float)((dat3.Data[1]<<8)+(dat3.Data[0]))*0.001;
            cellVol[9] =  (float)((dat3.Data[3]<<8)+(dat3.Data[2]))*0.001;
            cellVol[10] = (float)((dat3.Data[5]<<8)+(dat3.Data[4]))*0.001;
            cellVol[11] = (float)((dat3.Data[7]<<8)+(dat3.Data[6]))*0.001;

            cellVol[12] = (float)((dat4.Data[1]<<8)+(dat4.Data[0]))*0.001;
            cellVol[13] = (float)((dat4.Data[3]<<8)+(dat4.Data[2]))*0.001;
            cellVol[14] = (float)((dat4.Data[5]<<8)+(dat4.Data[4]))*0.001;
            cellVol[15] = (float)((dat4.Data[7]<<8)+(dat4.Data[6]))*0.001;

            cellVol[16] = (float)((dat5.Data[1]<<8)+(dat5.Data[0]))*0.001;
            cellVol[17] = (float)((dat5.Data[3]<<8)+(dat5.Data[2]))*0.001;
            cellVol[18] = (float)((dat5.Data[5]<<8)+(dat5.Data[4]))*0.001;
            cellVol[19] = (float)((dat5.Data[7]<<8)+(dat5.Data[6]))*0.001;
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
    if (m_dataRecv.contains(0x180350F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180350F4);
        float ret = (float)((dat.Data[1] << 8) + (dat.Data[0])) * 0.001;

        return ret;
    }
    else
        return 0;
}

float DataProcess::getMinCellVoltage()
{
    if (m_dataRecv.contains(0x180350F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180350F4);

        float ret = (float)((dat.Data[3] << 8) + (dat.Data[2]))*0.001;

        return ret;
    }
    else
        return 0;
}

char DataProcess::getMaxCellTemp()
{
    if (m_dataRecv.contains(0x180450F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180450F4);

        return dat.Data[0];
    }
    else
        return 0;
}

char DataProcess::getMinCellTemp()
{
    if (m_dataRecv.contains(0x180450F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180450F4);

        return dat.Data[1];
    }
    else
        return 0;
}


uchar DataProcess::getMaxCellVolNum()
{
    if (m_dataRecv.contains(0x180350F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180350F4);

        return dat.Data[4];
    }
    else
        return 0;
}

uchar DataProcess::getMinCellVolNum()
{
    if (m_dataRecv.contains(0x180350F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180350F4);

        return dat.Data[5];
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
        if(m_dataRecv.contains(0x182050F4))
        {
            static VCI_CAN_OBJ dat = m_dataRecv.value(0x182050F4);
            return dat.Data[num];
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
    if (m_dataRecv.contains(0x180450F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180350F4);

        return dat.Data[5];
    }
    else
        return 0;

}

uchar DataProcess::getMaxCellTempNum()
{
    if (m_dataRecv.contains(0x180450F4))
    {
        VCI_CAN_OBJ dat = m_dataRecv.value(0x180350F4);

        return dat.Data[4];
    }
    else
        return 0;

}


ushort DataProcess::getChgCircNum()//充放电循环次数
{
    return 0;
}

ushort DataProcess::getChgCurOverNum()      //充电过流故障发生次数
{
    return 0;
}

ushort DataProcess::getCellVoltOverNum()    //Cell过压故障次数
{
    return 0;
}


