#ifndef DIALOGCONFIGSCADAMAPVARS_H
#define DIALOGCONFIGSCADAMAPVARS_H

#include <QDialog>
#include "plcconfig.h"

namespace Ui {
class DialogConfigScadaMapVars;
}

class DialogConfigScadaMapVars : public QDialog
{
    Q_OBJECT

    const PLCConfig &plcConfig;

    std::vector<std::pair<QString,QString>> states;
    void createDITable();
    void createDOTable();
    void createAITable();
    void createClustBitsTable();
    void createClustRegsTable();
    void createNetBitsTable();
    void createNetRegsTable();
    void createTelemetryBitsTable();
    void createTelemetryRegsTable();

public:
    explicit DialogConfigScadaMapVars(const PLCConfig &plcConfig, QWidget *parent = nullptr);
    ~DialogConfigScadaMapVars();

private:
    Ui::DialogConfigScadaMapVars *ui;
};

#endif // DIALOGCONFIGSCADAMAPVARS_H
