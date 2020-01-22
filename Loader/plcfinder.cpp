#include "plcfinder.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include "SourceBuilder/plcparams.h"
#include <cmath>
#include <QDebug>
#include "ethfinderthread.h"
#include <QProgressBar>
#include "plcutils.h"
#include "ethloader.h"

PLCFinder::PLCFinder(const QString &plcName,const QString &pName, const portConfig &portConf, const QString &fName, QObject *parent):QObject(parent),portConf(portConf)
{
    this->plcName = plcName;
    if(portConf.isEthDefault) {
        binFileName = fName;
        emit stopScan();
        QThread::msleep(200);
        dialog = new QDialog();
        dialog->setWindowTitle("Поиск контроллера");

        connect(dialog,&QDialog::finished,this,&PLCFinder::deleteLater);
        auto *layout = new QVBoxLayout(dialog);
        auto *bar = new QProgressBar();
        oneBar = bar;
        auto *hLayout = new QHBoxLayout();
        hLayout->addWidget(new QLabel("IP: " + portConf.ip));
        hLayout->addWidget(bar);
        layout->addLayout(hLayout);

        auto *worker = new EthFinderThread(portConf);
        auto *thread = new QThread();
        worker->moveToThread(thread);

        connect(thread,&QThread::finished,worker,&EthFinderThread::deleteLater);
        connect(this,&PLCFinder::startSearch,worker,&EthFinderThread::startSearch);
        connect(worker,&EthFinderThread::percentUpdate,this,&PLCFinder::ethPercentUpdate);
        connect(worker,&EthFinderThread::found,this,&PLCFinder::ethFound);
        connect(worker,&EthFinderThread::failed,this,&PLCFinder::failed);
        thread->start();

        emit startSearch();
        dialog->adjustSize();
        dialog->setFixedWidth(dialog->width()*1.6);
        dialog->show();
    }else {
        std::vector<QString> pNames;
        binFileName = fName;
        if(pName.contains("COM")) {
            pNames.push_back(pName);
        }else {
            foreach(QSerialPortInfo pInfo,QSerialPortInfo::availablePorts()) {
                pNames.push_back(pInfo.portName());
            }
        }
        
        if(!pNames.empty()) {
            std::sort(pNames.begin(),pNames.end());
            emit stopScan();
            QThread::msleep(200);
            dialog = new QDialog();
            dialog->setWindowTitle("Поиск контроллера");
    
            connect(dialog,&QDialog::finished,this,&PLCFinder::deleteLater);
            auto *layout = new QVBoxLayout(dialog);
    
            for(const QString &pName: pNames) {
                auto *worker = new FinderThread(pName,portConf);
                finders.push_back(worker);
                auto *thread = new QThread();
                threads.push_back(thread);
                worker->moveToThread(thread);
    
                connect(thread,&QThread::finished,worker,&FinderThread::deleteLater);
                connect(this,&PLCFinder::startSearch,worker,&FinderThread::startSearch);
                connect(worker,&FinderThread::percentUpdate,this,&PLCFinder::percentUpdate);
                connect(worker,&FinderThread::found,this,&PLCFinder::found);
                connect(worker,&FinderThread::failed,this,&PLCFinder::failed);
                thread->start();
                auto *bar = new QProgressBar();
                auto *hLayout = new QHBoxLayout();
                hLayout->addWidget(new QLabel(pName+":"));
                hLayout->addWidget(bar);
                layout->addLayout(hLayout);
                bars[pName]=bar;
            }
            emit startSearch();
            dialog->adjustSize();
            dialog->setFixedWidth(dialog->width()*1.6);
            dialog->show();
        }else {
            QMessageBox::information(nullptr,tr("Сообщение"),tr("Не обнаружен ни один COM порт для поиска"));
        }
    }
}

PLCFinder::~PLCFinder()
{
    foreach (FinderThread* worker, finders) {
       worker->stopSearch();
    }
    foreach (QThread* thread, threads) {
        thread->quit();
        thread->wait();
    }
    delete dialog;
}

void PLCFinder::percentUpdate(const QString &pName, double value)
{
    if(bars.find(pName)!=bars.end()) {
        QProgressBar *bar = bars[pName];
        bar->setValue(lround(value));
    }
    bool prContinue = false;
    for(auto bar:bars) {
        if(bar.second->value()!=bar.second->maximum()) prContinue = true;
    }
    if(!prContinue) {
        QTimer::singleShot(500,this,&PLCFinder::checkFoundFlag);
    }
}

void PLCFinder::found(const PLCRS485State &plc)
{
    foundFlag = true;
    if(dialog!=nullptr) {
        QLabel *message = new QLabel(QString("<b style=\"color:green;\">Обнаружен контроллер ") + " порт: " + plc.pName + "</b>");
        dialog->layout()->addWidget(message);
        dialog->repaint();
        QTimer::singleShot(50,this,SLOT(deleteLater()));
        //QString fName = QCoreApplication::applicationDirPath() + "/CORE/prog.bin";
        if(plc.code==PLCParams::code || plc.code==1 || plc.code==5) {
            auto *loader = new Loader(plc,binFileName);
            Q_UNUSED(loader)
        }
        else QMessageBox::warning(nullptr,tr("Сообщение"),tr("Тип контроллера в проекте не соответствует типу подключенного контроллера"));
        //Q_UNUSED(loader)
    }
}

void PLCFinder::ethPercentUpdate(double value)
{
    if(oneBar) {
        oneBar->setValue(lround(value));
        bool prContinue = false;
        if(oneBar->value()!=oneBar->maximum()) prContinue = true;
        if(!prContinue) {
            QTimer::singleShot(500,this,&PLCFinder::checkFoundFlag);
        }
    }
}

void PLCFinder::ethFound(const PLCState &plc)
{
    Q_UNUSED(plc)
    foundFlag = true;
    if(dialog!=nullptr) {
        QLabel *message = new QLabel(QString("<b style=\"color:green;\">Обнаружен контроллер ")  + "</b>");
        dialog->layout()->addWidget(message);
        dialog->repaint();
        QTimer::singleShot(50,this,SLOT(deleteLater()));
        if(PLCUtils::checkPLCCode(plcName,plc.code)) {
            auto *loader = new EthLoader(portConf.ip,plc,binFileName);
            Q_UNUSED(loader)
        }else QMessageBox::warning(nullptr,tr("Сообщение"),tr("Тип контроллера в проекте не соответствует типу подключенного контроллера"));
    }
}

void PLCFinder::failed(const QString &pName)
{
    if(bars.find(pName)!=bars.end()) {
        QString danger = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FFB0B0,stop: 0.5 #FF5050,stop: 1 #FF0000 );border: .px solid black;}";
        QProgressBar *bar = bars[pName];
        bar->setStyleSheet(danger);
        bar->update();
    }
}

void PLCFinder::checkFoundFlag()
{
    if((!foundFlag)&&(dialog!=nullptr)) {
        QLabel *message = new QLabel("<b style=\"color:red;\">Контроллер не обнаружен!</b>");
        dialog->layout()->addWidget(message);
        QTimer::singleShot(2000,this,SLOT(deleteLater()));
    }
}
