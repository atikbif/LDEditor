#include "plcparams.h"
#include <QFile>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

int PLCParams::aiCnt=0;
int PLCParams::diCnt=0;
int PLCParams::doCnt=0;

int PLCParams::ibit_cnt=0;
int PLCParams::ireg_cnt=0;
int PLCParams::tmrms_cnt=0;
int PLCParams::tmrs_cnt=0;
int PLCParams::frsh_cnt=0;
int PLCParams::frd_cnt=0;
int PLCParams::frl_cnt=0;
int PLCParams::code=0;
 QString PLCParams::osBuildName="";


void PLCParams::readParamsByPLCName(const QString &plcName)
{
    QString applPath = QCoreApplication::applicationDirPath();
    QString corePath = applPath + "/CORE";
    QFile jsonFile(corePath+"/plc.json");
    if(jsonFile.open(QFile::ReadOnly)) {
        QByteArray saveData = jsonFile.readAll();
        QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));
        QJsonObject jsonOb = jsonDoc.object();
        QString confName;
        if (jsonOb.contains(plcName) && jsonOb[plcName].isObject()) {
            QJsonObject plcOb = jsonOb[plcName].toObject();
            if(plcOb.contains("AI_CNT")) aiCnt = plcOb["AI_CNT"].toInt();
            if(plcOb.contains("DI_CNT")) diCnt = plcOb["DI_CNT"].toInt();
            if(plcOb.contains("DO_CNT")) doCnt = plcOb["DO_CNT"].toInt();
            if(plcOb.contains("OS_PARAMS")) confName = plcOb["OS_PARAMS"].toString();
            if(plcOb.contains("CODE")) code = plcOb["CODE"].toInt();
            osBuildName = confName;
            if(!confName.isEmpty()) {
                if (jsonOb.contains(confName) && jsonOb[confName].isObject()) {
                    QJsonObject osOb = jsonOb[confName].toObject();
                    if(osOb.contains("IBIT_CNT")) ibit_cnt = osOb["IBIT_CNT"].toInt();
                    if(osOb.contains("IREG_CNT")) ireg_cnt = osOb["IREG_CNT"].toInt();
                    if(osOb.contains("TMRMS_CNT")) tmrms_cnt = osOb["TMRMS_CNT"].toInt();
                    if(osOb.contains("TMRS_CNT")) tmrs_cnt = osOb["TMRS_CNT"].toInt();
                    if(osOb.contains("FRL_CNT")) frl_cnt = osOb["FRL_CNT"].toInt();
                    if(osOb.contains("FRD_CNT")) frd_cnt = osOb["FRD_CNT"].toInt();
                    if(osOb.contains("FRSH_CNT")) frsh_cnt = osOb["FRSH_CNT"].toInt();
                }
            }
        }
    }
}

std::vector<QString> PLCParams::readPLCNames()
{
    std::vector<QString> result;
    QString applPath = QCoreApplication::applicationDirPath();
    QString corePath = applPath + "/CORE";
    QFile jsonFile(corePath+"/plc.json");
    if(jsonFile.open(QFile::ReadOnly)) {
        QByteArray saveData = jsonFile.readAll();
        QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));
        QJsonObject jsonOb = jsonDoc.object();
        if (jsonOb.contains("plc") && jsonOb["plc"].isArray()) {
            QJsonArray plcArray = jsonOb["plc"].toArray();
            for(const auto plcOb:plcArray) {
                result.push_back(plcOb.toString());
            }
        }
    }
    return result;
}
