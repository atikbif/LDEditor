#ifndef DIALOGCYCLECONFIG_H
#define DIALOGCYCLECONFIG_H

#include <QDialog>

namespace Ui {
class DialogCycleConfig;
}

class DialogCycleConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCycleConfig(int msCnt = 10, QWidget *parent = nullptr);
    ~DialogCycleConfig();
    int getDelay() const;

private:
    Ui::DialogCycleConfig *ui;
};

#endif // DIALOGCYCLECONFIG_H
