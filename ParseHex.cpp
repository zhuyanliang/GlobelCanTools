#include "ParseHex.h"
#include <QDebug>
#include <QMessageBox>


// crc16校验计算查找表
const unsigned short Crc16Table[256] =
{
   0x0000,0xc0c1,0xc181,0x0140,0xc301,0x03c0,0x0280,0xc241,
   0xc601,0x06c0,0x0780,0xc741,0x0500,0xc5c1,0xc481,0x0440,
   0xcc01,0x0cc0,0x0d80,0xcd41,0x0f00,0xcfc1,0xce81,0x0e40,
   0x0a00,0xcac1,0xcb81,0x0b40,0xc901,0x09c0,0x0880,0xc841,
   0xd801,0x18c0,0x1980,0xd941,0x1b00,0xdbc1,0xda81,0x1a40,
   0x1e00,0xdec1,0xdf81,0x1f40,0xdd01,0x1dc0,0x1c80,0xdc41,
   0x1400,0xd4c1,0xd581,0x1540,0xd701,0x17c0,0x1680,0xd641,
   0xd201,0x12c0,0x1380,0xd341,0x1100,0xd1c1,0xd081,0x1040,
   0xf001,0x30c0,0x3180,0xf141,0x3300,0xf3c1,0xf281,0x3240,
   0x3600,0xf6c1,0xf781,0x3740,0xf501,0x35c0,0x3480,0xf441,
   0x3c00,0xfcc1,0xfd81,0x3d40,0xff01,0x3fc0,0x3e80,0xfe41,
   0xfa01,0x3ac0,0x3b80,0xfb41,0x3900,0xf9c1,0xf881,0x3840,
   0x2800,0xe8c1,0xe981,0x2940,0xeb01,0x2bc0,0x2a80,0xea41,
   0xee01,0x2ec0,0x2f80,0xef41,0x2d00,0xedc1,0xec81,0x2c40,
   0xe401,0x24c0,0x2580,0xe541,0x2700,0xe7c1,0xe681,0x2640,
   0x2200,0xe2c1,0xe381,0x2340,0xe101,0x21c0,0x2080,0xe041,
   0xa001,0x60c0,0x6180,0xa141,0x6300,0xa3c1,0xa281,0x6240,
   0x6600,0xa6c1,0xa781,0x6740,0xa501,0x65c0,0x6480,0xa441,
   0x6c00,0xacc1,0xad81,0x6d40,0xaf01,0x6fc0,0x6e80,0xae41,
   0xaa01,0x6ac0,0x6b80,0xab41,0x6900,0xa9c1,0xa881,0x6840,
   0x7800,0xb8c1,0xb981,0x7940,0xbb01,0x7bc0,0x7a80,0xba41,
   0xbe01,0x7ec0,0x7f80,0xbf41,0x7d00,0xbdc1,0xbc81,0x7c40,
   0xb401,0x74c0,0x7580,0xb541,0x7700,0xb7c1,0xb681,0x7640,
   0x7200,0xb2c1,0xb381,0x7340,0xb101,0x71c0,0x7080,0xb041,
   0x5000,0x90c1,0x9181,0x5140,0x9301,0x53c0,0x5280,0x9241,
   0x9601,0x56c0,0x5780,0x9741,0x5500,0x95c1,0x9481,0x5440,
   0x9c01,0x5cc0,0x5d80,0x9d41,0x5f00,0x9fc1,0x9e81,0x5e40,
   0x5a00,0x9ac1,0x9b81,0x5b40,0x9901,0x59c0,0x5880,0x9841,
   0x8801,0x48c0,0x4980,0x8941,0x4b00,0x8bc1,0x8a81,0x4a40,
   0x4e00,0x8ec1,0x8f81,0x4f40,0x8d01,0x4dc0,0x4c80,0x8c41,
   0x4400,0x84c1,0x8581,0x4540,0x8701,0x47c0,0x4680,0x8641,
   0x8201,0x42c0,0x4380,0x8341,0x4100,0x81c1,0x8081,0x4040
};

//============================================================================
// Function    ：ComputeCrc16
// Description ：crc16计算
// Parameters  ：
// Returns     ：
//============================================================================
unsigned short ComputeCrc16(QByteArray buffer, unsigned int len)
{
   unsigned short tmp = 0;
   unsigned int i = 0;
   while (len--)
   {
       tmp = (tmp>>8) ^ Crc16Table[(buffer[i++] ^ tmp) & 0xff];
   }

   return tmp;
}

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
    m_hexFileName(fileName),m_userId(usrid), m_configBits(configBits),
    m_endOfHex(endofhex),m_byteNums(0),m_Crc(0)
{
    m_file = new QFile(m_hexFileName);
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
            // 计算CRC16
            m_Crc = ComputeCrc16(m_binData,m_byteNums);
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






