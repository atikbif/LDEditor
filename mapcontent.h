#ifndef MAPCONTENT_H
#define MAPCONTENT_H

#include <QString>
#include <vector>
#include <plcvar.h>

class MapContent
{
public:
    struct ClusterGlobalBit {
        int channelNum;
        int nodeNum;
        int bitNum;
        QString name;
        QString falseName;
        QString trueName;
        bool operator==(const ClusterGlobalBit &bit) {
            return this->bitNum==bit.bitNum;
        }
    };

    struct ClusterGlobalInteger {
        int channelNum;
        int nodeNum;
        int regNum;
        QString name;
        int initialValue;
        bool operator==(const ClusterGlobalInteger &reg) {
            return this->regNum==reg.regNum;
        }
    };

    struct NetGlobalBit {
        int channelNum;
        int nodeNum;
        int clusterNum;
        int bitNum;
        QString name;
        QString falseName;
        QString trueName;
        bool operator==(const NetGlobalBit &bit) {
            return this->bitNum==bit.bitNum;
        }
    };

    struct NetGlobalInteger {
        int channelNum;
        int nodeNum;
        int clusterNum;
        int regNum;
        QString name;
        int initialValue;
        bool operator==(const NetGlobalInteger &reg) {
            return this->regNum==reg.regNum;
        }
    };

    struct TelemetryBit {
        int channelNum;
        int nodeNum;
        int usedByNode;
        QString funcName;
        bool operator==(const TelemetryBit &bit) {
            return this->channelNum==bit.channelNum;
        }
    };

    struct TelemetryInteger {
        int channelNum;
        int nodeNum;
        int usedByNode;
        int initialValue;
        QString name;
        bool operator==(const TelemetryInteger &reg) {
            return this->channelNum==reg.channelNum;
        }
    };

    struct AnalogInput {
        int channelNum;
        int nodeNum;
        int inputNum;
        QString name;
        QString tdu;
        bool operator==(const AnalogInput &inp) {
            return this->inputNum==inp.inputNum && this->nodeNum==inp.nodeNum;
        }
    };

    struct DigitalInput {
        int channelNum;
        int nodeNum;
        int inputNum;
        QString name;
        QString falseName;
        QString trueName;
        bool operator==(const DigitalInput &inp) {
            return this->inputNum==inp.inputNum && this->nodeNum==inp.nodeNum;
        }
    };

    struct DigitalOutput {
        int channelNum;
        int nodeNum;
        int outNum;
        QString name;
        bool operator==(const DigitalOutput &out) {
            return this->outNum==out.outNum && this->nodeNum==out.nodeNum;
        }
    };

private:
    QString appName;
    QString appVersion;
    int nodeNum;
    int clusterNum;
    bool displayUsed;
    int appCN;
    QString ip;
    QString mask;
    QString gateway;
    std::vector<ClusterGlobalBit> clustGlBits;
    std::vector<ClusterGlobalInteger> clustGlIntegers;
    std::vector<NetGlobalBit> netGlBits;
    std::vector<NetGlobalInteger> netGlIntegers;
    std::vector<TelemetryBit> telemetryBits;
    std::vector<TelemetryInteger> telemetryIntegers;
    std::vector<DigitalInput> digInputs;
    std::vector<AnalogInput> anInputs;
    std::vector<DigitalOutput> digOuts;
public:
    MapContent();
    bool addVar(const PLCVar &var);
    QString getAppName() const;
    void setAppName(const QString &value);
    QString getAppVersion() const;
    void setAppVersion(const QString &value);
    int getNodeNum() const;
    void setNodeNum(int value);
    int getClusterNum() const;
    void setClusterNum(int value);
    bool getDisplayUsed() const;
    void setDisplayUsed(bool value);
    int getAppCN() const;
    void setAppCN(int value);
    QString getIp() const;
    void setIp(const QString &value);
    QString getMask() const;
    void setMask(const QString &value);
    QString getGateway() const;
    void setGateway(const QString &value);
    void addClusterGlobalBit(const ClusterGlobalBit &bit);
    std::vector<ClusterGlobalBit> getClusterGlobalBits() const;
    void addClusterGlobalInteger(const ClusterGlobalInteger &reg);
    std::vector<ClusterGlobalInteger> getClusterGlobalIntegers() const;
    void addNetGlobalBit(const NetGlobalBit &bit);
    std::vector<NetGlobalBit> getNetGlobalBits() const;
    void addNetGlobalInteger(const NetGlobalInteger &reg);
    std::vector<NetGlobalInteger> getNetGlobalIntegers() const;
    void addTelemetryBit(const TelemetryBit &bit);
    std::vector<TelemetryBit> getTelemetryBits() const;
    void addTelemetryInteger(const TelemetryInteger &reg);
    std::vector<TelemetryInteger> getTelemetryIntegers() const;
    void addDigitalInput(const DigitalInput &inp);
    std::vector<DigitalInput> getDigitalInputs() const;
    void addAnalogInput(const AnalogInput &inp);
    std::vector<AnalogInput> getAnalogInputs() const;
    void addDigitalOut(DigitalOutput &out);
    std::vector<DigitalOutput> getDigitalOuts() const;
};

#endif // MAPCONTENT_H
