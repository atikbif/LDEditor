#include "loaderthread.h"
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QSerialPort>
#include "rtuprotocol.h"
#include "plcfinder.h"
#include <QMutexLocker>
#include <vector>

LoaderThread::LoaderThread(const PLCRS485State &plc, const QString &fName, QObject *parent) : QObject(parent),plc(plc),fName(fName)
{

}

void LoaderThread::stopLoad()
{
    QMutexLocker locker(&mutex);
    stopFlag = true;
}

void LoaderThread::startLoad()
{
    const int wr_size = 256;
    int waitDelay = 100;
    switch(plc.baudrate) {
        case 115200:waitDelay=1000;break;
        case 57600:waitDelay=1000;break;
        case 56000:waitDelay=1000;break;
        case 38400:waitDelay=1000;break;
        case 19200:waitDelay=1000;break;
        case 9600:waitDelay=2000;break;
        case 4800:waitDelay=3000;break;
        case 2400:waitDelay=3000;break;
        case 1200:waitDelay=4000;break;
        case 600:waitDelay=8000;break;
        default:waitDelay=300;break;
    }

    QThread::msleep(1000);
    unsigned char try_num = 0;
    bool exit = false;

    QSerialPort port(plc.pName);
    port.setDataBits(QSerialPort::Data8);
    if(plc.two_stop_bits) port.setStopBits(QSerialPort::TwoStop);
    else port.setStopBits(QSerialPort::OneStop);
    if(plc.parity==PLCRS485State::EVEN) port.setParity(QSerialPort::EvenParity);
    else if(plc.parity==PLCRS485State::ODD) port.setParity(QSerialPort::OddParity);
    else port.setParity(QSerialPort::NoParity);
    port.setBaudRate(static_cast<qint32>(plc.baudrate));
    if(port.open(QSerialPort::ReadWrite)) {

        if(!plc.boot_mode) {
            emit info("Переход в режим загрузчика...");
            try_num=0;
            if(plc.code==PLCFinder::PC21_1_2019) {
                while(try_num<20) {
                    mutex.lock();
                    exit = stopFlag;
                    mutex.unlock();
                    if(exit) return;
                    port.readAll();
                    bool request = RTUProtocol::setBootMode(port,plc.net_address,500);
                    if(request) break;
                    try_num++;
                }
                QThread::msleep(100);
                emit info("Ожидание перезапуска...");
                try_num = 0;
                while(try_num<20) {
                    mutex.lock();
                    exit = stopFlag;
                    mutex.unlock();
                    if(exit) return;
                    port.readAll();
                    std::vector<quint8> request = RTUProtocol::getID(port,plc.net_address,500);
                    if(!request.empty()) break;
                    try_num++;
                    QThread::msleep(10);
                }
                if(try_num>=20) {
                    emit loadFailed("Контроллер не отвечает");
                    port.close();
                    return;
                }
                QThread::msleep(150);
            }else {
                while(try_num<TRY_LIM) {
                    mutex.lock();
                    exit = stopFlag;
                    mutex.unlock();
                    if(exit) return;
                    port.readAll();
                    auto request = RTUProtocol::erasePage(port,plc.net_address,7,2000);
                    if(!request.empty()) {
                        if(request.at(0)==0x01) {
                            break;
                        }
                        try_num++;
                    }else try_num++;
                    QThread::msleep(10);
                }
                if(try_num>=TRY_LIM) {
                    emit loadFailed("Контроллер не отвечает");
                    port.close();
                    return;
                }
                RTUProtocol::resetPLC(port,plc.net_address,1000);
                QThread::msleep(150);
            }
        }
        QFile file(fName);
        if(file.open(QFile::ReadOnly)) {

            QByteArray data = file.readAll();
            while(data.count()%4) data.append(static_cast<char>(0xFF)); // размер должен быть кратным 4 байтам

            quint32 sectSize = 4*1024;
            if(plc.code==4 || plc.code==5) sectSize = 128*1024;
            int sectCnt = static_cast<unsigned int>(data.count())%sectSize?static_cast<unsigned int>(data.count())/sectSize+1:static_cast<unsigned int>(data.count())/sectSize;
            unsigned char startSectNum = 0;
            int endSectnum = 0;
            if(plc.code==4 || plc.code==5) {
                if(sectCnt>6) {
                    sectCnt  = 6;
                    data.resize(static_cast<int>(sectSize)*sectCnt);
                }
                if(sectCnt==0) sectCnt = 1;
                endSectnum = sectCnt-1;
            }else  {
                if(sectCnt>24) {
                    sectCnt  = 24;
                    data.resize(static_cast<int>(sectSize)*sectCnt);
                }
                sectCnt++;
                startSectNum=8;
                endSectnum = startSectNum + sectCnt - 1;
            }
            double percWriteStep = data.count()%wr_size?100.0/(data.count()/wr_size+1):100.0/(data.count()/wr_size);

            // стирание Flash


            double percEraseStep = 100.0/(endSectnum-startSectNum+1);
            double percent = 0;
            try_num = 0;
            emit info("стирание Flash памяти");
            for(auto sectNum=startSectNum;sectNum<=endSectnum;sectNum++){
                try_num=0;
                while(try_num<TRY_LIM) {
                    mutex.lock();
                    exit = stopFlag;
                    mutex.unlock();
                    if(exit) return;
                    port.readAll();
                    auto request = RTUProtocol::erasePage(port,plc.net_address,sectNum,2500);
                    if(!request.empty()) {
                        if(request.at(0)==0x01) {
                            percent += percEraseStep;
                            emit percentUpdate(percent);
                            break;
                        }
                        try_num++;
                    }else try_num++;
                    QThread::msleep(10);
                }
                if(try_num>=TRY_LIM) {
                    emit loadFailed("Контроллер не отвечает");
                    port.close();
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
                quint16 length = wr_size;
                if(dist<wr_size) length = dist;
                try_num=0;
                while(try_num<TRY_LIM) {
                    mutex.lock();
                    exit = stopFlag;
                    mutex.unlock();
                    if(exit) return;
                    port.readAll();
                    auto request = RTUProtocol::writeFlash(port,plc.net_address,addr,length,curIt,waitDelay);
                    if(!request.empty()) {
                        if(request.at(0)==0x01) {
                            percent += percWriteStep;
                            emit percentUpdate(percent);
                            break;
                        }
                        try_num++;
                    }else try_num++;
                    QThread::msleep(10);
                }
                if(try_num>=TRY_LIM) {
                    emit loadFailed("контроллер не отвечает");
                    port.close();
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
            port.readAll();
            auto request = RTUProtocol::jumpToProg(port,plc.net_address,500);
            QThread::msleep(10);

        }else emit loadFailed("Ошибка открытия файла " + fName);
        port.close();
    }
}
