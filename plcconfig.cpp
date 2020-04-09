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
    if(num < static_cast<int>(input_type.size())) input_type[num] = value;
}

void PLCConfig::setSensorType(int num, int value)
{
    if(num < static_cast<int>(sensor_type.size())) sensor_type[num] = value;
}

bool PLCConfig::getInputType(int num) const
{
    if(num < static_cast<int>(input_type.size())) return input_type.at(num);
    return false;
}

int PLCConfig::getSensorType(int num) const
{
    if(num < static_cast<int>(sensor_type.size())) return sensor_type.at(num);
    return 0;
}

AINSensor PLCConfig::getADCSensor(int num) const
{
    if(num< static_cast<int>(adcSensors.size())) return adcSensors.at(num);
    return AINSensor();
}

void PLCConfig::addADCSensor(AINSensor sensor)
{
    adcSensors.push_back(sensor);
}

QByteArray PLCConfig::toBytes() const
{
    QByteArray result;
    QDataStream out(&result,QIODevice::WriteOnly);
    int version = 3;
    out << version;
    out << name;
    out << static_cast<int>(input_type.size());
    for(bool inp_type:input_type) out << inp_type;
    out << static_cast<int>(sensor_type.size());
    for(int sens_type:sensor_type) out << sens_type;
    out << static_cast<int>(adcSensors.size());
    for(auto sensor:adcSensors) {
        out << sensor.getName();
        out << sensor.getLowLimit();
        out << sensor.getHighLimit();
        out << sensor.getUnit();
        out << sensor.getSensorType();
    }
    out << settings;
    return result;
}

int PLCConfig::getTDUCode(int adcNum) const
{
    if(adcNum>=0 && adcNum<static_cast<int>(sensor_type.size())) {
        int type = sensor_type.at(static_cast<std::size_t>(adcNum));
        // 0 - не подключен
        if(type>0) {
            type--;
            // формирование списка сортированных имён датчиков
            std::vector<QString> sensorsNames;
            for(AINSensor sensor:adcSensors) {
                sensorsNames.push_back(sensor.getName());
            }
            std::sort(sensorsNames.begin(),sensorsNames.end());
            sensorsNames.erase( std::unique( sensorsNames.begin(), sensorsNames.end() ), sensorsNames.end() );
            if(type < static_cast<int>(sensorsNames.size())) {
                QString sensName = sensorsNames.at(static_cast<std::size_t>(type));
                // поиск датчика в неупорядоченном архиве
                auto it = std::find_if(adcSensors.begin(),adcSensors.end(),[sensName](AINSensor value){return value.getName()==sensName;});
                int tdu = static_cast<int>(std::distance(adcSensors.begin(),it));
                return tdu;
            }
        }
    }
    return 0;
}

int PLCConfig::getSensorTypeCode(int adcNum) const
{
    if(adcNum>=0 && adcNum<static_cast<int>(sensor_type.size())) {
        int type = sensor_type.at(static_cast<std::size_t>(adcNum));
        // 0 - не подключен
        if(type>0) {
            type--;
            // формирование списка сортированных имён датчиков
            std::vector<QString> sensorsNames;
            for(AINSensor sensor:adcSensors) {
                sensorsNames.push_back(sensor.getName());
            }
            std::sort(sensorsNames.begin(),sensorsNames.end());
            sensorsNames.erase( std::unique( sensorsNames.begin(), sensorsNames.end() ), sensorsNames.end() );
            if(type < static_cast<int>(sensorsNames.size())) {
                QString sensName = sensorsNames.at(static_cast<std::size_t>(type));
                // поиск датчика в неупорядоченном архиве
                auto it = std::find_if(adcSensors.begin(),adcSensors.end(),[sensName](AINSensor value){return value.getName()==sensName;});
                int res = it->getSensorType();
                return res;
            }
        }
    }
    return -1;
}

void PLCConfig::fromBytes(QByteArray &value)
{
    QDataStream in(&value,QIODevice::ReadOnly);
    int version = 0;
    in >> version;
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
    adcSensors.clear();
    if(version>=3) {
        int cnt = 0;
        in >> cnt;
        for(int i=0;i<cnt;i++) {
            QString name;
            QString lowLimit;
            QString highLimit;
            QString unit;
            int type = 0;
            in >> name;
            in >> lowLimit;
            in >> highLimit;
            in >> unit;
            in >> type;
            AINSensor sensor;
            sensor.setName(name);
            sensor.setLowLimit(lowLimit);
            sensor.setHighLimit(highLimit);
            sensor.setUnit(unit);
            sensor.setSensorType(type);
            adcSensors.push_back(sensor);
        }
    }
    if(version>=2) {
        in >> settings;
    }
}
