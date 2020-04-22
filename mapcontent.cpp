#include "mapcontent.h"
#include <QRegExp>

QString MapContent::getAppName() const
{
    return appName;
}

void MapContent::setAppName(const QString &value)
{
    appName = value;
}

QString MapContent::getAppVersion() const
{
    return appVersion;
}

void MapContent::setAppVersion(const QString &value)
{
    appVersion = value;
}

int MapContent::getNodeNum() const
{
    return nodeNum;
}

void MapContent::setNodeNum(int value)
{
    nodeNum = value;
}

int MapContent::getClusterNum() const
{
    return clusterNum;
}

void MapContent::setClusterNum(int value)
{
    clusterNum = value;
}

bool MapContent::getDisplayUsed() const
{
    return displayUsed;
}

void MapContent::setDisplayUsed(bool value)
{
    displayUsed = value;
}

int MapContent::getAppCN() const
{
    return appCN;
}

void MapContent::setAppCN(int value)
{
    appCN = value;
}

QString MapContent::getIp() const
{
    return ip;
}

void MapContent::setIp(const QString &value)
{
    ip = value;
}

QString MapContent::getMask() const
{
    return mask;
}

void MapContent::setMask(const QString &value)
{
    mask = value;
}

QString MapContent::getGateway() const
{
    return gateway;
}

void MapContent::setGateway(const QString &value)
{
    gateway = value;
}

void MapContent::addClusterGlobalBit(const MapContent::ClusterGlobalBit &bit)
{
    clustGlBits.push_back(bit);
}

std::vector<MapContent::ClusterGlobalBit> MapContent::getClusterGlobalBits() const
{
    return clustGlBits;
}

void MapContent::addClusterGlobalInteger(const MapContent::ClusterGlobalInteger &reg)
{
    clustGlIntegers.push_back(reg);
}

std::vector<MapContent::ClusterGlobalInteger> MapContent::getClusterGlobalIntegers() const
{
    return clustGlIntegers;
}

void MapContent::addNetGlobalBit(const MapContent::NetGlobalBit &bit)
{
    netGlBits.push_back(bit);
}

std::vector<MapContent::NetGlobalBit> MapContent::getNetGlobalBits() const
{
    return netGlBits;
}

void MapContent::addNetGlobalInteger(const MapContent::NetGlobalInteger &reg)
{
    netGlIntegers.push_back(reg);
}

std::vector<MapContent::NetGlobalInteger> MapContent::getNetGlobalIntegers() const
{
    return netGlIntegers;
}

void MapContent::addTelemetryBit(const MapContent::TelemetryBit &bit)
{
    telemetryBits.push_back(bit);
}

std::vector<MapContent::TelemetryBit> MapContent::getTelemetryBits() const
{
    return telemetryBits;
}

void MapContent::addTelemetryInteger(const MapContent::TelemetryInteger &reg)
{
    telemetryIntegers.push_back(reg);
}

std::vector<MapContent::TelemetryInteger> MapContent::getTelemetryIntegers() const
{
    return telemetryIntegers;
}

void MapContent::addDigitalInput(const MapContent::DigitalInput &inp)
{
    digInputs.push_back(inp);
}

std::vector<MapContent::DigitalInput> MapContent::getDigitalInputs() const
{
    return digInputs;
}

void MapContent::addAnalogInput(const MapContent::AnalogInput &inp)
{
    anInputs.push_back(inp);
}

std::vector<MapContent::AnalogInput> MapContent::getAnalogInputs() const
{
    return anInputs;
}

void MapContent::addDigitalOut(MapContent::DigitalOutput &out)
{
    digOuts.push_back(out);
}

std::vector<MapContent::DigitalOutput> MapContent::getDigitalOuts() const
{
    return digOuts;
}

MapContent::MapContent()
{

}

bool MapContent::addVar(const PLCVar &var)
{
    QString name = var.getName();
    if(var.isSystem() && !name.isEmpty()) {
        QRegExp clustBitExp("^CLBIT(\\d+)$");
        QRegExp clustRegExp("^CLREG(\\d+)$");
        QRegExp netBitExp("^NBIT(\\d+)$");
        QRegExp netRegExp("^NReg(\\d+)$");
        QRegExp scadaBitExp("^SC_BIT(\\d+)$");
        QRegExp scadaRegExp("^SC_BIT(\\d+)$");
        QRegExp diExp("^DI(\\d+)$");
        QRegExp doExp("^DO(\\d+)$");
        if(clustBitExp.indexIn(name)!=-1) {
            const int minNum = 17;
            const int maxNum = 240;
            int num = clustBitExp.cap(1).toInt();
            if(num>=minNum && num<=maxNum) {
                ClusterGlobalBit bit;
                bit.name = var.getComment();
                bit.bitNum = num;
                bit.nodeNum = 8; // FE Node num
                bit.channelNum = num;
                bit.trueName = "ON";
                bit.trueName = "OFF";
                auto it = std::find(clustGlBits.begin(),clustGlBits.end(),bit);
                if(it==clustGlBits.end()) {
                    addClusterGlobalBit(bit);
                    return true;
                }
            }
        }else if(clustRegExp.indexIn(name)!=-1){
            int num = clustRegExp.cap(1).toInt();
            const int minNum = 17;
            const int maxNum = 80;
            if(num>=minNum && num<=maxNum) {
                ClusterGlobalInteger reg;
                reg.name = var.getComment();
                reg.regNum = num;
                reg.nodeNum = 8;
                reg.channelNum = num-1;
                reg.initialValue = 0;
                auto it = std::find(clustGlIntegers.begin(),clustGlIntegers.end(),reg);
                if(it==clustGlIntegers.end()) {
                    addClusterGlobalInteger(reg);
                    return true;
                }
            }
        }else if(netBitExp.indexIn(name)!=-1){
            int num = netBitExp.cap(1).toInt();
            const int minNum = 257;
            const int maxNum = 384;
            const int txMinNum = 241;
            const int txMaxNum = 256;
            if(num>txMinNum && num<=txMaxNum && nodeNum>=0 && nodeNum<=7) {
                num+=nodeNum*16;
            }
            if(num>=minNum && num<=maxNum) {
                NetGlobalBit bit;
                bit.name = var.getComment();
                bit.bitNum = num;
                bit.nodeNum = 9;
                bit.channelNum = num-txMinNum+1;
                bit.trueName = "ON";
                bit.falseName = "OFF";
                bit.clusterNum = (num-minNum)/16;
                auto it = std::find(netGlBits.begin(),netGlBits.end(),bit);
                if(it!=netGlBits.end()) {
                    addNetGlobalBit(bit);
                    return true;
                }
            }
        }else if(netRegExp.indexIn(name)!=-1){
            int num = netRegExp.cap(1).toInt();
            const int minNum = 97;
            const int maxNum = 224;
            const int txMinNum = 81;
            const int txMaxNum = 96;
            if(num>txMinNum && num<=txMaxNum && nodeNum>=0 && nodeNum<=7) {
                num+=nodeNum*16;
            }
            if(num>=minNum && num<=maxNum) {
                NetGlobalInteger reg;
                reg.name = var.getComment();
                reg.regNum = num;
                reg.nodeNum = 9;
                reg.channelNum = num - minNum + 1;
                reg.clusterNum = reg.channelNum / 16;
                reg.initialValue = 0;
                auto it = std::find(netGlIntegers.begin(),netGlIntegers.end(),reg);
                if(it!=netGlIntegers.end()) {
                    addNetGlobalInteger(reg);
                    return true;
                }
            }
        }else if(scadaBitExp.indexIn(name)!=-1){
            int num = scadaBitExp.cap(1).toInt();
            const int minNum = 1;
            const int maxNum = 16;
            if(num>=minNum && num<=maxNum) {
                TelemetryBit bit;
                bit.funcName = var.getComment();
                bit.nodeNum = 8;
                bit.channelNum = num;
                if(nodeNum>=0 && nodeNum<=7) bit.usedByNode = nodeNum;
                else bit.usedByNode = 0;
                auto it = std::find(telemetryBits.begin(),telemetryBits.end(),bit);
                if(it!=telemetryBits.end()) {
                    addTelemetryBit(bit);
                    return true;
                }
            }
        }else if(scadaRegExp.indexIn(name)!=-1){
            int num = scadaRegExp.cap(1).toInt();
            const int minNum = 1;
            const int maxNum = 16;
            if(num>=minNum && num<=maxNum) {
                TelemetryInteger reg;
                reg.name = var.getComment();
                reg.nodeNum = 8;
                reg.channelNum = num;
                if(nodeNum>=0 && nodeNum<=7) reg.usedByNode = nodeNum;
                else reg.usedByNode = 0;
                reg.initialValue = 0;
                auto it = std::find(telemetryIntegers.begin(),telemetryIntegers.end(),reg);
                if(it!=telemetryIntegers.end()) {
                    addTelemetryInteger(reg);
                    return 1;
                }
            }
        }else if(diExp.indexIn(name)!=-1){
            int num = diExp.cap(1).toInt();
            const int minNum = 1;
            const int maxNum = 14;
            if(num>=minNum && num<=maxNum) {
                DigitalInput inp;
                inp.name = var.getComment();
                inp.inputNum = num;
                inp.nodeNum = nodeNum;
                inp.channelNum = num;
                inp.trueName = "CLOSED";
                inp.trueName = "OPEN";

                auto it = std::find(digInputs.begin(),digInputs.end(),inp);
                if(it!=digInputs.end()) {
                    addDigitalInput(inp);
                    return true;
                }
            }
        }else if(doExp.indexIn(name)!=-1){
            int num = doExp.cap(1).toInt();
            const int minNum = 1;
            const int maxNum = 6;
            if(num>=minNum && num<=maxNum) {
                DigitalOutput out;
                out.name = var.getComment();
                out.outNum = num;
                out.nodeNum = nodeNum;
                out.channelNum = 64 + num;

                auto it = std::find(digOuts.begin(),digOuts.end(),out);
                if(it!=digOuts.end()) {
                    addDigitalOut(out);
                    return true;
                }
            }
        }
    }
    return false;
}
