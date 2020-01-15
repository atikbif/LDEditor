#ifndef PLCFINDER_H
#define PLCFINDER_H

#include <QObject>
#include <QDialog>
#include <QProgressBar>
#include "finderthread.h"
#include "portconfig.h"
#include "loader.h"

class PLCFinder:public QObject
{
    Q_OBJECT
    QDialog *dialog = nullptr;
    std::map<QString,QProgressBar*> bars;
    portConfig portConf;
    std::vector<FinderThread*> finders;
    std::vector<QThread*> threads;
    bool foundFlag = false;
    QString binFileName;

public:
    enum {MKU_BOOT=1,MKU=3,PC21_1_2019=4,BOOT2=5};
    PLCFinder(const QString &pName="", const portConfig &portConf=portConfig(), const QString &fName="", QObject *parent = nullptr);
    ~PLCFinder();
    void percentUpdate(const QString &pName, double value);
    void found(const PLCRS485State &plc);
    void failed(const QString &pName);
    void checkFoundFlag();
signals:
    void stopScan();
    void startSearch();
};

#endif // PLCFINDER_H
