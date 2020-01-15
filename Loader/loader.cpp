#include "loader.h"
#include <QThread>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <cmath>
#include <QDebug>

void Loader::percentUpdate(double value)
{
    if(bar) bar->setValue(lround(value));
}

void Loader::loadFailed(const QString &message)
{
    if(label) label->setText("<b style=\"color:red;\">" + message + "</b>");
}

void Loader::info(const QString &message)
{
    if(label) label->setText("<b style=\"color:green;\">" + message + "</b>");
}

void Loader::successLoad()
{
    if(dialog!=nullptr) {
        label->setText(QString("<b style=\"color:green;\">Программа успешно загружена") + "</b>");
        dialog->repaint();
        QTimer::singleShot(2000,this,SLOT(deleteLater()));
    }
}

Loader::Loader(const PLCRS485State &plc, const QString &fName, QObject *parent) : QObject(parent),plc(plc),fName(fName)
{
    QThread::msleep(500);
    dialog = new QDialog();
    dialog->setWindowTitle("Загрузка программы");
    connect(dialog,&QDialog::finished,this,&Loader::deleteLater);
    auto *layout = new QVBoxLayout(dialog);

    worker = new LoaderThread(plc,fName);
    thread = new QThread();
    worker->moveToThread(thread);

    connect(thread,&QThread::finished,worker,&FinderThread::deleteLater);
    connect(this,&Loader::startLoad,worker,&LoaderThread::startLoad);
    connect(worker,&LoaderThread::percentUpdate,this,&Loader::percentUpdate);
    connect(worker,&LoaderThread::successLoad,this,&Loader::successLoad);
    connect(worker,&LoaderThread::loadFailed,this,&Loader::loadFailed);
    connect(worker,&LoaderThread::info,this,&Loader::info);
    thread->start();
    bar = new QProgressBar();
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(plc.pName+":"));
    hLayout->addWidget(bar);
    layout->addLayout(hLayout);
    label = new QLabel();
    layout->addWidget(label);
    emit startLoad();
    dialog->adjustSize();
    dialog->setMinimumWidth(dialog->width()*1.3);
    dialog->show();
}

Loader::~Loader()
{
    if(worker) {worker->stopLoad();}
    if(thread) {
        thread->quit();
        thread->wait();
    }
    delete dialog;
}
