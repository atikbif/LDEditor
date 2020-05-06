#ifndef DIALOGCONFIGSCADAMAPVARS_H
#define DIALOGCONFIGSCADAMAPVARS_H

#include <QDialog>
#include "plcconfig.h"
#include <map>
#include <QDataStream>
#include <QByteArray>

namespace Ui {
class DialogConfigScadaMapVars;
}

class DialogConfigScadaMapVars : public QDialog
{
    Q_OBJECT

public:
    struct VarDescription {
        QString varName;
        std::map<QString,QString> properties;
    };

private:
    QStringList falseTrueStates;
    const PLCConfig &plcConfig;
    const std::vector<QString> &prVars;
    std::vector<std::pair<QString,QString>> states;
    void createDITable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createDOTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createAITable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createClustBitsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createClustRegsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createNetBitsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createNetRegsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createTelemetryBitsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);
    void createTelemetryRegsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars);

    void markDITable(const std::vector<QString> &names);
    void markDOTable(const std::vector<QString> &names);
    void markAITable(const std::vector<QString> &names);
    void markClustBitsTable(const std::vector<QString> &names);
    void markClustRegsTable(const std::vector<QString> &names);
    void markNetBitsTable(const std::vector<QString> &names);
    void markNetRegsTable(const std::vector<QString> &names);
    void markTelemetryBitsTable(const std::vector<QString> &names);
    void markTelemetryRegsTable(const std::vector<QString> &names);

    void clearDITable();
    void clearDOTable();
    void clearAITable();
    void clearClustBitsTable();
    void clearClustRegsTable();
    void clearNetBitsTable();
    void clearNetRegsTable();
    void clearTelemetryBitsTable();
    void clearTelemetryRegsTable();

public:
    explicit DialogConfigScadaMapVars(const PLCConfig &plcConfig, const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars, const std::vector<QString> &prVars, QWidget *parent = nullptr);
    ~DialogConfigScadaMapVars();
    std::vector<DialogConfigScadaMapVars::VarDescription> getCheckedVars() const;
    static QByteArray scadaMapToBytes(const std::vector<DialogConfigScadaMapVars::VarDescription> &vars);
    static std::vector<DialogConfigScadaMapVars::VarDescription> scadaMapFromBytes(QByteArray &value);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogConfigScadaMapVars *ui;
};

QDataStream &operator<<(QDataStream &stream, const DialogConfigScadaMapVars::VarDescription &item);
QDataStream &operator>>(QDataStream &stream, DialogConfigScadaMapVars::VarDescription &item);

#endif // DIALOGCONFIGSCADAMAPVARS_H
