#ifndef PLCCONFIG_H
#define PLCCONFIG_H

#include <QString>
#include <vector>
#include <QByteArray>
#include "ainsensor.h"

class PLCConfig
{
    std::vector<bool> input_type;
    std::vector<int> sensor_type;
    std::vector<AINSensor> adcSensors;
    QByteArray settings;
    QByteArray scadaMap;
    QString name;
    quint16 appCN=0;
    quint16 appCRC = 0;
    QString appName;
    int nodeNum;
    int clusterNum;
    QString appVersion;
    QString appTime;
public:
    PLCConfig(const QString &name="", int input_cnt=64, int ai_cnt=64);
    void setName(const QString &name);
    QString getName() const {return name;}
    void setInputType(int num, bool value);
    void setSensorType(int num, int value);
    bool getInputType(int num) const;
    int getSensorType(int num) const;
    void clearADCSensors() {adcSensors.clear();}
    std::size_t getADCSensorsCount() const {return adcSensors.size();}
    AINSensor getADCSensor(int num) const;
    std::vector<AINSensor> getADCSensors() const {return adcSensors;}
    void addADCSensor(AINSensor sensor);
    QByteArray toBytes() const;
    QByteArray getSettings() const {return settings;}
    int getTDUCode(int adcNum) const;
    int getSensorTypeCode(int adcNum) const;
    void setSettings(const QByteArray &inp) {settings = inp;}
    void fromBytes(QByteArray &value);
    quint16 getApplicationCN() const {return appCN;}
    void setApplicationCN(quint16 value) {appCN = value;}
    QString getIP() const;
    QString getIPMask() const;
    QString getIPGate() const;
    QString getAppName() const;
    void setAppName(const QString &value);
    int getNodeNum() const;
    void setNodeNum(int value);
    int getClusterNum() const;
    void setClusterNum(int value);
    QString getAppVersion() const;
    void setAppVersion(const QString &value);
    QString getAppTime() const;
    void setAppTime(const QString &value);
    quint16 getAppCRC() const;
    void setAppCRC(const quint16 &value);
    void clearScadaMap();
    QByteArray getScadaMap() const;
    void setScadaMap(const QByteArray &value);
};

#endif // PLCCONFIG_H
