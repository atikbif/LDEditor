#ifndef PLCCONFIG_H
#define PLCCONFIG_H

#include <QString>
#include <vector>
#include <QByteArray>

class PLCConfig
{
    std::vector<bool> input_type;
    std::vector<int> sensor_type;
    QByteArray settings;
    QString name;
public:
    PLCConfig(const QString &name="", int input_cnt=64, int ai_cnt=64);
    void setName(const QString &name);
    QString getName() const {return name;}
    void setInputType(int num, bool value);
    void setSensorType(int num, int value);
    bool getInputType(int num) const;
    int getSensorType(int num) const;
    QByteArray toBytes() const;
    QByteArray getSettings() const {return settings;}
    void setSettings(const QByteArray &inp) {settings = inp;}
    void fromBytes(QByteArray &value);
};

#endif // PLCCONFIG_H
