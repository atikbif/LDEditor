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
    qDebug() << "TIME" << curDT;
    xlsx.write("A1", "Transmission MAP \t" + curDT, format1);
    xlsx.write("A3","Имя приложения");
    xlsx.write("B3","  " + content.getAppName());
    xlsx.write("A4","Версия приложения");
    xlsx.write("B4","  " + content.getAppVersion());
    xlsx.write("A5","Дата приложения");
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
    for(MapContent::AnalogInput inp:content.getAnalogInputs()) {
        xlsx.write("A"+QString::number(str),QString("  ")+QString::number(inp.channelNum));
        xlsx.write("B"+QString::number(str),QString("  ")+QString::number(inp.nodeNum));
        xlsx.write("C"+QString::number(str),QString("  ")+QString::number(inp.inputNum));
        xlsx.write("D"+QString::number(str),QString("  ")+inp.name);
        xlsx.write("E"+QString::number(str),QString("  ")+inp.tdu);
        str++;
    }

    xlsx.autosizeColumnWidth(1, 5);
    xlsx.selectSheet("Описание");


    return xlsx.saveAs(fName+".xlsx");
}
