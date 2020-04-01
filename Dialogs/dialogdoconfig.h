#ifndef DIALOGDOCONFIG_H
#define DIALOGDOCONFIG_H

#include <QDialog>

namespace Ui {
class DialogDOConfig;
}

class DialogDOConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDOConfig(QWidget *parent = nullptr);
    ~DialogDOConfig();
    void setOuts(const std::vector<QString> names);
    std::vector<QString> getOuts() const;


private:
    Ui::DialogDOConfig *ui;
};

#endif // DIALOGDOCONFIG_H
