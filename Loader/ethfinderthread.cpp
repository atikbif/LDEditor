#include "ethfinderthread.h"
#include <QThread>
#include <QMutexLocker>
#include <QUdpSocket>
#include "checksum.h"

QByteArray EthFinderThread::createGetIDRequest()
{
    QByteArray res;
    static quint16 id = 0;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0xA0));
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

bool EthFinderThread::checkCRCAnswer(char *data, int length)
{
    if(CheckSum::getCRC16(data,length)==0) {
        return true;
    }
    return false;
}

EthFinderThread::EthFinderThread(const portConfig &portConf, QObject *parent):QObject(parent),portConf(portConf)
{
    qRegisterMetaType<PLCState>("PLCState");
}

bool EthFinderThread::startSearch()
{
    bool funcRes = false;
    int try_num = 0;
    double percent = 0;
    bool exit = false;

    const int try_limit = 5;

    double percWriteStep = 100.0/try_limit;

    QUdpSocket udp;
    char receiveBuf[1024];
    udp.connectToHost(QHostAddress(portConf.ip),12146);
    udp.open(QIODevice::ReadWrite);
    udp.readAll();

    while(try_num<try_limit) {
        mutex.lock();
        exit = stopFlag;
        mutex.unlock();
        if(exit) return false;
        udp.readAll();
        auto request = createGetIDRequest();
        udp.write(request);
        percent+=percWriteStep;
        percentUpdate(percent);
        if(udp.waitForReadyRead(400)) {
            int cnt = 0;
            while(udp.hasPendingDatagrams()) {
                cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
            }
            if(cnt>0) {
                if(checkCRCAnswer(receiveBuf,cnt)) {
                    funcRes = true;
                    break;
                }
                else try_num++;
            }
        }else try_num++;
        QThread::msleep(50);
    }
    if(try_num==try_limit) {
        emit failed("Устройство не отвечает");
        udp.close();
    }
    if(funcRes) {
        PLCState plc;
        int code = (uint8_t)receiveBuf[3];
        code <<= 8;code |= (uint8_t)receiveBuf[4];
        code <<= 8;code |= (uint8_t)receiveBuf[5];
        code <<= 8;code |= (uint8_t)receiveBuf[6];
        plc.code = code;
        if((code>>16)==0x01F5) plc.boot_mode = true;else plc.boot_mode=false;
        emit found(plc);
    }
    return funcRes;
}

void EthFinderThread::stopSearch()
{
    QMutexLocker locker(&mutex);
    stopFlag = true;
}
