#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QTimer>
#include <QMap>
#include "ControlCAN.h"
#include <QMutex>

#define BMS_VERSION     0x1800F5FD
#define BMS_INFO        0x1801F5FD  //电压、总电流、故障报警状态等
#define CELL_INFO       0x1802F5FD
#define CELL_VOLT_1     0x1803F5FD
#define CELL_VOLT_2     0x1804F5FD
#define CELL_VOLT_3     0x1805F5FD
#define CELL_VOLT_4     0x1806F5FD
#define CELL_VOLT_5     0x1807F5FD
#define PACKSOH         0x180CF5FD
#define PACKWARINF      0x180DF5FD
#define TEMPERTURE      0x180EF5FD
#define PACK_INFO       0x1820F5FD
#define CCURRENT_OVER   0x1822F5FD
#define PCURRENT_OVET   0x1823F5FD
#define CTEMP_OVER      0x1824F5FD
#define PTEMP_OVER      0x1825F5FD
#define CTEMP_LOW       0x1826F5FD
#define PTEMP_LOW       0x1827F5FD
#define CELLVOLO        0x1828F5FD
#define WRITEMCU        0x1830F5FD
#define READMCU         0x1831F5FD
#define READDATA        0x1840F5FD

#define R_BMS_VERSION     0x1800FDF5
#define R_BMS_INFO        0x1801FDF5  //电压、总电流、故障报警状态等
#define R_CELL_INFO       0x1802FDF5
#define R_CELL_VOLT_1     0x1803FDF5
#define R_CELL_VOLT_2     0x1804FDF5
#define R_CELL_VOLT_3     0x1805FDF5
#define R_CELL_VOLT_4     0x1806FDF5
#define R_CELL_VOLT_5     0x1807FDF5
#define R_ACKSOH          0x180CFDF5
#define R_PACKWARINF      0x180DFDF5
#define R_TEMPERTURE      0x180EFDF5
#define R_PACK_INFO       0x1820FDF5
#define R_CCURRENT_OVER   0x1822FDF5
#define R_PCURRENT_OVET   0x1823FDF5
#define R_CTEMP_OVER      0x1824FDF5
#define R_PTEMP_OVER      0x1825FDF5
#define R_CTEMP_LOW       0x1826FDF5
#define R_PTEMP_LOW       0x1827FDF5
#define R_CELLVOLO        0x1828FDF5
#define R_WRITEMCU        0x1830FDF5
#define R_READMCU         0x1831FDF5
#define R_READDATA        0x1840FDF5

class CanBus;
class QTableView;
class QStandardItemModel;
class TableView2Excel;

class DataProcess : public QObject
{
    Q_OBJECT
private:
    QTimer *m_timer;
    CanBus *m_can;
    QMap<uint32_t,VCI_CAN_OBJ> m_dataRecv;
    int m_requestNum;
    TableView2Excel *m_tableView2excel;

    float m_cellsVolt[20];

public:
    explicit DataProcess();
    int receiveData();
    int dataAnalyze();
    int dataStore(QTableView *tableView, QStandardItemModel *model);
    short getCurrent();
    uint16_t getTotalVoltage();
    uchar getSoc();

    float getMaxCellVoltage();
    float getMinCellVoltage();
    char getMinCellTemp();
    char getMaxCellTemp();
    uchar getMaxCellVolNum();
    uchar getMinCellVolNum();
    uchar getMaxCellTempNum();
    uchar getMinCellTempNum();
    ushort getCellVolt(uchar num);
    char getCellTemp(uchar num);
    ushort getChgCircNum();         //充放电循环次数
    ushort getChgCurOverNum();      //充电过流故障发生次数
    ushort getCellVoltOverNum();    //Cell过压故障次数

    void getCellsVoltage(float cellVol[], int len);//获取所有电芯实时电压

    void timerStart(void){m_timer->start();}
    void timerStop(void){m_timer->stop();}
    void setTimerInterval(int t){m_timer->setInterval(t);}
    void sendGetDataRequest();
    QMap<uint,VCI_CAN_OBJ>& getAllData(){return m_dataRecv;}

signals:
    void datRecSignal(VCI_CAN_OBJ &);
    
public slots:
    void onTimeout(); 
};

#endif // DATAPROCESS_H
