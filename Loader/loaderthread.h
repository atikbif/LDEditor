#ifndef LOADERTHREAD_H
#define LOADERTHREAD_H

#include <QObject>
#include "finderthread.h"
#include <QMutex>

class LoaderThread : public QObject
{
    Q_OBJECT
    PLCRS485State plc;
    QString fName;
    static const int TRY_LIM = 3;
    QMutex mutex;
    bool stopFlag = false;
public:
    explicit LoaderThread(const PLCRS485State &plc, const QString &fName, QObject *parent = nullptr);
    void stopLoad();
    void startLoad();
signals:
    void percentUpdate(double value);
    void loadFailed(const QString &message);
    void successLoad();
    void info(const QString &message);
public slots:
};

#endif // LOADERTHREAD_H
