#ifndef DIALOGDICONFIG_H
#define DIALOGDICONFIG_H

#include <QDialog>

namespace Ui {
class DialogDIConfig;
}

class DialogDIConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDIConfig(QWidget *parent = nullptr);
    ~DialogDIConfig();
    void setInputs(const std::vector<QString> names);
    std::vector<QString> getInputs() const;

private:
    Ui::DialogDIConfig *ui;
};

#endif // DIALOGDICONFIG_H
