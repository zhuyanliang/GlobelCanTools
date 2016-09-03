#ifndef PARSEHEX_H
#define PARSEHEX_H

#include <QObject>
#include <QString>
#include <QFile>

class ParseHex : public QObject
{
    Q_OBJECT
public:
    explicit ParseHex(QObject *parent = 0);
    explicit ParseHex(QString fileName,QString usrid,
                      QString configBits, QString endofhex,
                      QObject *parent = 0);
    ~ParseHex();

    unsigned short GetAddr(QString &line);
    bool LinePutToArray(QString &line);    //将一行数据转换后放入m_binData
    bool StartParse(void);
    unsigned long GetByteNums(){return m_byteNums;} //必须放在StartParse之后调用
    unsigned char GetBinData(unsigned int i){return m_binData[i];}
    unsigned short GetCrc(){return m_Crc;}

private:
    QFile           *m_file; //Hex文件
    QByteArray      m_binData;
    const QString   m_hexFileName;
    QString         m_userId;
    QString         m_configBits;
    QString         m_endOfHex;
    unsigned long   m_byteNums;   //hex文件转换后的字节数
    unsigned short  m_Crc;
    
signals:
    
public slots:
    
};

#endif // PARSEHEX_H
