#include "CellMinMaxInfo.h"
#include <QGridLayout>
#include <QGroupBox>

CellMinMaxInfo::CellMinMaxInfo(QWidget *parent) :
    QWidget(parent)
{
    groupBox = new QGroupBox(this);
    groupBox->setTitle(tr("统计信息"));

    gridLayout = new QGridLayout(groupBox);

    labelMaxCellVol = new QLabel("电芯最大电压:");
    labelMaxVPos = new QLabel("最大电压位置:");
    labelMinCellVol = new QLabel("电芯最小电压:");
    labelMinVPos = new QLabel("最小电压位置:");
    labelDltV = new QLabel("电压差:",this);

    labelMaxCellTemp = new QLabel("最大温度:");
    labelMaxTPos = new QLabel("最大温度位置:");
    labelMinCellTemp = new QLabel("最小温度:");
    labelMinTPos = new QLabel("最小温度位置:");
    labelDltT = new QLabel("温度差:");
    labelCycTimes = new QLabel("循环次数:");
    labelCycTimes->setAlignment(Qt::AlignCenter);

    lineEditMaxCellVol = new QLineEdit();
    lineEditMaxVPos = new QLineEdit();
    lineEditMinCellVol = new QLineEdit();
    lineEditMinVPos = new QLineEdit();
    lineEditDltV = new QLineEdit();
    lineEditMaxCellTemp = new QLineEdit();
    lineEditMaxTPos = new QLineEdit();
    lineEditMinCellTemp = new QLineEdit();
    lineEditMinTPos = new QLineEdit();
    lineEditDltT = new QLineEdit();
    lineEditCycTimes = new QLineEdit();

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
    gridLayout->addWidget(labelCycTimes,4,0,1,1);

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
    gridLayout->addWidget(lineEditCycTimes,4,1,1,2);

    lineEditMaxCellVol->setReadOnly(true);
    lineEditMaxVPos->setReadOnly(true);
    lineEditMinCellVol->setReadOnly(true);
    lineEditMinVPos->setReadOnly(true);
    lineEditDltV->setReadOnly(true);
    lineEditMaxCellTemp->setReadOnly(true);
    lineEditMaxTPos->setReadOnly(true);
    lineEditMinCellTemp->setReadOnly(true);
    lineEditMinTPos->setReadOnly(true);
    lineEditDltT->setReadOnly(true);
    lineEditCycTimes->setReadOnly(true);

    gridLayout->setSpacing(5);
    //groupBox->setLayout(gridLayout);
    groupBox->resize(620,150);
    resize(630,200);
}
