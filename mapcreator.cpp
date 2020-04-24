#include "mapcreator.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include "xlsxformat.h"
#include <QDateTime>
#include <QDebug>

using namespace QXlsx;

MapCreator::MapCreator(const MapContent &content):content(content)
{

}

bool MapCreator::createFile(const QString &fName)
{
    QXlsx::Document xlsx;
    QXlsx::Format format1, format2;
    format1.setFontBold(true);
    format1.setFontColor(QColor(Qt::blue));
    format2.setFontBold(true);

    xlsx.addSheet("Описание");
    QString curDT = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    //qDebug() << "TIME" << curDT;
    xlsx.write("A1", "Transmission MAP \t" + curDT, format1);
    xlsx.write("A3","Имя приложения");
    xlsx.write("B3","  " + content.getAppName());
    xlsx.write("A4","Версия приложения");
    xlsx.write("B4","  " + content.getAppVersion());
    xlsx.write("A5","Дата приложения");
    xlsx.write("B5","  " + content.getAppTime());
    xlsx.write("A7","Номер кластера");
    xlsx.write("B7","  Cluster " + QString::number(content.getClusterNum()));

    xlsx.write("A10","NODE Applications", format1);
    xlsx.write("A12","Node "+QString::number(content.getNodeNum()));
    xlsx.write("B12","  PC21-1");
    xlsx.write("C12","  CN = " + QString::number(content.getAppCN()));
    xlsx.write("A13","Node 7");
    xlsx.write("B13","  PC21-FE");
    xlsx.write("C13","  CN = " + QString::number(content.getAppCN()));

    xlsx.write("A16","PC21-FE Configuration", format1);
    xlsx.write("A18","CN Number");
    xlsx.write("B18", QString("  ") + QString::number(content.getAppCN()));
    xlsx.write("A19","Node Address");
    xlsx.write("B19","  7");
    xlsx.write("A20","DHCP Disabled");
    xlsx.write("B20", "  Фиксированный IP адрес");
    xlsx.write("A21","IP адрес");
    xlsx.write("B21",QString("  ") + content.getIp());
    xlsx.write("A22","IP маска");
    xlsx.write("B22",QString("  ") + content.getMask());
    xlsx.write("A23","IP шлюз");
    xlsx.write("B23", QString("  ") + content.getGateway());

    xlsx.autosizeColumnWidth(1, 3);


    xlsx.addSheet("Входы-Выходы");
    xlsx.write("A1", "Analogue Inputs - ID 144 (0x90)", format1);
    xlsx.write("A3","  Channel",format2);
    xlsx.write("B3","  Node",format2);
    xlsx.write("C3","  Input",format2);
    xlsx.write("D3","  Name",format2);
    xlsx.write("E3","  TDU Type",format2);

    int str = 4;
    auto anInputs = content.getAnalogInputs();
    std::sort(anInputs.begin(),anInputs.end());
    for(MapContent::AnalogInput inp:anInputs) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(inp.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(inp.nodeNum));
        xlsx.write("C"+QString::number(str),QString("  ")+QString::number(inp.inputNum));
        xlsx.write("D"+QString::number(str),QString("  ")+inp.name);
        xlsx.write("E"+QString::number(str),QString("  ")+inp.tdu);
        str++;
    }

    str+=3;
    xlsx.write("A"+QString::number(str),"Digital Inputs - ID 145 (0x91)",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Input",format2);
    xlsx.write("D"+QString::number(str),"  Name",format2);
    xlsx.write("E"+QString::number(str),"  False",format2);
    xlsx.write("F"+QString::number(str),"  True",format2);
    str++;
    auto digInputs = content.getDigitalInputs();
    std::sort(digInputs.begin(),digInputs.end());
    for(MapContent::DigitalInput inp:digInputs) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(inp.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(inp.nodeNum));
        xlsx.write("C"+QString::number(str),QString("  ")+QString::number(inp.inputNum));
        xlsx.write("D"+QString::number(str),QString("  ")+inp.name);
        xlsx.write("E"+QString::number(str),QString("  ")+inp.falseName);
        xlsx.write("F"+QString::number(str),QString("  ")+inp.trueName);
        str++;
    }

    str+=3;
    xlsx.write("A"+QString::number(str),"Output Readbacks - ID 147 (0x93)",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Output",format2);
    xlsx.write("D"+QString::number(str),"  Name",format2);
    str++;
    auto digOuts = content.getDigitalOuts();
    std::sort(digOuts.begin(),digOuts.end());
    for(MapContent::DigitalOutput out:digOuts) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(out.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(out.nodeNum));
        xlsx.write("C"+QString::number(str),QString("  ")+QString::number(out.outNum));
        xlsx.write("D"+QString::number(str),QString("  ")+out.name);
        str++;
    }

    xlsx.autosizeColumnWidth(1, 6);

    xlsx.addSheet("Cluster Global Bits");
    str=1;
    xlsx.write("A"+QString::number(str),"Cluster Global Bits - ID 148 (0x94)",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Global Bit",format2);
    xlsx.write("D"+QString::number(str),"  Name",format2);
    xlsx.write("E"+QString::number(str),"  False",format2);
    xlsx.write("F"+QString::number(str),"  True",format2);
    str++;

    auto clustBits = content.getClusterGlobalBits();
    std::sort(clustBits.begin(),clustBits.end());
    for(MapContent::ClusterGlobalBit bit:clustBits) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(bit.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(bit.nodeNum));
        if(bit.bitNum<17) {
            switch(bit.bitNum) {
                case 0: xlsx.write("C"+QString::number(str),"  PC21-1");break;
                case 1: xlsx.write("C"+QString::number(str),"  PC21-CD");break;
                case 7: xlsx.write("C"+QString::number(str),"  PC21-FE");break;
            }
        }else xlsx.write("C"+QString::number(str),QString("  ")+QString::number(bit.bitNum));
        xlsx.write("D"+QString::number(str),QString("  ")+bit.name);
        xlsx.write("E"+QString::number(str),QString("  ")+bit.falseName);
        xlsx.write("F"+QString::number(str),QString("  ")+bit.trueName);
        str++;
    }
    xlsx.autosizeColumnWidth(1, 6);

    xlsx.addSheet("Cluster Global Integers");
    str=1;
    xlsx.write("A"+QString::number(str),"Cluster Global Integers - ID 149 (0x95)",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Global Int",format2);
    xlsx.write("D"+QString::number(str),"  Name",format2);
    xlsx.write("E"+QString::number(str),"  Initial Value",format2);
    str++;
    auto clustRegs = content.getClusterGlobalIntegers();
    std::sort(clustRegs.begin(),clustRegs.end());
    for(MapContent::ClusterGlobalInteger reg:clustRegs) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(reg.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(reg.nodeNum));
        xlsx.write("C"+QString::number(str),QString("  ")+QString::number(reg.regNum));
        xlsx.write("D"+QString::number(str),QString("  ")+reg.name);
        xlsx.write("E"+QString::number(str),QString("  ")+QString::number(reg.initialValue));
        str++;
    }
    xlsx.autosizeColumnWidth(1, 5);

    xlsx.addSheet("Net Global Bits");
    str=1;
    xlsx.write("A"+QString::number(str),"Net Global Bits - ID 150 (0x96)",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Cluster",format2);
    xlsx.write("D"+QString::number(str),"  Global Bit",format2);
    xlsx.write("E"+QString::number(str),"  Name",format2);
    xlsx.write("F"+QString::number(str),"  False",format2);
    xlsx.write("G"+QString::number(str),"  True",format2);
    str++;

    auto netBits = content.getNetGlobalBits();
    std::sort(netBits.begin(),netBits.end());
    for(MapContent::NetGlobalBit bit:netBits) {
        xlsx.write("B"+QString::number(str),"  9");
        if(bit.bitNum<257) {
            if(bit.bitNum>=0 && bit.bitNum<8) {
                xlsx.write("A"+QString::number(str),QString("  ")+QString::number(bit.bitNum+1));
                xlsx.write("C"+QString::number(str),"  --");
                xlsx.write("D"+QString::number(str),"  --");
                xlsx.write("E"+QString::number(str),"  Cluster "+QString::number(bit.bitNum));
                xlsx.write("F"+QString::number(str),"  Offline");
                xlsx.write("G"+QString::number(str),"  Online");
            }
        }else {
            xlsx.write("A"+QString::number(str),QString("  ")+QString::number(bit.channelNum));
            xlsx.write("C"+QString::number(str),QString("  ")+QString::number(bit.clusterNum));
            xlsx.write("D"+QString::number(str),QString("  ")+QString::number(bit.bitNum));
            xlsx.write("E"+QString::number(str),QString("  ")+bit.name);
            xlsx.write("F"+QString::number(str),QString("  ")+bit.falseName);
            xlsx.write("G"+QString::number(str),QString("  ")+bit.trueName);
        }
        str++;
    }
    xlsx.autosizeColumnWidth(1, 7);

    xlsx.addSheet("Net Global Integers");
    str=1;
    xlsx.write("A"+QString::number(str),"Net Global Integers 1 (97 to 160) - ID 151 (0x97)",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Cluster",format2);
    xlsx.write("D"+QString::number(str),"  Global Int",format2);
    xlsx.write("E"+QString::number(str),"  Name",format2);
    xlsx.write("F"+QString::number(str),"  Initial Value",format2);
    str++;
    auto netRegs = content.getNetGlobalIntegers();
    std::sort(netRegs.begin(),netRegs.end());
    for(MapContent::NetGlobalInteger reg:netRegs) {
        if(reg.regNum>=97 && reg.regNum<=160) {
            xlsx.write("A"+QString::number(str),QString("  ")+QString::number(reg.channelNum));
            xlsx.write("B"+QString::number(str),QString("  ")+QString::number(reg.nodeNum));
            xlsx.write("C"+QString::number(str),QString("  ")+QString::number(reg.clusterNum));
            xlsx.write("D"+QString::number(str),QString("  ")+QString::number(reg.regNum));
            xlsx.write("E"+QString::number(str),QString("  ")+reg.name);
            xlsx.write("F"+QString::number(str),QString("  ")+QString::number(reg.initialValue));
            str++;
        }else break;
    }

    str+=2;
    xlsx.write("A"+QString::number(str),"Net Global Integers 2 (161 to 224) - ID 152 (0x98)",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Cluster",format2);
    xlsx.write("D"+QString::number(str),"  Global Int",format2);
    xlsx.write("E"+QString::number(str),"  Name",format2);
    xlsx.write("F"+QString::number(str),"  Initial Value",format2);
    str++;
    for(MapContent::NetGlobalInteger reg:netRegs) {
        if(reg.regNum>=161 && reg.regNum<=224) {
            xlsx.write("A"+QString::number(str),QString("  ")+QString::number(reg.channelNum));
            xlsx.write("B"+QString::number(str),QString("  ")+QString::number(reg.nodeNum));
            xlsx.write("C"+QString::number(str),QString("  ")+QString::number(reg.clusterNum));
            xlsx.write("D"+QString::number(str),QString("  ")+QString::number(reg.regNum));
            xlsx.write("E"+QString::number(str),QString("  ")+reg.name);
            xlsx.write("F"+QString::number(str),QString("  ")+QString::number(reg.initialValue));
            str++;
        }
    }

    xlsx.autosizeColumnWidth(1, 6);

    xlsx.addSheet("Telemetry");
    str=1;
    xlsx.write("A"+QString::number(str),"Telemetry Bit Controls",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Used by Node",format2);
    xlsx.write("D"+QString::number(str),"  Function",format2);
    str++;
    auto telBits = content.getTelemetryBits();
    std::sort(telBits.begin(),telBits.end());
    for(MapContent::TelemetryBit bit:telBits) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(bit.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(bit.nodeNum));
        xlsx.write("C"+QString::number(str),QString("  ")+QString::number(bit.usedByNode));
        xlsx.write("D"+QString::number(str),QString("  ")+bit.funcName);
        str++;
    }

    str+=2;
    xlsx.write("A"+QString::number(str),"Telemetry Int Controls",format1);
    str+=2;
    xlsx.write("A"+QString::number(str),"  Channel",format2);
    xlsx.write("B"+QString::number(str),"  Node",format2);
    xlsx.write("C"+QString::number(str),"  Used by Node",format2);
    xlsx.write("D"+QString::number(str),"  Function",format2);
    xlsx.write("E"+QString::number(str),"  Initial Value",format2);
    str++;
    auto telRegs = content.getTelemetryIntegers();
    std::sort(telRegs.begin(),telRegs.end());
    for(MapContent::TelemetryInteger reg:telRegs) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(reg.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(reg.nodeNum));
        xlsx.write("C"+QString::number(str),QString("  ")+QString::number(reg.usedByNode));
        xlsx.write("D"+QString::number(str),QString("  ")+reg.name);
        xlsx.write("E"+QString::number(str),QString("  ")+QString::number(reg.initialValue));
        str++;
    }

    xlsx.autosizeColumnWidth(1, 5);

    xlsx.selectSheet("Описание");
    return xlsx.saveAs(fName+".xlsx");
}
