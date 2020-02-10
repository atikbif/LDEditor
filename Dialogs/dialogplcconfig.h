#ifndef DIALOGPLCCONFIG_H
#define DIALOGPLCCONFIG_H

#include <QDialog>
#include <QByteArray>
#include <QString>

namespace Ui {
class DialogPLCConfig;
}

class DialogPLCConfig : public QDialog
{
    Q_OBJECT
    QByteArray conf;
    const int confSize = 27;
    std::vector<quint16> convertToPLCMap(const QByteArray & value);
    QByteArray convertFromPLCMap(std::vector<quint16> value);
    std::vector<quint16> convertCurrentToPLCMap();
    bool checkCurrentConfig();
public:
    explicit DialogPLCConfig(QWidget *parent = nullptr);
    ~DialogPLCConfig();
    void setConfig(const QByteArray &value);
    QByteArray getConfig() const;
    void setCurrentIP(const QString &value);
    void setPeriodms(int value);
    int getPeriodms() const;

private slots:

    void on_pushButtonRead_clicked();

    void on_pushButtonWrite_clicked();

private:
    Ui::DialogPLCConfig *ui;
};

#endif // DIALOGPLCCONFIG_H
