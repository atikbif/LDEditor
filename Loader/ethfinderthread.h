#ifndef ETHFINDERTHREAD_H
#define ETHFINDERTHREAD_H

#include <QObject>
#include <QMutex>
#include "portconfig.h"
#include <set>
#include <QByteArray>

struct PLCState {
    bool boot_mode = false;
    int code;
};

class EthFinderThread : public QObject
{
    Q_OBJECT
    QMutex mutex;
    portConfig portConf;
    bool stopFlag = false;
    static std::set<int> bootloaderCode;
    QByteArray createGetIDRequest();
    bool checkCRCAnswer(char *data, int length);
public:
    explicit EthFinderThread(const portConfig &portConf, QObject *parent = nullptr);
    bool startSearch();
    void stopSearch();

signals:
    void percentUpdate(double value);
    void found(const PLCState &plc);
    void failed(QString pName);

};

#endif // ETHFINDERTHREAD_H
