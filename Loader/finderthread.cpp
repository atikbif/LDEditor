#include "finderthread.h"
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include "rtuprotocol.h"
#include "SourceBuilder/plcparams.h"

std::vector<qint32> FinderThread::baudTable = {};
std::set<int> FinderThread::bootloaderCode = {1,5};

FinderThread::FinderThread(const QString &pName, const portConfig &portConf, QObject *parent) : QObject(parent),pName(pName),portConf(portConf)
{
    qRegisterMetaType<PLCRS485State>("PLCRS485State");
}

bool FinderThread::startSearch() {
    bool funcRes = false;
    QSerialPort port(pName);
    port.setDataBits(QSerialPort::Data8);
    if(portConf.stopBits==2) port.setStopBits(QSerialPort::TwoStop);
    else port.setStopBits(QSerialPort::OneStop);
    if(portConf.parity.contains("even")) port.setParity(QSerialPort::EvenParity);
    else if(portConf.parity.contains("odd")) port.setParity(QSerialPort::OddParity);
    else port.setParity(QSerialPort::NoParity);
    bool convertToDigit = false;
    long baud = portConf.baudrate.toLong(&convertToDigit);
    if(baud) {
        port.setBaudRate(baud);
        baudTable.clear();
        baudTable.push_back(baud);
    }else {
        port.setBaudRate(QSerialPort::Baud115200);
        baudTable.clear();
        baudTable.push_back(QSerialPort::Baud115200);
        baudTable.push_back(QSerialPort::Baud9600);
        baudTable.push_back(QSerialPort::Baud19200);
        baudTable.push_back(QSerialPort::Baud38400);
        baudTable.push_back(QSerialPort::Baud57600);
        baudTable.push_back(QSerialPort::Baud4800);
        baudTable.push_back(QSerialPort::Baud2400);
        baudTable.push_back(QSerialPort::Baud1200);
        baudTable.push_back(600);
        //baudTable.push_back(300);
    }
    if(port.open(QSerialPort::ReadWrite)) {
        quint16 waitDelay = 100;
        const auto percOffset = static_cast<double>(baudTable.size());
        double percValue = 0;
        foreach (qint32 baud, baudTable) {
            switch(baud) {
                case 115200:waitDelay=300;break;
                case 57600:waitDelay=300;break;
                case 56000:waitDelay=300;break;
                case 38400:waitDelay=500;break;
                case 19200:waitDelay=500;break;
                case 9600:waitDelay=1000;break;
                case 4800:waitDelay=2000;break;
                case 2400:waitDelay=3000;break;
                case 1200:waitDelay=3000;break;
                case 600:waitDelay=3000;break;
                //case 300:waitDelay=1600;break;
                default:waitDelay=300;break;
            }
            mutex.lock();
            if(stopFlag) {mutex.unlock();break;}
            mutex.unlock();
            port.setBaudRate(baud);
            percValue+=100/percOffset;
            emit percentUpdate(pName,percValue);
            std::vector<quint8> res = RTUProtocol::getID(port,portConf.netAddress,waitDelay);
            if(res.size()>=4) {
                PLCRS485State plc;
                plc.baudrate = static_cast<unsigned long>(baud);
                // check id
                plc.code = res.at(0);
                plc.code <<= 8;
                plc.code |= res.at(1);
                plc.code <<= 8;
                plc.code |= res.at(2);
                plc.code <<= 8;
                plc.code |= res.at(3);

                if(plc.code==0x01 || plc.code==0x05) plc.boot_mode = true;
                else {plc.boot_mode = false;}
                plc.net_address = static_cast<unsigned char>(portConf.netAddress);
                plc.pName = pName;
                if(portConf.parity.contains("even")) plc.parity = PLCRS485State::EVEN;
                else if(portConf.parity.contains("odd")) plc.parity = PLCRS485State::ODD;
                else plc.parity = PLCRS485State::NO;
                plc.two_stop_bits = portConf.stopBits==2 ? true:false;
                emit found(plc);
                funcRes = true;
                break;
            }
            //QThread::msleep(200);
        }
        port.close();
    }else {
        emit failed(pName);
        emit percentUpdate(pName,100);
    }
    return funcRes;
}
void FinderThread::stopSearch() {
    QMutexLocker locker(&mutex);
    stopFlag = true;
}
