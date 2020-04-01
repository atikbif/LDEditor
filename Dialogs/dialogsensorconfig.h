#ifndef DIALOGSENSORCONFIG_H
#define DIALOGSENSORCONFIG_H

#include <QDialog>
#include "ainsensor.h"

namespace Ui {
class DialogSensorConfig;
}

class DialogSensorConfig : public QDialog
{
    Q_OBJECT
    bool editMode = false;
    AINSensor sensor;
public:
    explicit DialogSensorConfig(QWidget *parent = nullptr);
    ~DialogSensorConfig();
    void setSensor(const AINSensor &sensor);
    void setEditMode(bool value);
    AINSensor getSensor();

private:
    Ui::DialogSensorConfig *ui;

    // QDialog interface
public slots:
    void accept();
};

#endif // DIALOGSENSORCONFIG_H
