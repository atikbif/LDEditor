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

private:
    Ui::DialogADCconfig *ui;
};

#endif // DIALOGADCCONFIG_H
