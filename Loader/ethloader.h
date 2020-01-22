#ifndef ETHLOADER_H
#define ETHLOADER_H

#include <QObject>
#include "ethfinderthread.h"
#include <QDialog>
#include <QProgressBar>
#include <QString>
#include <QLabel>
#include <QThread>
#include "ethloaderthread.h"

class EthLoader : public QObject
{
    Q_OBJECT
    QString ip;
    PLCState plc;
    QDialog *dialog;
    EthLoaderThread *worker = nullptr;
    QThread *thread = nullptr;
    QProgressBar *bar = nullptr;
    QString fName;
    QLabel *label = nullptr;
    void percentUpdate(double value);
    void loadFailed(const QString &message);
    void info(const QString &message);
    void successLoad();
public:
    explicit EthLoader(const QString ip, const PLCState &plc, const QString &fName, QObject *parent = nullptr);
    ~EthLoader();
signals:
    void stopLoad();
    void startLoad();
public slots:
};

#endif // ETHLOADER_H
