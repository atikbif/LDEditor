#ifndef DIALOGPROJECTCONFIG_H
#define DIALOGPROJECTCONFIG_H

#include <QDialog>

namespace Ui {
class DialogProjectConfig;
}

class DialogProjectConfig : public QDialog
{
    Q_OBJECT
    QString _plcName;

public:
    explicit DialogProjectConfig(const QString &plcName, QWidget *parent = nullptr);
    ~DialogProjectConfig();
    int getNetAddress() const;
    void setNetAddress(int value);
    QString getBaudrate() const;
    void setBaudrate(const QString &value);
    QString getParity() const;
    void setParity(const QString &value);
    int getStopBits() const;
    void setStopBits(int value);
    QString getIP() const;
    void setIP(const QString &value);
    bool useIPasDefault() const;
    void setIPasDefault(bool value);
private:
    Ui::DialogProjectConfig *ui;
};

#endif // DIALOGPROJECTCONFIG_H
