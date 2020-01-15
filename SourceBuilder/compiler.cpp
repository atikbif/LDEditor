#include "compiler.h"
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QProcess>
#include "plcvarcontainer.h"
#include "plcparams.h"

void Compiler::makeProgFile(const std::vector<QString> &vars, const std::vector<QString> &prog, const std::vector<QString> &funcBody, int delay)
{
    QFile progFile(getBuildPath() + "/CompileFiles/ld_prog.c");


    /*QFile osConfFile(getBuildPath() + "/IncludeFiles/Inc/os_conf.h");
    if(osConfFile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&osConfFile);
        out << "#ifndef OS_CONF_H_\r\n";
        out << "#define OS_CONF_H_\r\n";
        out << "\t#define DI_CNT\t"  + QString::number(PLCParams::diCnt)  +"\r\n";
        out << "\t#define DO_CNT\t"  + QString::number(PLCParams::doCnt)  +"\r\n";
        out << "\t#define AI_CNT\t"  + QString::number(PLCParams::aiCnt)  +"\r\n";
        out << "\t#define FRL_CNT\t"  + QString::number(PLCParams::frl_cnt)  +"\r\n";
        out << "\t#define FRD_CNT\t"  + QString::number(PLCParams::frsh_cnt)  +"\r\n";
        out << "\t#define FRSH_CNT\t"  + QString::number(PLCParams::frd_cnt)  +"\r\n";
        out << "\t#define IREG_CNT\t"  + QString::number(PLCParams::ireg_cnt)  +"\r\n";
        out << "\t#define IBIT_CNT\t"  + QString::number(PLCParams::ibit_cnt)  +"\r\n";
        out << "\t#define TMRMS_CNT\t"  + QString::number(PLCParams::tmrms_cnt)  +"\r\n";
        out << "\t#define TMRS_CNT\t"  + QString::number(PLCParams::tmrs_cnt)  +"\r\n";
        out << "#endif\r\n";
        osConfFile.close();
    }*/
    if(progFile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&progFile);
        out << "#include \"ld_prog.h\"\n";
        out << "#include \"elements.h\"\n";
        out << "#include \"iodef.h\"\n\n";
        out << "#include \"os_conf.h\"\n\n";
        out << "#include \"lib_elements.h\"\n\n";

        out << "unsigned short plc_cycle = " + QString::number(delay) + ";\n\n";

        std::vector<QString> intVars = getInternalVars();
        for(const QString &v:intVars) {
            out << v << "\n";
        }
        out << "\n";

        for(const QString &var:vars) out << var + "\n";
        out << "\n";

        out << "void inc_timers(){\n";
        for(const QString &s:funcBody) out << "\t" << s << "\n";
        out << "}\n";
        out << "\n";

        out << "void ld_process(void) {\n";
        for(const QString &line:prog) out << "\t" << line << "\n";
        out << "}\n";
        progFile.close();
    }
}

void Compiler::makeLibraryfiles(const std::set<QString> &header, const std::set<QString> &body)
{
    QFile headFile(getBuildPath() + "/IncludeFiles/Inc/lib_elements.h");
    if(headFile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&headFile);
        for(const QString &s:header) out << s << "\n";
        headFile.close();
    }
    QFile bodyFile(getBuildPath() + "/CompileFiles/lib_elements.c");
    if(bodyFile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&bodyFile);
        out << "#include \"lib_elements.h\"\n";
        out << "#include <stdlib.h>\n";
        out << "#include <math.h>\n";
        out << "\n";
        for(const QString &s:body) {
            out << s << "\n";
        }
        bodyFile.close();
    }
}

std::vector<QString> Compiler::compile(const QString &plcName, const QString &dirPath)
{
    std::vector<QString> compOut;
    // read json compile params
    QString applPath = QCoreApplication::applicationDirPath();
    QString gccPath = applPath + "/arm-gcc";
    QString corePath = getCorePath();
    QFile jsonFile(corePath+"/build.json");
    if(jsonFile.open(QFile::ReadOnly)) {
        QByteArray saveData = jsonFile.readAll();
        QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));
        QJsonObject jsonOb = jsonDoc.object();
        QString confName;
        if (jsonOb.contains("plc") && jsonOb["plc"].isArray()) {
            QJsonArray plcArray = jsonOb["plc"].toArray();
            for(const auto plc:plcArray) {
                QJsonObject plcOb = plc.toObject();
                if(plcOb.contains(plcName) && plcOb[plcName].isString()) {
                    confName = plcOb[plcName].toString();
                    break;
                }
            }
        }
        if(!confName.isEmpty()) {
            if (jsonOb.contains(confName) && jsonOb[confName].isObject()) {
                QJsonObject baseOb = jsonOb[confName].toObject();
                if (baseOb.contains("compile files") && baseOb["compile files"].isArray()) {
                    QJsonArray compFiles = baseOb["compile files"].toArray();
                    for(const auto comp:compFiles) {
                        QJsonObject compOb = comp.toObject();
                        if(compOb.contains("params") && compOb["params"].isArray()) {
                            QJsonArray compParams = compOb["params"].toArray();
                            std::vector<QString> params;
                            for(const auto param:compParams) {
                                if(param.isString()) params.push_back(param.toString());
                            }

                            QDir::setCurrent(gccPath);
                            QString program = "\"" + gccPath + "/bin/arm-none-eabi-gcc.exe\"";
                            for(const QString &p:params) program += " " + p;

                            QProcess builder;
                            builder.setProcessChannelMode(QProcess::MergedChannels);

                            builder.start(program);
                            if (!builder.waitForFinished()) {
                                QDir::setCurrent(applPath);
                                compOut.push_back(builder.errorString());
                            }
                            QDir::setCurrent(applPath);
                            QByteArray res = builder.readAll();
                            if(!res.isEmpty()) compOut.push_back(QString(res.toStdString().c_str()));
                        }
                    }
                }
                if(compOut.size()==0) {

                    // линковка
                    if(baseOb.contains("link") && baseOb["link"].isArray()) {
                        QJsonArray linkParams = baseOb["link"].toArray();
                        std::vector<QString> params;
                        for(const auto par:linkParams) {
                            if(par.isString()) params.emplace_back(par.toString());
                        }

                        QDir::setCurrent(gccPath);
                        QString program = "\"" + gccPath + "/bin/arm-none-eabi-gcc.exe\"";
                        for(const QString &p:params) program += " " + p ;

                        QProcess builder;
                        builder.setProcessChannelMode(QProcess::MergedChannels);

                        builder.start(program);
                        if (!builder.waitForFinished()) {
                            QDir::setCurrent(applPath);
                            compOut.push_back(builder.errorString());
                        }
                        QDir::setCurrent(applPath);
                        QByteArray res = builder.readAll();
                        if(!res.isEmpty()) compOut.push_back(res.toStdString().c_str());
                    }

                    if(compOut.size()==0) {
                        // преобразование в bin
                        QDir::setCurrent(gccPath);
                        if (QFile::exists(corePath+"/prog.bin")) QFile::remove(corePath+"/prog.bin");

                        QString program = "\"" + gccPath + "/bin/arm-none-eabi-objcopy.exe\"";
                        QString attr = QString(" -O binary \"") + getBuildPath() +"/prog.elf\"";
                        attr += QString(" \"") + corePath + QString("/prog.bin\"");

                        QProcess builder;
                        builder.setProcessChannelMode(QProcess::MergedChannels);
                        builder.start(program+attr);
                        if (!builder.waitForFinished()) {
                            QDir::setCurrent(applPath);
                            compOut.push_back(builder.errorString());
                        }

                        // copy file
                        if(QFile::exists(dirPath+"prog.bin")) QFile::remove(dirPath+"prog.bin");
                        QFile::copy(corePath + "/prog.bin", dirPath+"prog.bin");

                        QDir::setCurrent(applPath);
                        QByteArray res = builder.readAll();
                        if(!res.isEmpty()) compOut.push_back(res.toStdString().c_str());
                    }
                }
                //if(compOut.size()) qDebug()<<compOut;
            }else compOut.push_back("сборка " + confName + " не найдена в файле конфигурации");
        }else {
            compOut.push_back("контроллер " + plcName + " не найден в файле конфигурации");
        }
        jsonFile.close();
    }
    return compOut;
}

std::vector<QString> Compiler::getInternalVars()
{
    std::vector<QString> result;
    std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();

    result.push_back(QString("short ain[AI_CNT];"));
    result.push_back(QString("unsigned char din[DI_CNT];"));
    result.push_back(QString("unsigned char dinr[DI_CNT];"));
    result.push_back(QString("unsigned char dinf[DI_CNT];"));
    result.push_back(QString("unsigned char dout[DO_CNT];"));
    result.push_back(QString("extern unsigned char din_break[4];;"));
    result.push_back(QString("extern unsigned char din_short_circuit[4];;"));
    result.push_back(QString("extern unsigned char din_fault[4];;"));

    result.push_back(QString("unsigned char ibit[IBIT_CNT];"));
    result.push_back(QString("unsigned short tmrms[TMRMS_CNT];"));
    result.push_back(QString("unsigned short tmrs[TMRS_CNT];"));
    result.push_back(QString("short ireg[IREG_CNT];"));
    result.push_back(QString("short frsh[FRSH_CNT];"));
    result.push_back(QString("double frd[FRD_CNT];"));
    result.push_back(QString("long frl[FRL_CNT];\n\n"));

    result.push_back(QString("extern unsigned short work_time;\n\n"));

    for(const QString &group:groups) {
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group);
        for(const auto &var:vars) {
            QString vType = var.getType();
            if(vType == "bool") vType = "unsigned char";
            QString vStr = vType + " " + var.getName() + "=0;";
            result.push_back(vStr);
        }
    }
    return result;
}

QString Compiler::getCorePath()
{
    return QCoreApplication::applicationDirPath() + "/CORE";
}

QString Compiler::getBuildPath()
{
    return getCorePath() + "/" + PLCParams::osBuildName;
}
