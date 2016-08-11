#include "IndictorLight.h"

IndictorLight::IndictorLight(QWidget *parent) :
    QWidget(parent)
{
    m_shoudColor = Qt::color0;
    m_color = Qt::color0;
    m_outerRadius = width()>height() ? height()/2 : width()/2;
    m_innerRadius = 0.8*m_outerRadius;
    m_colorPieRadius = m_outerRadius*0.8;
    m_center = rect().center();

    flashTimer = new QTimer(this);
    flashTimer->setInterval(500);
    connect(flashTimer,SIGNAL(timeout()),this,SLOT(UpdateGraph()));

    flashTimer->start();
}

void IndictorLight::resetVariables(QPainter *painter)
{
    m_outerRadius = width()>height() ? height()/2 : width()/2;
    m_innerRadius = 0.8*m_outerRadius;
    m_colorPieRadius = m_outerRadius*0.8;
    m_center = rect().center();
}

void IndictorLight::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    resetVariables(&painter);
    drawOuterCircle(&painter);
    drawInnerCircle(&painter);
    drawColorPies(&painter);

    painter.end();
}

void IndictorLight::drawOuterCircle(QPainter* painter)
{
    painter->save();

    QRadialGradient outerGradient(m_center,m_outerRadius,m_center);
    outerGradient.setColorAt(0.0,QColor(130,130,130));
    outerGradient.setColorAt(0.9,QColor(130,130,130));
    outerGradient.setColorAt(0.95,QColor(200,200,200));
    outerGradient.setColorAt(1.0,QColor(130,130,130));

    painter->setPen(Qt::NoPen);
    painter->setBrush(outerGradient);
    painter->drawEllipse(m_center,m_outerRadius,m_outerRadius);

    painter->restore();
}

void IndictorLight::drawInnerCircle(QPainter* painter)
{
    painter->save();

    QRadialGradient innerGradient(m_center,m_innerRadius,m_center);
    innerGradient.setColorAt(0.0,QColor(180,180,180));
    innerGradient.setColorAt(1.0,QColor(130,130,130));

    painter->setPen(Qt::NoPen);
    painter->setBrush(innerGradient);
    painter->drawEllipse(m_center,m_innerRadius,m_innerRadius);

    painter->restore();
}

void IndictorLight::drawColorPies(QPainter* painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    QPointF pieRectTopLeftPot(m_center.x()-m_colorPieRadius,m_center.y()-m_colorPieRadius);
    QPointF pieRectBottomRightPot(m_center.x()+m_colorPieRadius,m_center.y()+m_colorPieRadius);

    m_pieRect = QRectF(pieRectTopLeftPot,pieRectBottomRightPot);

    //draw pie
    painter->setBrush(m_color);
    painter->drawPie(m_pieRect,0*16,360*16);


    painter->restore();
}

void IndictorLight::UpdateGraph(void)
{
    if(m_light)
    {
        m_light = false;
        m_color = (Qt::color0);
    }
    else
    {
        m_light = true;
        m_color = m_shoudColor;
    }
    update();
}


void IndictorLight::setColor(QColor color)
{
    m_shoudColor = color;
    UpdateGraph();
}
