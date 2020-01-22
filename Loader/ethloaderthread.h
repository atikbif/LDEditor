#ifndef ETHLOADERTHREAD_H
#define ETHLOADERTHREAD_H

#include <QObject>
#include "ethfinderthread.h"
#include <QMutex>
#include <QByteArray>

class EthLoaderThread : public QObject
{
    Q_OBJECT
    QString ip;
    PLCState plc;
    QString fName;
    static const int TRY_LIM = 3;
    QMutex mutex;
    bool stopFlag = false;
    static quint16 id;
    QByteArray createGetIDRequest();
    QByteArray createSetBootModeRequest();
    QByteArray createErasePageRequest(int pageNum);
    QByteArray createWriteFlashRequest(quint32 addr, quint16 cnt, QByteArray::ConstIterator it);
    QByteArray createJumpToAppRequest();
public:
    explicit EthLoaderThread(const QString &ip, const PLCState &plc, const QString &fName, QObject *parent = nullptr);
    void stopLoad();
    void startLoad();
signals:
    void percentUpdate(double value);
    void loadFailed(const QString &message);
    void successLoad();
    void info(const QString &message);
public slots:
};

#endif // ETHLOADERTHREAD_H
