#ifndef DOWNLOADUI_H
#define DOWNLOADUI_H

#include <QDialog>
#include <QTimer>
#include <QProgressDialog>
#include "ControlCAN.h"

//Commands
#define WR_END  0x01 //
#define WR_MEM  0x02 //写程序存储器
#define ER_MEM  0x03 //擦除程序存储器
#define RUN_APP 0x04

#define GUI             ((unsigned char)0xFD)
#define BMS             ((unsigned char)0xF4)

//Function
#define CAN_MSG_IMAGE_ERASE     ((unsigned char)0xF0)
#define CAN_MSG_IMAGE_REQUEST   ((unsigned char)0xF1) //请求App下载数据
#define CAN_MSG_IMAGE_LOAD      ((unsigned char)0xF2)
#define CAN_MSG_OVER_LOAD       ((unsigned char)0xF3) // 下载数据结束标志
#define CAN_MSG_REQ_NEXT        ((unsigned char)0xF4) // 请求下一帧数据
#define CAN_MSG_REQ_AGAIN       ((unsigned char)0xF5) // 请求重新发送该帧数据

class ParseHex;
class CanBus;

namespace Ui {
class DownLoadUi;
}

class DownLoadUi : public QDialog
{
    Q_OBJECT
    
public:
    explicit DownLoadUi(QWidget *parent = 0);
    ~DownLoadUi();
    void FillFuncData(unsigned char dat);
    void FillCodeData();
    unsigned long   filledIndex;
    VCI_CAN_OBJ     m_canData;
    
private slots:
    void on_chooseHexPushButton_clicked();

    void on_downLoadPushButton_clicked();

    void onTimerout();

private:
    Ui::DownLoadUi  *ui;
    QTimer          *m_timer;
    QProgressDialog *m_progress;
    ParseHex        *m_parse;
    CanBus          *m_can;

};

#endif // DOWNLOADUI_H
