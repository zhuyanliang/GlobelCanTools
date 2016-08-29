#ifndef XMLCONFIG_H
#define XMLCONFIG_H

#include <QObject>
#include <QXmlStreamReader>
#include <QMap>

class XmlConfig : public QObject
{
    Q_OBJECT
public:
    explicit XmlConfig(QObject *parent = 0);
    bool readFile(const QString &fileName);
    QMap<QString,short> &getValue(){return value;}
    bool readXmlok;
signals:
    
public slots:
private:
    QXmlStreamReader reader;
    QMap<QString,short> value;
};

#endif // XMLCONFIG_H
