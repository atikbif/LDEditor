#ifndef DIALOGPLCCONFIG_H
#define DIALOGPLCCONFIG_H

#include <QDialog>
#include <QByteArray>
#include <QString>
#include "plcconfig.h"

namespace Ui {
class DialogPLCConfig;
}

class DialogPLCConfig : public QDialog
{
    Q_OBJECT
    QByteArray conf;
    const int confSize = 27;
    const int confSize2 = confSize + 4;
    PLCConfig config;

    std::vector<quint16> convertToPLCMap(const QByteArray & value);
    QByteArray convertFromPLCMap(std::vector<quint16> value);
    std::vector<quint16> convertCurrentToPLCMap();
    bool checkCurrentConfig();
public:
    explicit DialogPLCConfig(PLCConfig config, QWidget *parent = nullptr);
    ~DialogPLCConfig();
    void setConfig(const QByteArray &value);
    QByteArray getConfig() const;
    void setCurrentIP(const QString &value);
    void setPeriodms(int value);
    int getPeriodms() const;

private slots:

    void on_pushButtonRead_clicked();

    void on_pushButtonWrite_clicked();

    void on_pushButtonSave_clicked();

signals:
    void saveConf(const QByteArray &conf);

private:
    Ui::DialogPLCConfig *ui;
};

#endif // DIALOGPLCCONFIG_H
