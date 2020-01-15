#ifndef RTUPROTOCOL_H
#define RTUPROTOCOL_H

#include <QByteArray>
#include <QSerialPort>
#include <optional>

class RTUProtocol
{
    static const unsigned char crc16H[256];
    static const unsigned char crc16L[256];

    static void addCRC(QByteArray &buf);
    static unsigned short req_id;
public:
    RTUProtocol() = default;
    static std::vector<quint8> getID(QSerialPort &port, int &netAddr, int waitTime);
    static std::vector<quint8> erasePage(QSerialPort &port, int &netAddr, int pageNum, int waitTime);
    static std::vector<quint8> writeFlash(QSerialPort &port, int &netAddr,quint32 addr, quint16 cnt, QByteArray::ConstIterator it, int waitTime);
    static std::vector<quint8> jumpToProg(QSerialPort &port, int &netAddr, int waitTime);
    static std::vector<quint8> resetPLC(QSerialPort &port, int &netAddr, int waitTime);

    static bool setBootMode(QSerialPort &port, int &netAddr, int waitTime);
    static std::optional<uint16_t> readEraseSectorQuantity(QSerialPort &port, int &netAddr, uint16_t fl_size, int waitTime);
};

#endif // RTUPROTOCOL_H
