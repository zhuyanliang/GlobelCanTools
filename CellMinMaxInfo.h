#ifndef CELLMINMAXINFO_H
#define CELLMINMAXINFO_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class QGroupBox;
class QGridLayout;

class CellMinMaxInfo : public QWidget
{
    Q_OBJECT

private:
    QGroupBox *groupBox;
    QLabel* labelMaxCellVol;
    QLabel* labelMaxVPos;
    QLabel* labelMinCellVol;
    QLabel* labelMinVPos;
    QLabel* labelDltV;
    QLabel* labelMaxCellTemp;
    QLabel* labelMaxTPos;
    QLabel* labelMinCellTemp;
    QLabel* labelMinTPos;
    QLabel* labelDltT;
    QLabel* labelCycTimes;
    QGridLayout* gridLayout;

public:
    explicit CellMinMaxInfo(QWidget *parent = 0);

    QLineEdit* lineEditMaxCellVol;
    QLineEdit* lineEditMaxVPos;
    QLineEdit* lineEditMinCellVol;
    QLineEdit* lineEditMinVPos;
    QLineEdit* lineEditDltV;
    QLineEdit* lineEditMaxCellTemp;
    QLineEdit* lineEditMaxTPos;
    QLineEdit* lineEditMinCellTemp;
    QLineEdit* lineEditMinTPos;
    QLineEdit* lineEditDltT;
    QLineEdit* lineEditCycTimes;
signals:
    
public slots:
    
};

#endif // CELLMINMAXINFO_H
