#ifndef ETHCONFIGWRITER_H
#define ETHCONFIGWRITER_H

#include <QObject>
#include <QString>
#include <vector>
#include "ethconfigreaderthread.h"
#include <QThread>

class EthConfigWriter : public QObject
{
    Q_OBJECT
    QThread* thread;
public:
    explicit EthConfigWriter(const QString &ip, std::vector<quint16> conf, QObject *parent = nullptr);
    ~EthConfigWriter();

signals:
    void startWrite(std::vector<quint16> value);
    void success();
    void info(const QString & value);
};

#endif // ETHCONFIGWRITER_H
