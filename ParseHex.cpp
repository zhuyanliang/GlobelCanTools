#include "ParseHex.h"
#include <QDebug>
#include <QMessageBox>


unsigned char ChartoByte(char c)
{
    if(c-'a'>=0 )
        return(c-'a'+10);
    else if(c-'A'>=0 )
        return(c-'A'+10);
    else
        return(c-'0');
}

unsigned char ChartoByte(QChar c)
{
    if(c.toAscii()-'a'>=0 )
        return(c.toAscii()-'a'+10);
    else if(c.toAscii()-'A'>=0 )
        return(c.toAscii()-'A'+10);
    else
        return(c.toAscii()-'0');
}

unsigned char TwoCharToByte(char *s)
{
    return ((ChartoByte(*s)<<4)+ChartoByte(*(s+1)));
}

unsigned char TwoCharToByte(QString s)
{
    if(s.count() >=2)
        return ((ChartoByte(s.at(0))<<4)
            +ChartoByte(s.at(1)));
    else
        return 0xFF;
}

ParseHex::ParseHex(QString fileName,QString usrid,QString configBits,
                   QString endofhex, QObject *parent) :
    QObject(parent),
    m_hexFileName(fileName)
{
    m_file = new QFile(m_hexFileName);
    m_userId = usrid;
    m_configBits = configBits;
    m_endOfHex = endofhex;
    m_byteNums = 0;
    m_binData.resize(1024);
}

ParseHex::~ParseHex()
{
    if(m_file->isOpen())
        m_file->close();
    m_binData.clear();
}

bool ParseHex::StartParse(void)
{
    QString line;
    m_binData.clear();

    if(!m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QTextStream in(m_file);

    while (!in.atEnd())
    {
        line = in.readLine();
        if(line.at(0) != ':')
        {
            qDebug() << "Hex文件损坏";
            return false;
        }

        if(line == m_userId)
        {
            return true;
        }
        else
        {
            // 往m_binData中填充bin类型数据
            if(!LinePutToArray(line))
            {
                QMessageBox::warning(NULL,tr("Hex文件解析失败"),
                          tr("Hex文件:%1 被损坏").arg(m_hexFileName));
                return false;
            }
        }
    }

    return true;
}

bool ParseHex::LinePutToArray(QString &line)
{
    unsigned short  thisLineAddr = 0x00;
    unsigned char   crc = 0;
    unsigned int    crcsum = 0;
    static unsigned short   lastLineAddr = 0x00;
    static unsigned char    lastLen = 0;

    unsigned char thisLen = TwoCharToByte(line.mid(1,2));

    thisLineAddr = GetAddr(line);
    if((lastLineAddr + lastLen) < thisLineAddr) //如果地址不连续
    {
        if(m_byteNums != 0)
        {
            int fillLen = (thisLineAddr - lastLineAddr - lastLen);
            for(int i=0;i<fillLen;i++)
            {
                m_binData.append(0xFF);
                m_byteNums++;
            }
        }
    }

    for(int i=0;i<thisLen;i++)
    {
        unsigned char byte = TwoCharToByte(line.mid(9+i*2,2));
        m_binData.append(byte);
        m_byteNums++;
    }

    //计算读取到的一行的CRC
    for(int i=0;i<thisLen+4;i++)
    {
        crcsum += TwoCharToByte(line.mid(i*2+1,2));
    }

    //读取的CRC
    crc = TwoCharToByte(line.mid(9+thisLen*2,2));

    if(crc != (unsigned char)(0x100 - crcsum%256))
    {
        qDebug() << "CRC Error!";
        return false;
    }

    lastLineAddr = GetAddr(line);
    lastLen = TwoCharToByte(line.mid(1,2));

    return true;
}

unsigned short ParseHex::GetAddr(QString &line)
{
    unsigned short ret = 0;
    ret += (TwoCharToByte(line.mid(3,2))<<8);
    ret += TwoCharToByte(line.mid(5,2));
    return ret;
}






