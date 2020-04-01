#ifndef DIALOGADCCONFIG_H
#define DIALOGADCCONFIG_H

#include <QDialog>
#include "plcconfig.h"

namespace Ui {
class DialogADCconfig;
}

class DialogADCconfig : public QDialog
{
    Q_OBJECT
    PLCConfig config;

public:
    explicit DialogADCconfig(PLCConfig config, QWidget *parent = nullptr);
    ~DialogADCconfig();
    std::vector<int> getSensorTypes() const;
    std::vector<AINSensor> getAINSensors() const {return config.getADCSensors();}

private slots:
    void on_pushButtonAddSensor_clicked();

    void on_pushButtonRemoveSensor_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonDown_clicked();

private:
    void editSensor();
    void updateTDU();
    void updateSensNames();
    void updateSensNames(const QString &prevName, const QString &updName);
    Ui::DialogADCconfig *ui;

    // QDialog interface
public slots:
    void accept();
};

#endif // DIALOGADCCONFIG_H
