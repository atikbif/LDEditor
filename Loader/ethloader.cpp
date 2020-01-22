#include "ethloader.h"
#include <QVBoxLayout>
#include <QTimer>
#include <cmath>
#include <QDebug>

void EthLoader::percentUpdate(double value)
{
    if(bar) bar->setValue(lround(value));
}

void EthLoader::loadFailed(const QString &message)
{
    if(label) label->setText("<b style=\"color:red;\">" + message + "</b>");
}

void EthLoader::info(const QString &message)
{
    if(label) label->setText("<b style=\"color:green;\">" + message + "</b>");
}

void EthLoader::successLoad()
{
    if(dialog!=nullptr) {
        label->setText(QString("<b style=\"color:green;\">Программа успешно загружена") + "</b>");
        dialog->repaint();
        QTimer::singleShot(2000,this,SLOT(deleteLater()));
    }
}

EthLoader::EthLoader(const QString ip, const PLCState &plc, const QString &fName, QObject *parent) : QObject(parent),ip(ip),plc(plc),fName(fName)
{
    QThread::msleep(500);
    dialog = new QDialog();
    dialog->setWindowTitle("Загрузка программы");
    connect(dialog,&QDialog::finished,this,&EthLoader::deleteLater);
    auto *layout = new QVBoxLayout(dialog);

    worker = new EthLoaderThread(ip,plc,fName);
    thread = new QThread();
    worker->moveToThread(thread);

    connect(thread,&QThread::finished,worker,&EthLoaderThread::deleteLater);
    connect(this,&EthLoader::startLoad,worker,&EthLoaderThread::startLoad);
    connect(worker,&EthLoaderThread::percentUpdate,this,&EthLoader::percentUpdate);
    connect(worker,&EthLoaderThread::successLoad,this,&EthLoader::successLoad);
    connect(worker,&EthLoaderThread::loadFailed,this,&EthLoader::loadFailed);
    connect(worker,&EthLoaderThread::info,this,&EthLoader::info);
    thread->start();
    bar = new QProgressBar();
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel("IP: "+ip));
    hLayout->addWidget(bar);
    layout->addLayout(hLayout);
    label = new QLabel();
    layout->addWidget(label);
    emit startLoad();
    dialog->adjustSize();
    dialog->setMinimumWidth(dialog->width()*1.3);
    dialog->show();
}

EthLoader::~EthLoader()
{
    if(worker) {worker->stopLoad();}
    if(thread) {
        thread->quit();
        thread->wait();
    }
    delete dialog;
}

