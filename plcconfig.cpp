#include "plcconfig.h"
#include <QDataStream>
#include <QDebug>

PLCConfig::PLCConfig(const QString &name, int input_cnt, int ai_cnt):name(name)
{
    for(int i=0;i<input_cnt;i++) input_type.push_back(false);
    for(int i=0;i<ai_cnt;i++) sensor_type.push_back(0);
}

void PLCConfig::setName(const QString &name)
{
    this->name = name;
}

void PLCConfig::setInputType(int num, bool value)
{
    if(num<input_type.size()) input_type[num] = value;
}

void PLCConfig::setSensorType(int num, int value)
{
    if(num<sensor_type.size()) sensor_type[num] = value;
}

bool PLCConfig::getInputType(int num) const
{
    if(num<input_type.size()) return input_type.at(num);
    return false;
}

int PLCConfig::getSensorType(int num) const
{
    if(num<sensor_type.size()) return sensor_type.at(num);
    return 0;
}

QByteArray PLCConfig::toBytes() const
{
    QByteArray result;
    QDataStream out(&result,QIODevice::WriteOnly);
    int version = 2;
    out << version;
    out << name;
    out << static_cast<int>(input_type.size());
    for(bool inp_type:input_type) out << inp_type;
    out << static_cast<int>(sensor_type.size());
    for(int sens_type:sensor_type) out << sens_type;
    out << settings;
    qDebug() << "SAVE SETTINGS" << settings.size();
    return result;
}

void PLCConfig::fromBytes(QByteArray &value)
{
    QDataStream in(&value,QIODevice::ReadOnly);
    int version = 0;
    in >> version;
    if(version==1 || version==2) {
        in >> name;
        input_type.clear();
        int cnt = 0;
        in >> cnt;
        bool inp_type = false;
        for(int i=0;i<cnt;i++) {
            in >> inp_type;
            input_type.push_back(inp_type);
        }
        sensor_type.clear();
        int sens_type = 0;
        in >> cnt;
        for(int i=0;i<cnt;i++) {
            in >> sens_type;
            sensor_type.push_back(sens_type);
        }
        settings.clear();
    }
    if(version==2) {
        in >> settings;
        qDebug() << "READ SETTINGS" << settings.size();
    }
}
