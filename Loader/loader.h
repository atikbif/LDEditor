#ifndef LOADER_H
#define LOADER_H

#include <QObject>
#include "finderthread.h"
#include "loaderthread.h"
#include <QDialog>
#include <QProgressBar>
#include <QLabel>

class Loader : public QObject
{
    Q_OBJECT
    PLCRS485State plc;
    QDialog *dialog;
    LoaderThread *worker = nullptr;
    QThread *thread = nullptr;
    QProgressBar *bar = nullptr;
    QString fName;
    QLabel *label = nullptr;
    void percentUpdate(double value);
    void loadFailed(const QString &message);
    void info(const QString &message);
    void successLoad();
public:
    explicit Loader(const PLCRS485State &plc, const QString &fName, QObject *parent = nullptr);
    ~Loader();
signals:
    void stopLoad();
    void startLoad();
public slots:
};

#endif // LOADER_H
