#include "ethconfigwriterthread.h"
#include "checksum.h"
#include <QUdpSocket>
#include <QThread>

QByteArray EthConfigWriterThread::createSetConfigRequest(std::vector<quint16> conf)
{
    QByteArray res;
    static quint16 id = 0;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0x10));
    res.append('\0');
    res.append('\0');
    res.append(conf.size()>>8);
    res.append(conf.size()&0xFF);
    for(int i=0;i<conf.size();i++) {
        res.append(conf.at(i)>>8);
        res.append(conf.at(i)&0xFF);
    }
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

QByteArray EthConfigWriterThread::createResetRequest()
{
    QByteArray res;
    static quint16 id = 0;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0xED));
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

EthConfigWriterThread::EthConfigWriterThread(const QString &ip, QObject *parent) : QObject(parent), ip(ip)
{

}

void EthConfigWriterThread::startWrite(std::vector<quint16> conf)
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
        auto request = createSetConfigRequest(conf);
        udp.write(request);
        if(udp.waitForReadyRead(400)) {
            int cnt = 0;
            while(udp.hasPendingDatagrams()) {
                cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
            }
            if(cnt>0) {
                if(CheckSum::getCRC16(receiveBuf,cnt)==0) {
                    if((cnt==2+1+1+2) && receiveBuf[3]==0x01) funcRes = true;
                    else emit info("Ошибка при записи конфигурации");
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
        QThread::msleep(50);
        auto request = createResetRequest();
        udp.write(request);
        emit success();
    }
}
