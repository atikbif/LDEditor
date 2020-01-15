#ifndef FINDERTHREAD_H
#define FINDERTHREAD_H

#include <QObject>
#include <QMutex>
#include "portconfig.h"
#include <set>

struct PLCRS485State {
    QString pName;
    int net_address = 1;
    unsigned long baudrate = 115200;
    enum {EVEN,ODD,NO} parity = NO;
    bool two_stop_bits = false;
    bool boot_mode = false;
    int code;
};



class FinderThread : public QObject
{
    Q_OBJECT
    QMutex mutex;
    QString pName;
    portConfig portConf;
    bool stopFlag = false;
    static std::vector<qint32> baudTable;
    static std::set<int> bootloaderCode;
public:
    explicit FinderThread(const QString &pName, const portConfig &portConf, QObject *parent = nullptr);
    bool startSearch();
    void stopSearch();
signals: 
    void percentUpdate(QString pName, double value);
    void found(const PLCRS485State &plc);
    void failed(QString pName);
};

#endif // FINDERTHREAD_H
