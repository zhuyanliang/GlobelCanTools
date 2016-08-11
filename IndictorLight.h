#ifndef INDICTORLIGHT_H
#define INDICTORLIGHT_H

#include <QWidget>
#include <QtGui>

class IndictorLight : public QWidget
{
    Q_OBJECT
private:
    bool m_light;   //…¡À∏ π”√
    QColor m_color;
    QColor m_shoudColor;
    qreal m_outerRadius;
    qreal m_innerRadius;
    qreal m_colorPieRadius;
    qreal m_coverCircleRadius;
    QPointF m_center;
    QRectF m_pieRect;
    QTimer* flashTimer;

public:
    explicit IndictorLight(QWidget *parent = 0);
    void setColor(QColor color);

protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const
    {
        return QSize(30,30);
    }

    QSize minimumSizeHint() const
    {
        return QSize(10,10);
    }

private:
    void resetVariables(QPainter*);
    void drawOuterCircle(QPainter*);
    void drawInnerCircle(QPainter*);
    void drawColorPies(QPainter*);
    void drawCoverCircle(QPainter*);

signals:
    
private slots:
    void UpdateGraph();
    
};

#endif // INDICTORLIGHT_H
