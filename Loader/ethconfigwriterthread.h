#ifndef ETHCONFIGWRITERTHREAD_H
#define ETHCONFIGWRITERTHREAD_H

#include <QObject>
#include <QString>
#include <QByteArray>

class EthConfigWriterThread : public QObject
{
    Q_OBJECT
    QString ip;
    QByteArray createSetConfigRequest(std::vector<quint16> conf);
    QByteArray createResetRequest();
public:
    explicit EthConfigWriterThread(const QString &ip, QObject *parent = nullptr);
    void startWrite(std::vector<quint16> conf);
signals:
    void success();
    void info(const QString & value);
};

#endif // ETHCONFIGWRITERTHREAD_H
