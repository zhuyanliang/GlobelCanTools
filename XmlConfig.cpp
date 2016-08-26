#include "XmlConfig.h"
#include <QFile>
#include <QMessageBox>

#define ATTRIBUTES(X)     reader.attributes().value(X).toString()

XmlConfig::XmlConfig(QObject *parent) :
    QObject(parent)
{
}

bool XmlConfig::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(NULL, tr("Error"),
                             tr("Cannot read file %1").arg(fileName));
        return false;
    }

    //将file设置为QXmlStreamReader的设备
    reader.setDevice(&file);
    while (!reader.atEnd())
    {
        reader.readNextStartElement();
        if(reader.isEndElement())
            continue;
        if(reader.name() == "cov")
        {
            value.insert("cov1",ATTRIBUTES("cov1").toInt());
            value.insert("cov2",ATTRIBUTES("cov2").toInt());
        }
        else if(reader.name() == "cuv")
        {
            value.insert("cuv1",ATTRIBUTES("cuv1").toInt());
            value.insert("cuv2",ATTRIBUTES("cuv2").toInt());
        }
        else if(reader.name() == "pcot")
        {
            value.insert("pcot1",ATTRIBUTES("pcot1").toInt());
            value.insert("pcot2",ATTRIBUTES("pcot2").toInt());
        }
        else if(reader.name() == "pcut")
        {
            value.insert("pcut1",ATTRIBUTES("pcut1").toInt());
            value.insert("pcut2",ATTRIBUTES("pcut2").toInt());
        }
        else if(reader.name() == "pdot")
        {
            value.insert("pdot1",ATTRIBUTES("pdot1").toInt());
            value.insert("pdot2",ATTRIBUTES("pdot2").toInt());
        }
        else if(reader.name() == "pdut")
        {
            value.insert("pdut1",ATTRIBUTES("pdut1").toInt());
            value.insert("pdut2",ATTRIBUTES("pdut2").toInt());
        }
        else if(reader.name() == "pcoc")
        {
            value.insert("pcoc1",ATTRIBUTES("pcoc1").toInt());
            value.insert("pcoc2",ATTRIBUTES("pcoc2").toInt());
        }
        else if(reader.name() == "pdoc")
        {
            value.insert("pdoc1",ATTRIBUTES("pdoc1").toInt());
            value.insert("pdoc2",ATTRIBUTES("pdoc2").toInt());
        }
        else if(reader.name() == "pdit")
        {
            value.insert("pdit1",ATTRIBUTES("pdit1").toInt());
            value.insert("pdit2",ATTRIBUTES("pdit2").toInt());
        }
        else if(reader.name() == "pov")
        {
            value.insert("pov1",ATTRIBUTES("pov1").toInt());
            value.insert("pov2",ATTRIBUTES("pov2").toInt());
        }
        else if(reader.name() == "puv")
        {
            value.insert("puv1",ATTRIBUTES("puv1").toInt());
            value.insert("puv2",ATTRIBUTES("puv2").toInt());
        }
        else if(reader.name() == "dim")
        {
            value.insert("dim1",ATTRIBUTES("dim1").toInt());
            value.insert("dim2",ATTRIBUTES("dim2").toInt());
        }
        else
        {
            //reader.raiseError(tr("Not a valid book file"));
        }

    }
    file.close();

    return true;
}

