#include "ethconfigreader.h"
#include <QDebug>


EthConfigReader::EthConfigReader(const QString &ip, QObject *parent) : QObject(parent)
{
    qRegisterMetaType<std::vector<quint16>>("std::vector<quint16>");

    auto *worker = new EthconfigReaderThread(ip);
    thread = new QThread();
    worker->moveToThread(thread);
    connect(thread,&QThread::finished,worker,&EthconfigReaderThread::deleteLater);
    connect(this,&EthConfigReader::startRead,worker,&EthconfigReaderThread::startRead);
    connect(worker,&EthconfigReaderThread::readResult,this,&EthConfigReader::readResultDebug);
    connect(worker,&EthconfigReaderThread::info,this,&EthConfigReader::infoDebug);
    thread->start();

    emit startRead();
}

EthConfigReader::~EthConfigReader()
{
    thread->quit();
    thread->wait();
}

void EthConfigReader::readResultDebug(std::vector<quint16> value)
{
    qDebug() << "READ OK" << value;
    emit readResult(value);
}

void EthConfigReader::infoDebug(const QString &value)
{
    emit info(value);
}
