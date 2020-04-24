#ifndef DIALOGAPPLICATIONCONFIG_H
#define DIALOGAPPLICATIONCONFIG_H

#include <QDialog>
#include "plcconfig.h"

namespace Ui {
class DialogApplicationConfig;
}

class DialogApplicationConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogApplicationConfig(const PLCConfig &config, QWidget *parent = nullptr);
    ~DialogApplicationConfig();
    QString getApplicationName() const;
    QString getApplicationVersion() const;
    int getClusterNum() const;
    int getNodeNum() const;
    int getAppCN() const;

private:
    Ui::DialogApplicationConfig *ui;

    // QDialog interface
public slots:
    void accept();
};

#endif // DIALOGAPPLICATIONCONFIG_H
