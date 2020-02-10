#include "ethconfigreaderthread.h"
#include "checksum.h"
#include <QUdpSocket>
#include <QThread>

QByteArray EthconfigReaderThread::createGetConfigRequest()
{
    QByteArray res;
    static quint16 id = 0;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0x03));
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(0x08);
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

EthconfigReaderThread::EthconfigReaderThread(const QString &ip, QObject *parent) : QObject(parent),ip(ip)
{

}

void EthconfigReaderThread::startRead()
{
    bool funcRes = false;
    int try_num = 0;
    const int try_limit = 5;

    QUdpSocket udp;
    char receiveBuf[1024];
    udp.connectToHost(QHostAddress(ip),12146);
    udp.open(QIODevice::ReadWrite);
    udp.readAll();

    while(try_num<try_limit) {
        udp.readAll();
        auto request = createGetConfigRequest();
        udp.write(request);
        if(udp.waitForReadyRead(400)) {
            int cnt = 0;
            while(udp.hasPendingDatagrams()) {
                cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
            }
            if(cnt>0) {
                if(CheckSum::getCRC16(receiveBuf,cnt)==0) {
                    if((cnt==2+1+1+16+2) && receiveBuf[3]==0x01) funcRes = true;
                    else emit info("Ошибка при чтении конфигурации");
                    break;
                }
                else try_num++;
            }
        }else try_num++;
        QThread::msleep(50);
    }
    if(try_num==try_limit) {
        emit info("Устройство не отвечает");
        udp.close();
    }
    if(funcRes) {
        std::vector<quint16> config;
        for(int i=0;i<8;i++) {
            quint16 reg = static_cast<quint8>(receiveBuf[4+i*2]);
            reg = static_cast<quint16>(reg << 8);
            reg |= static_cast<quint8>(receiveBuf[5+i*2]);
            config.push_back(reg);
        }
        emit readResult(config);
    }
}
