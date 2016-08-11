#ifndef BATTERYSOC_H
#define BATTERYSOC_H

#include <QWidget>
#include <QtGui>

class BatterySoc : public QWidget
{
    Q_OBJECT
public:
    explicit BatterySoc(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const
    {
        return QSize(200,120);
    }

private:
    void init(void);
    void drawFrame(QPainter* painter);
    void drawBattery(QPainter* painter);
    void drawTextRect(QPainter* painter);

private:
    QTimer* updateTimer;
    bool    m_bReverse;
    qreal   m_headHeight;
    qreal   m_value;
    qreal   m_currentValue;
    QRectF  m_batteryRect;

private slots:
    void UpdateGraph();

public slots:
    void setValue(qreal value);

};

#endif // BATTERYSOC_H
