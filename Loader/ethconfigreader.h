#ifndef ETHCONFIGREADER_H
#define ETHCONFIGREADER_H

#include <QObject>
#include <QString>
#include <vector>
#include "ethconfigreaderthread.h"
#include <QThread>

class EthConfigReader : public QObject
{
    Q_OBJECT
    QThread* thread;
public:
    explicit EthConfigReader(const QString &ip, QObject *parent = nullptr);
    ~EthConfigReader();
    void readResultDebug(std::vector<quint16> value);
    void infoDebug(const QString & value);
signals:
    void startRead();
    void readResult(std::vector<quint16> value);
    void info(const QString & value);
};

#endif // ETHCONFIGREADER_H
