#include "ethconfigwriter.h"
#include "ethconfigwriterthread.h"

EthConfigWriter::EthConfigWriter(const QString &ip, std::vector<quint16> conf, QObject *parent) : QObject(parent)
{
    qRegisterMetaType<std::vector<quint16>>("std::vector<quint16>");

    auto *worker = new EthConfigWriterThread(ip);
    thread = new QThread();
    worker->moveToThread(thread);
    connect(thread,&QThread::finished,worker,&EthConfigWriterThread::deleteLater);
    connect(this,&EthConfigWriter::startWrite,worker,&EthConfigWriterThread::startWrite);
    connect(worker,&EthConfigWriterThread::success,this,&EthConfigWriter::success);
    connect(worker,&EthConfigWriterThread::info,this,&EthConfigWriter::info);
    thread->start();

    emit startWrite(conf);
}

EthConfigWriter::~EthConfigWriter()
{
    thread->quit();
    thread->wait();
}
