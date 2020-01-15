#include "rtuprotocol.h"

const unsigned char RTUProtocol::crc16H[256] = {

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40} ;

const unsigned char RTUProtocol::crc16L[256] = {

    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40} ;

unsigned short RTUProtocol::req_id=0;

void RTUProtocol::addCRC(QByteArray &buf)
{
    unsigned char i;
    unsigned short num = 0;
    unsigned char CRCH=0xFF,CRCL=0xFF;
    int cnt = buf.count();
    while(cnt--)
    {
        i=CRCH ^ buf.at(num++);
        CRCH = CRCL ^ crc16H[i];
        CRCL = crc16L[i];
    }
    buf.append(CRCH);
    buf.append(CRCL);
}

std::vector<quint8> RTUProtocol::getID(QSerialPort &port, int &netAddr, int waitTime)
{
    std::vector<quint8> result;
    if(port.isOpen()) {
        QByteArray tx;
        QByteArray rx;
        tx.append(static_cast<char>(netAddr));
        tx.append(static_cast<char>(0xA0));
        tx.append(static_cast<char>(req_id>>8));
        tx.append(static_cast<char>(req_id&0xFF));
        addCRC(tx);
        port.write(tx);
        port.waitForBytesWritten(waitTime);

        if(port.waitForReadyRead(waitTime)) {
            rx = port.readAll();
            while (port.waitForReadyRead(30)) rx += port.readAll();
        }

        if(rx.count()>=10) {
            addCRC(rx);
            if((rx.at(rx.length()-1)==0)&&(rx.at(rx.length()-2)==0)) {
                if(static_cast<quint8>(rx.at(1))==0xA0 && static_cast<quint8>(rx.at(2))==(req_id>>8) && static_cast<quint8>(rx.at(3))==(req_id&0xFF)) {
                    netAddr = static_cast<int>(rx.at(0));
                    rx = rx.mid(4,rx.length()-6);
                    for(const auto &v: rx) {
                        result.push_back(static_cast<quint8>(v));
                    }
                }
            }
        }
    }
    req_id++;
    return result;
}

std::vector<quint8> RTUProtocol::erasePage(QSerialPort &port, int &netAddr, int pageNum, int waitTime)
{
    std::vector<quint8> result;
    if(port.isOpen()) {
        QByteArray tx;
        QByteArray rx;
        tx.append(static_cast<char>(netAddr));
        tx.append(static_cast<char>(0xE8));
        tx.append(static_cast<char>(req_id>>8));
        tx.append(static_cast<char>(req_id&0xFF));
        tx.append(static_cast<char>(pageNum&0xFF));
        addCRC(tx);
        port.write(tx);
        port.waitForBytesWritten(waitTime);

        if(port.waitForReadyRead(waitTime)) {
            rx = port.readAll();
            while (port.waitForReadyRead(30)) rx += port.readAll();
        }

        if(rx.count()>=7) {
            addCRC(rx);
            if((rx.at(rx.length()-1)==0)&&(rx.at(rx.length()-2)==0)) {
                if(static_cast<quint8>(rx.at(1))==0xE8 && static_cast<quint8>(rx.at(2))==(req_id>>8) && static_cast<quint8>(rx.at(3))==(req_id&0xFF)) {
                    netAddr = static_cast<int>(rx.at(0));
                    rx = rx.mid(4,rx.length()-6);
                    for(const auto &v: rx) {
                        result.push_back(static_cast<quint8>(v));
                    }
                }
            }
        }
    }
    req_id++;
    return result;
}

std::vector<quint8> RTUProtocol::writeFlash(QSerialPort &port, int &netAddr, quint32 addr, quint16 cnt, QByteArray::ConstIterator it, int waitTime)
{
    std::vector<quint8> result;
    if(port.isOpen()) {
        QByteArray tx;
        QByteArray rx;
        tx.append(static_cast<char>(netAddr));
        tx.append(static_cast<char>(0xE9));
        tx.append(static_cast<char>(req_id>>8));
        tx.append(static_cast<char>(req_id&0xFF));
        tx.append(static_cast<char>((addr>>24)&0xFF));
        tx.append(static_cast<char>((addr>>16)&0xFF));
        tx.append(static_cast<char>((addr>>8)&0xFF));
        tx.append(static_cast<char>(addr&0xFF));
        tx.append(static_cast<char>((cnt>>8)&0xFF));
        tx.append(static_cast<char>(cnt&0xFF));
        for (quint16 i=0;i<cnt;i++) {
            tx.append(*it);
            it++;
        }
        addCRC(tx);
        port.write(tx);
        port.waitForBytesWritten(waitTime);

        if(port.waitForReadyRead(waitTime)) {
            rx = port.readAll();
            while (port.waitForReadyRead(30)) rx += port.readAll();
        }

        if(rx.count()>=7) {
            addCRC(rx);
            if((rx.at(rx.length()-1)==0)&&(rx.at(rx.length()-2)==0)) {
                if(static_cast<quint8>(rx.at(1))==0xE9 && static_cast<quint8>(rx.at(2))==(req_id>>8) && static_cast<quint8>(rx.at(3))==(req_id&0xFF)) {
                    netAddr = static_cast<int>(rx.at(0));
                    rx = rx.mid(4,rx.length()-6);
                    for(const auto &v: rx) {
                        result.push_back(static_cast<quint8>(v));
                    }
                }
            }
        }
    }
    req_id++;
    return result;
}

std::vector<quint8> RTUProtocol::jumpToProg(QSerialPort &port, int &netAddr, int waitTime)
{
    std::vector<quint8> result;
    if(port.isOpen()) {
        QByteArray tx;
        QByteArray rx;
        tx.append(static_cast<char>(netAddr));
        tx.append(static_cast<char>(0xEA));
        tx.append(static_cast<char>(req_id>>8));
        tx.append(static_cast<char>(req_id&0xFF));
        addCRC(tx);
        port.write(tx);
        port.waitForBytesWritten(waitTime);

        if(port.waitForReadyRead(waitTime)) {
            rx = port.readAll();
            while (port.waitForReadyRead(30)) rx += port.readAll();
        }

        if(rx.count()>=7) {
            addCRC(rx);
            if((rx.at(rx.length()-1)==0)&&(rx.at(rx.length()-2)==0)) {
                if(static_cast<quint8>(rx.at(1))==0xEA && static_cast<quint8>(rx.at(2))==(req_id>>8) && static_cast<quint8>(rx.at(3))==(req_id&0xFF)) {
                    netAddr = static_cast<int>(rx.at(0));
                    rx = rx.mid(4,rx.length()-6);
                    for(const auto &v: rx) {
                        result.push_back(static_cast<quint8>(v));
                    }
                }
            }
        }
    }
    req_id++;
    return result;
}

std::vector<quint8> RTUProtocol::resetPLC(QSerialPort &port, int &netAddr, int waitTime)
{
    std::vector<quint8> result;
    if(port.isOpen()) {
        QByteArray tx;
        tx.append(static_cast<char>(netAddr));
        tx.append(static_cast<char>(0xEF));
        tx.append(static_cast<char>(req_id>>8));
        tx.append(static_cast<char>(req_id&0xFF));
        addCRC(tx);
        port.write(tx);
        port.waitForBytesWritten(waitTime);
    }
    req_id++;
    return result;
}

bool RTUProtocol::setBootMode(QSerialPort &port, int &netAddr, int waitTime)
{
    if(port.isOpen()) {
        QByteArray tx;
        QByteArray rx;
        tx.append(static_cast<char>(netAddr));
        tx.append(static_cast<char>(0xEC));
        tx.append(static_cast<char>(req_id>>8));
        tx.append(static_cast<char>(req_id&0xFF));
        addCRC(tx);
        port.write(tx);
        port.waitForBytesWritten(waitTime);

        if(port.waitForReadyRead(waitTime)) {
            rx = port.readAll();
            while (port.waitForReadyRead(30)) rx += port.readAll();
        }

        if(rx.count()>=6) {
            addCRC(rx);
            if((rx.at(rx.length()-1)==0)&&(rx.at(rx.length()-2)==0)) {
                if(static_cast<quint8>(rx.at(1))==0xEC && static_cast<quint8>(rx.at(2))==(req_id>>8) && static_cast<quint8>(rx.at(3))==(req_id&0xFF)) {
                    return true;
                }
            }
        }
    }
    req_id++;
    return false;
}

std::optional<uint16_t> RTUProtocol::readEraseSectorQuantity(QSerialPort &port, int &netAddr, uint16_t fl_size, int waitTime)
{
    if(port.isOpen()) {
        QByteArray tx;
        QByteArray rx;
        tx.append(static_cast<char>(netAddr));
        tx.append(static_cast<char>(0xEB));
        tx.append(static_cast<char>(req_id>>8));
        tx.append(static_cast<char>(req_id&0xFF));
        tx.append(static_cast<char>(fl_size>>8));
        tx.append(static_cast<char>(fl_size&0xFF));
        addCRC(tx);
        port.write(tx);
        port.waitForBytesWritten(waitTime);

        if(port.waitForReadyRead(waitTime)) {
            rx = port.readAll();
            while (port.waitForReadyRead(30)) rx += port.readAll();
        }

        if(rx.count()>=8) {
            addCRC(rx);
            if((rx.at(rx.length()-1)==0)&&(rx.at(rx.length()-2)==0)) {
                if(static_cast<quint8>(rx.at(1))==0xEB && static_cast<quint8>(rx.at(2))==(req_id>>8) && static_cast<quint8>(rx.at(3))==(req_id&0xFF)) {
                    uint16_t res = (uint8_t)rx.at(4);
                    res = res << 8;
                    res |= (uint8_t)rx.at(5);
                    return res;
                }
            }
        }
    }
    req_id++;
    return {};
}
