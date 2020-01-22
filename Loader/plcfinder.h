#ifndef PLCFINDER_H
#define PLCFINDER_H

#include <QObject>
#include <QDialog>
#include <QProgressBar>
#include "finderthread.h"
#include "ethfinderthread.h"
#include "portconfig.h"
#include "loader.h"

class PLCFinder:public QObject
{
    Q_OBJECT
    QDialog *dialog = nullptr;
    std::map<QString,QProgressBar*> bars;
    QProgressBar* oneBar=nullptr;
    portConfig portConf;
    std::vector<FinderThread*> finders;
    std::vector<QThread*> threads;
    bool foundFlag = false;
    QString binFileName;
    QString plcName;

public:
    enum {MKU_BOOT=1,MKU=3,PC21_1_2019=4,BOOT2=5};
    PLCFinder(const QString &plcName="",const QString &pName="", const portConfig &portConf=portConfig(), const QString &fName="", QObject *parent = nullptr);
    ~PLCFinder();
    void percentUpdate(const QString &pName, double value);
    void found(const PLCRS485State &plc);
    void ethPercentUpdate(double value);
    void ethFound(const PLCState &plc);
    void failed(const QString &pName);
    void checkFoundFlag();
signals:
    void stopScan();
    void startSearch();
};

#endif // PLCFINDER_H
