#include "BatterySoc.h"

BatterySoc::BatterySoc(QWidget *parent) :
        QWidget(parent)
{
    init();
}

void BatterySoc::init(void)
{
    m_bReverse = false;
    m_value = 0;
    m_currentValue = 0;
    updateTimer = new QTimer(this);
    updateTimer->setInterval(5);
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(UpdateGraph()));
}

void BatterySoc::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawFrame(&painter);
    drawBattery(&painter);
    drawTextRect(&painter);
    painter.end();
}
void BatterySoc::drawFrame(QPainter *painter)
{
    painter->save();
    m_headHeight = height()/20;

    QPointF topLeftPot(0,m_headHeight);
    QPointF bottomRightPot(width(),height());
    QRectF batteryRect(topLeftPot,bottomRightPot);
    m_batteryRect=batteryRect;
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::black,2));
    painter->drawRect(batteryRect);

    QPointF headRectTopLeft(width()/4,0);
    QPointF headRectBottomRight(3*width()/4,m_headHeight);
    QRectF headRect(headRectTopLeft,headRectBottomRight);
    QLinearGradient headRectGradient(headRect.topLeft(),headRect.bottomLeft());
    headRectGradient.setColorAt(0.0,QColor(150,150,150));
    headRectGradient.setColorAt(0.5,QColor(220,220,220));
    headRectGradient.setColorAt(1.0,QColor(150,150,150));

    painter->setBrush(headRectGradient);
    painter->drawRect(headRect);

    painter->restore();
}

void BatterySoc::drawBattery(QPainter *painter)
{
    painter->save();

    QLinearGradient batteryGradient(QPointF(width(),height()),QPointF(0,m_batteryRect.height()));

    if(m_value<20)
    {
        batteryGradient.setColorAt(0.0,QColor(220,220,220));
        batteryGradient.setColorAt(0.7,QColor(180,50,50));
        batteryGradient.setColorAt(1.0,QColor(120,50,50));
    }
    else
    {
        batteryGradient.setColorAt(0.3,QColor(0,180,0));
    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(batteryGradient);
    QPointF bottomLeftPot(m_batteryRect.bottomLeft().x()+1,m_batteryRect.bottomLeft().y()-1);

    qreal unit = m_batteryRect.height()/100;
    qreal wid = m_value*1.0*unit;
    QPointF topRightPot(width()-1,height()-wid);
    painter->drawRect(QRectF(bottomLeftPot,topRightPot));

    painter->restore();
}

void BatterySoc::drawTextRect(QPainter* painter)
{
    painter->save();
    qreal rectWidth = width()*0.6;

    painter->setOpacity(0.7);
    QPointF topLeftPot(width()/2-rectWidth/2,height()/2-rectWidth/2);
    QPointF bottomRightPot(width()/2+rectWidth/2,height()/2+rectWidth/2);
    QRectF textRect(topLeftPot,bottomRightPot);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::color0);
    painter->drawRoundRect(textRect,rectWidth,rectWidth);

    painter->setOpacity(1.0);
    painter->setPen(Qt::black);
    qreal fontSize = rectWidth*0.4;
    QFont font;
    font.setPointSize(fontSize);
    painter->setFont(font);
    painter->drawText(textRect,Qt::AlignVCenter|Qt::AlignHCenter,tr("%1").arg((uchar)m_value).append("%"));
    painter->restore();
}

void BatterySoc::UpdateGraph()
{
    if(m_bReverse)
    {
        if((uint)m_value != (uint)m_currentValue)
            m_value -= 0.1;

        if(m_value <= m_currentValue)
        {
            updateTimer->stop();
        }
    }
    else
    {
        if((uint)m_value != (uint)m_currentValue)
            m_value += 0.1;
        if(m_value >= m_currentValue)
        {
            updateTimer->stop();
        }
    }
    update();
}

void BatterySoc::setValue(qreal value)
{
    if(value<0 || value>100)
    {
        qWarning() << "The value must be between 0 and 100 !";
        return ;
    }
    if(value > m_value)
    {
        m_bReverse = false;
    }
    else if(value < m_value)
    {
        m_bReverse = true;
    }

    updateTimer->start();
    m_currentValue = value;

    update();
}

