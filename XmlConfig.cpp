#include "XmlConfig.h"
#include <QFile>
#include <QMessageBox>

#define ATTRIBUTES(X)     reader.attributes().value(X).toString()

XmlConfig::XmlConfig(QObject *parent) :
    QObject(parent)
{
    readXmlok = false;
    value.clear();
}

bool XmlConfig::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(NULL, tr("错误"),
                             tr("无法打开配置文件%1").arg(fileName));
        return false;
    }

    //将file设置为QXmlStreamReader的设备
    reader.setDevice(&file);
    while (!reader.atEnd())
    {
        reader.readNextStartElement();
        if(reader.isEndElement())
            continue;
        if(reader.name() == "systemconfig")
        {
            continue;
        }
        else if(reader.name() == "cov")
        {
            value.insert("cov1",ATTRIBUTES("cov1").toShort());
            value.insert("cov2",ATTRIBUTES("cov2").toShort());
        }
        else if(reader.name() == "cuv")
        {
            value.insert("cuv1",ATTRIBUTES("cuv1").toShort());
            value.insert("cuv2",ATTRIBUTES("cuv2").toShort());
        }
        else if(reader.name() == "pcot")
        {
            value.insert("pcot1",ATTRIBUTES("pcot1").toShort());
            value.insert("pcot2",ATTRIBUTES("pcot2").toShort());
        }
        else if(reader.name() == "pcut")
        {
            value.insert("pcut1",ATTRIBUTES("pcut1").toShort());
            value.insert("pcut2",ATTRIBUTES("pcut2").toShort());
        }
        else if(reader.name() == "pdot")
        {
            value.insert("pdot1",ATTRIBUTES("pdot1").toShort());
            value.insert("pdot2",ATTRIBUTES("pdot2").toShort());
        }
        else if(reader.name() == "pdut")
        {
            value.insert("pdut1",ATTRIBUTES("pdut1").toShort());
            value.insert("pdut2",ATTRIBUTES("pdut2").toShort());
        }
        else if(reader.name() == "pcoc")
        {
            value.insert("pcoc1",ATTRIBUTES("pcoc1").toShort());
            value.insert("pcoc2",ATTRIBUTES("pcoc2").toShort());
        }
        else if(reader.name() == "pdoc")
        {
            value.insert("pdoc1",ATTRIBUTES("pdoc1").toShort());
            value.insert("pdoc2",ATTRIBUTES("pdoc2").toShort());
        }
        else if(reader.name() == "pdlt")
        {
            value.insert("pdlt1",ATTRIBUTES("pdlt1").toShort());
            value.insert("pdlt2",ATTRIBUTES("pdlt2").toShort());
        }
        else if(reader.name() == "pov")
        {
            value.insert("pov1",ATTRIBUTES("pov1").toShort());
            value.insert("pov2",ATTRIBUTES("pov2").toShort());
        }
        else if(reader.name() == "puv")
        {
            value.insert("puv1",ATTRIBUTES("puv1").toShort());
            value.insert("puv2",ATTRIBUTES("puv2").toShort());
        }
        else if(reader.name() == "dlv")
        {
            value.insert("dlv1",ATTRIBUTES("dlv1").toShort());
            value.insert("dlv2",ATTRIBUTES("dlv2").toShort());
        }
        else
        {
            reader.raiseError(tr("Not a valid config file"));
        }

    }
    file.close();
    readXmlok = true;

    return true;
}

