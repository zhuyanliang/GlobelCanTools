#include "CellMinMaxInfo.h"
#include <QGridLayout>

CellMinMaxInfo::CellMinMaxInfo(QWidget *parent) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);

    labelMaxCellVol = new QLabel("电芯最大电压:",this);
    labelMaxVPos = new QLabel("电芯最大电压位置:",this);
    labelMinCellVol = new QLabel("电芯最小电压:",this);
    labelMinVPos = new QLabel("电芯最小电压位置:",this);
    labelDltV = new QLabel("电压差:",this);

    labelMaxCellTemp = new QLabel("最大温度:",this);
    labelMaxTPos = new QLabel("最大温度位置:",this);
    labelMinCellTemp = new QLabel("最小温度:",this);
    labelMinTPos = new QLabel("最小温度位置:",this);
    labelDltT = new QLabel("温度差:",this);

    lineEditMaxCellVol = new QLineEdit(this);
    lineEditMaxVPos = new QLineEdit(this);
    lineEditMinCellVol = new QLineEdit(this);
    lineEditMinVPos = new QLineEdit(this);
    lineEditDltV = new QLineEdit(this);
    lineEditMaxCellTemp = new QLineEdit(this);
    lineEditMaxTPos = new QLineEdit(this);
    lineEditMinCellTemp = new QLineEdit(this);
    lineEditMinTPos = new QLineEdit(this);
    lineEditDltT = new QLineEdit(this);

    gridLayout->addWidget(labelMaxCellVol,0,0,1,1);
    gridLayout->addWidget(labelMaxVPos,0,1,1,1);
    gridLayout->addWidget(labelMinCellVol,0,2,1,1);
    gridLayout->addWidget(labelMinVPos,0,3,1,1);
    gridLayout->addWidget(labelDltV,0,4,1,1);
    gridLayout->addWidget(labelMaxCellTemp,2,0,1,1);
    gridLayout->addWidget(labelMaxTPos,2,1,1,1);
    gridLayout->addWidget(labelMinCellTemp,2,2,1,1);
    gridLayout->addWidget(labelMinTPos,2,3,1,1);
    gridLayout->addWidget(labelDltT,2,4,1,1);

    gridLayout->addWidget(lineEditMaxCellVol,1,0,1,1);
    gridLayout->addWidget(lineEditMaxVPos,1,1,1,1);
    gridLayout->addWidget(lineEditMinCellVol,1,2,1,1);
    gridLayout->addWidget(lineEditMinVPos,1,3,1,1);
    gridLayout->addWidget(lineEditDltV,1,4,1,1);
    gridLayout->addWidget(lineEditMaxCellTemp,3,0,1,1);
    gridLayout->addWidget(lineEditMaxTPos,3,1,1,1);
    gridLayout->addWidget(lineEditMinCellTemp,3,2,1,1);
    gridLayout->addWidget(lineEditMinTPos,3,3,1,1);
    gridLayout->addWidget(lineEditDltT,3,4,1,1);

    gridLayout->setSpacing(5);
    setLayout(gridLayout);
    resize(500,100);
}
