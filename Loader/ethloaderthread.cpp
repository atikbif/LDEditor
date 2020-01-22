#include "ethloaderthread.h"
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QMutexLocker>
#include <vector>
#include "plcfinder.h"
#include <QUdpSocket>
#include "checksum.h"

quint16 EthLoaderThread::id = 0;

QByteArray EthLoaderThread::createGetIDRequest()
{
    QByteArray res;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0xA0));
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

QByteArray EthLoaderThread::createSetBootModeRequest()
{
    QByteArray res;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0xEC));
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

QByteArray EthLoaderThread::createErasePageRequest(int pageNum)
{
    QByteArray res;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0xE8));
    res.append(static_cast<char>(pageNum));
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

QByteArray EthLoaderThread::createWriteFlashRequest(quint32 addr, quint16 cnt, QByteArray::ConstIterator it)
{
    QByteArray res;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0xE9));

    res.append(static_cast<char>((addr>>24)&0xFF));
    res.append(static_cast<char>((addr>>16)&0xFF));
    res.append(static_cast<char>((addr>>8)&0xFF));
    res.append(static_cast<char>(addr&0xFF));

    res.append(static_cast<char>((cnt>>8)&0xFF));
    res.append(static_cast<char>(cnt&0xFF));

    for (quint16 i=0;i<cnt;i++) {
        res.append(*it);
        it++;
    }

    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

QByteArray EthLoaderThread::createJumpToAppRequest()
{
    QByteArray res;
    res.append(static_cast<char>(id>>8));
    res.append(static_cast<char>(id&0xFF));
    res.append(static_cast<char>(0xEA));
    int crc = CheckSum::getCRC16(res);
    res.append(static_cast<char>(crc & 0xFF));
    res.append(static_cast<char>(crc >> 8));
    id++;
    return res;
}

EthLoaderThread::EthLoaderThread(const QString &ip, const PLCState &plc, const QString &fName, QObject *parent) : QObject(parent),ip(ip),plc(plc),fName(fName)
{

}

void EthLoaderThread::stopLoad()
{
    QMutexLocker locker(&mutex);
    stopFlag = true;
}

void EthLoaderThread::startLoad()
{
    const int wr_size = 256;
    QThread::msleep(1000);
    unsigned char try_num = 0;
    bool exit = false;

    QUdpSocket udp;
    char receiveBuf[1024];
    udp.connectToHost(QHostAddress(ip),12146);
    udp.open(QIODevice::ReadWrite);
    udp.readAll();

    if(!plc.boot_mode) {
        emit info("Переход в режим загрузчика...");
        try_num=0;

        udp.readAll();
        auto request = createSetBootModeRequest();
        udp.write(request);
        if(udp.waitForReadyRead(400)) {
            int cnt = 0;
            while(udp.hasPendingDatagrams()) {
                cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
            }
        }

        QThread::msleep(100);
        emit info("Ожидание перезапуска...");
        try_num = 0;

        while(try_num<20) {
            mutex.lock();
            exit = stopFlag;
            mutex.unlock();
            if(exit) return;
            udp.readAll();
            auto request = createGetIDRequest();
            udp.write(request);
            if(udp.waitForReadyRead(400)) {
                int cnt = 0;
                while(udp.hasPendingDatagrams()) {
                    cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
                }
                if(cnt>0) {
                    if(CheckSum::getCRC16(receiveBuf,cnt)==0) {
                        break;
                    }
                    else try_num++;
                }
            }else try_num++;
            QThread::msleep(50);
        }

        if(try_num>=20) {
            emit loadFailed("Контроллер не отвечает");
            udp.close();
            return;
        }
        QThread::msleep(150);
    }
    QFile file(fName);
    if(file.open(QFile::ReadOnly)) {

        QByteArray data = file.readAll();
        while(data.count()%4) data.append(static_cast<char>(0xFF)); // размер должен быть кратным 4 байтам

        quint32 sectSize = 128*1024;
        int sectCnt = static_cast<unsigned int>(data.count())%sectSize?static_cast<unsigned int>(data.count())/sectSize+1:static_cast<unsigned int>(data.count())/sectSize;
        unsigned char startSectNum = 0;
        int endSectnum = 0;
        if(sectCnt>6) {
            sectCnt  = 6;
            data.resize(static_cast<int>(sectSize)*sectCnt);
        }
        if(sectCnt==0) sectCnt = 1;
        endSectnum = sectCnt-1;

        double percWriteStep = data.count()%wr_size?100.0/(data.count()/wr_size+1):100.0/(data.count()/wr_size);

        // стирание Flash


        double percEraseStep = 100.0/(endSectnum-startSectNum+1);
        double percent = 0;
        try_num = 0;
        emit info("стирание Flash памяти");
        for(auto sectNum=startSectNum;sectNum<=endSectnum;sectNum++){
            qDebug() << "SECTOR:" << sectNum;
            try_num=0;

            while(try_num<TRY_LIM) {
                mutex.lock();
                exit = stopFlag;
                mutex.unlock();
                if(exit) return;
                udp.readAll();
                auto request = createErasePageRequest(sectNum);
                udp.write(request);
                if(udp.waitForReadyRead(2500)) {
                    int cnt = 0;
                    while(udp.hasPendingDatagrams()) {
                        cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
                    }
                    if(cnt>0) {
                        if(CheckSum::getCRC16(receiveBuf,cnt)==0 && receiveBuf[3]==0x01) {
                            percent += percEraseStep;
                            emit percentUpdate(percent);
                            break;
                        }
                        else try_num++;
                    }
                }else try_num++;
                QThread::msleep(50);
            }
            if(try_num>=TRY_LIM) {
                emit loadFailed("Контроллер не отвечает");
                udp.close();
                return;
            }
        }
        emit info("загрузка программы");
        //waitDelay = 300;
        percent = 0;
        emit percentUpdate(percent);

        auto endIt = data.constEnd();
        auto curIt = data.constBegin();
        quint32 addr = 0;
        auto dist=std::distance(curIt,endIt);
        while(dist!=0) {
            qDebug() << dist;
            quint16 length = wr_size;
            if(dist<wr_size) length = dist;
            try_num=0;

            while(try_num<TRY_LIM) {
                mutex.lock();
                exit = stopFlag;
                mutex.unlock();
                if(exit) return;
                udp.readAll();
                auto request = createWriteFlashRequest(addr,length,curIt);
                udp.write(request);
                if(udp.waitForReadyRead(300)) {
                    int cnt = 0;
                    while(udp.hasPendingDatagrams()) {
                        cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
                    }
                    if(cnt>0) {
                        if(CheckSum::getCRC16(receiveBuf,cnt)==0 && receiveBuf[3]==0x01) {
                            percent += percWriteStep;
                            emit percentUpdate(percent);
                            break;
                        }
                        else try_num++;
                    }
                }else try_num++;
                QThread::msleep(50);
            }
            QThread::msleep(10);
            if(try_num>=TRY_LIM) {
                emit loadFailed("контроллер не отвечает");
                //port.close();
                return;
            }
            curIt+=length;
            addr+=length;
            dist=std::distance(curIt,endIt);
        }

        emit successLoad();

        // переход к программе

        mutex.lock();
        exit = stopFlag;
        mutex.unlock();
        if(exit) return;

        udp.readAll();
        auto request = createJumpToAppRequest();
        udp.write(request);
        if(udp.waitForReadyRead(100)) {
            int cnt = 0;
            while(udp.hasPendingDatagrams()) {
                cnt = static_cast<int>(udp.readDatagram(receiveBuf,sizeof(receiveBuf)));
            }
        }
        QThread::msleep(50);
    }else emit loadFailed("Ошибка открытия файла " + fName);
}

