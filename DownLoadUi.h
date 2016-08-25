#ifndef DOWNLOADUI_H
#define DOWNLOADUI_H

#include <QDialog>
#include <QTimer>
#include <QProgressDialog>
#include "ControlCAN.h"

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
