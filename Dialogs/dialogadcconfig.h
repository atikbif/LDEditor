#ifndef DIALOGADCCONFIG_H
#define DIALOGADCCONFIG_H

#include <QDialog>

namespace Ui {
class DialogADCconfig;
}

class DialogADCconfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogADCconfig(QWidget *parent = nullptr);
    ~DialogADCconfig();

private:
    Ui::DialogADCconfig *ui;
};

#endif // DIALOGADCCONFIG_H
