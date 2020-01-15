#ifndef DIALOGPROJECTCONFIG_H
#define DIALOGPROJECTCONFIG_H

#include <QDialog>

namespace Ui {
class DialogProjectConfig;
}

class DialogProjectConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProjectConfig(int msCnt = 10, QWidget *parent = nullptr);
    ~DialogProjectConfig();
    int getDelay() const;
    int getNetAddress() const;
    void setNetAddress(int value);
    QString getBaudrate() const;
    void setBaudrate(const QString &value);
    QString getParity() const;
    void setParity(const QString &value);
    int getStopBits() const;
    void setStopBits(int value);
private:
    Ui::DialogProjectConfig *ui;
};

#endif // DIALOGPROJECTCONFIG_H
