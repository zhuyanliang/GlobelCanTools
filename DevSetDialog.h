#ifndef DEVSETDIALOG_H
#define DEVSETDIALOG_H

#include <QDialog>
#include "CanBus.h"

namespace Ui {
class DevSetDialog;
}

class DevSetDialog : public QDialog
{
    Q_OBJECT
private:
    CanBus *m_can;
public:
    explicit DevSetDialog(QWidget *parent = 0);
    ~DevSetDialog();
    void getValueFrmUi(void);
    CanBus* getCan(){return m_can;}
    
private slots:
    void on_comboBoxBaudrate_activated(const QString &arg1);

    void on_pushButtonOK_clicked();

public slots:
    void devSetButtonClicked();

private:
    Ui::DevSetDialog *ui;
};

#endif // DevSetDialog_H
