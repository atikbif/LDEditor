#ifndef ETHCONFIGREADERTHREAD_H
#define ETHCONFIGREADERTHREAD_H

#include <QObject>
#include <QString>
#include <QByteArray>

class EthconfigReaderThread : public QObject
{
    Q_OBJECT
    QString ip;
    QByteArray createGetConfigRequest();
public:
    explicit EthconfigReaderThread(const QString &ip, QObject *parent = nullptr);
    void startRead();
signals:
    void readResult(std::vector<quint16> value);
    void info(const QString & value);
};

#endif // ETHCONFIGREADERTHREAD_H
