#ifndef MODBUSREQUESTLIST_H
#define MODBUSREQUESTLIST_H

#include <QString>
#include <QVector>
#include <QStringList>
#include <QByteArray>
#include <QSharedPointer>
#include "modbusvarsstorage.h"

class ModbusRequestList
{
    struct VarReq {
        int byteOffset;
        int bitOffset;
        QString varName;
    };

    enum ModbusCMD {READ_DI, READ_COILS, WRITE_COILS, READ_HOLD_REG, WRITE_HOLD_REG, READ_INP_REG};

    struct Request {
        int netAddress;
        ModbusCMD cmd;
        int memAddress;
        int cnt;
        QVector<VarReq> reqVars;
    };
    QVector<Request> reqs;

    static const unsigned char crc16H[256];
    static const unsigned char crc16L[256];
public:
    ModbusRequestList();
    static QStringList getVarNames();
    QStringList getResult(int canNum);
    QStringList getHeader();
private:
    void createRequests(int canNum);
    QByteArray getRequestBody(const Request &r);
    int getAnswerLength(const Request &r);
    bool isWriteReq(const Request &r);
    void addCRC16(QByteArray &reqData);
    void sortVarsByMemAddress(QVector<QSharedPointer<ModbusVar>> &vars);
    void addRequestToList(const Request &r, int canNum);
    QStringList getReqText(int canNum);
};

#endif // MODBUSREQUESTLIST_H
