#include "serialize.h"
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include "elementlibrary.h"

static qreal cell_width = 0;
static qreal cell_height = 0;
static int column_count = 1;

struct LDElementProperties {
    QString name;
    QString comment;
    int column = 0;
    int row = 0;
    QString varGroup;
    QString varName;
    bool isEnabled = false;
};

static void setLDElementProperties(LDElement &item, const LDElementProperties &properties) {
    item.setName(properties.name);
    item.setComment(properties.comment);
    item.setColNum(properties.column);
    item.setRowNum(properties.row);
    item.connectedVar.group = properties.varGroup;
    item.connectedVar.name = properties.varName;
    item.setEnabled(properties.isEnabled);
}

static std::map<QString,std::function<LDElement*(const LDElementProperties&)>> ld_map {
    {"closed contact",[](const LDElementProperties &properties){
        auto *item = new ClosedContact(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"const variable",[](const LDElementProperties &properties){
        auto *item = new ConstVar(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"divide",[](const LDElementProperties &properties){
        auto *item = new DivideElement(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"equal",[](const LDElementProperties &properties){
        auto *item = new EqualElement(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"greather than",[](const LDElementProperties &properties){
        auto *item = new GreaterElement(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"greather or equal",[](const LDElementProperties &properties){
        auto *item = new GreaterOrEqual(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"horizontal line",[](const LDElementProperties &properties){
        auto *item = new HorLine(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"vertical line",[](const LDElementProperties &properties){
        auto *item = new VerLine(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"less than",[](const LDElementProperties &properties){
        auto *item = new LessElement(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"less or equal",[](const LDElementProperties &properties){
        auto *item = new LessOrEqual(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"minus",[](const LDElementProperties &properties){
        auto *item = new MinusElement(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"multiple two inputs",[](const LDElementProperties &properties){
        auto *item = new MultipleTwoInputs(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"not equal",[](const LDElementProperties &properties){
        auto *item = new NotEqualElement(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"open contact",[](const LDElementProperties &properties){
        auto *item = new OpenContact(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"plus two inputs",[](const LDElementProperties &properties){
        auto *item = new PlusTwoInputs(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"relay",[](const LDElementProperties &properties){
        auto *item = new Relay(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"relay_enabled",[](const LDElementProperties &properties){
        auto *item = new RelayEnabled(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"variable",[](const LDElementProperties &properties){
        auto *item = new Variable(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"delay on",[](const LDElementProperties &properties){
        auto *item = new DelayOn(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"delay off",[](const LDElementProperties &properties){
        auto *item = new DelayOff(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"rs trigger",[](const LDElementProperties &properties){
        auto *item = new RSTrigger(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"sr trigger",[](const LDElementProperties &properties){
        auto *item = new SRTrigger(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"comment",[](const LDElementProperties &properties){
        auto *item = new CommentElement(cell_width,cell_height,column_count);
        setLDElementProperties(*item,properties);
        return item;
    }},
    {"not",[](const LDElementProperties &properties){
        auto *item = new NotElement(cell_width,cell_height);
        setLDElementProperties(*item,properties);
        return item;
    }}
};

void savePLCVarContainer(QDataStream &stream) {
    stream << QString("Var container");
    std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
    auto groupCnt = static_cast<int>(groups.size());
    stream << groupCnt;
    for(const QString &group:groups) {
        stream << QString("group");
        stream << group;
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group);
        auto varCnt = static_cast<int>(vars.size());
        stream << varCnt;
        for(const PLCVar &var:vars) {
            stream << QString("var");
            stream << var.getName();
            stream << var.getComment();
            stream << var.getType();
        }
    }

}

void readPLCVarContainer(QDataStream &stream) {
    QString tmpStr;
    stream >> tmpStr;
    if(tmpStr=="Var container") {
        // удалить текущие несистемные переменные
        std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
        for(const QString &group:groups) {
            PLCVarContainer::getInstance().delGroup(group);
        }
        int groupCnt = 0;
        stream >> groupCnt;
        for(int groupNum=0;groupNum<groupCnt;groupNum++) {
            stream >> tmpStr;
            if(tmpStr=="group") {

                QString groupName;
                stream >> groupName;
                int varCnt = 0;
                stream >> varCnt;
                for(int i=0;i<varCnt;i++) {
                    stream >> tmpStr;
                    if(tmpStr=="var") {

                        QString varName;
                        QString varComment;
                        QString varType;
                        stream >> varName;
                        stream >> varComment;
                        stream >> varType;

                        PLCVar v(varName,groupName);
                        v.setComment(varComment);
                        if(varType=="short") {v.setValue(static_cast<short>(0));}
                        else if(varType=="long") {v.setValue(0l);}
                        else if(varType=="double") {v.setValue(0.0);}
                        else if(varType=="bool") {v.setValue(false);}
                        v.setReadable(true);
                        v.setWriteable(true);
                        PLCVarContainer::getInstance().addVar(v);
                    }
                }
            }
        }
        PLCVarContainer::getInstance().saveState();
    }
}

QDataStream &operator<<(QDataStream &stream, const LDElement &item)
{
    stream << QString("LD Element");
    stream << item.getType();
    stream << item.getName();
    stream << item.getComment();
    stream << item.getColNum();
    stream << item.getRowNum();
    stream << item.connectedVar.group;
    stream << item.connectedVar.name;
    stream << item.isEnabled();
    if(item.type=="comment") {
        const auto *el = dynamic_cast<const CommentElement*>(&item);
        if(el) stream << *el;
    }else if(item.type=="const variable") {
        const auto *el = dynamic_cast<const ConstVar*>(&item);
        if(el) stream << *el;
    }else if(item.type.contains("library:")) {
        const auto *el = dynamic_cast<const LibraryElement*>(&item);
        if(el) {
            QFileInfo fInfo(el->getFileName());
            stream << fInfo.fileName();
        }
    }
    return stream;
}

std::pair<LDElement *, QString> readLDelement(QDataStream &stream)
{
    QString tmpStr;
    stream >> tmpStr;
    if(tmpStr=="LD Element") {
        QString type;
        LDElementProperties properties;

        stream >> type;
        stream >> properties.name;
        stream >> properties.comment;
        stream >> properties.column;
        stream >> properties.row;
        stream >> properties.varGroup;
        stream >> properties.varName;
        stream >> properties.isEnabled;

        auto it = ld_map.find(type);
        if(it!=ld_map.end()) {
            LDElement *item = ld_map[type](properties);
            if(type=="comment") {
                auto *el = dynamic_cast<CommentElement*>(item);
                if(el) {
                    QString projectComment;
                    stream >> projectComment;
                    el->setProjectComment(projectComment);
                }
            }else if(type=="const variable") {
                auto *el = dynamic_cast<ConstVar*>(item);
                if(el) {
                    QString varType;
                    QString varValue;
                    stream >> varType;
                    stream >> varValue;
                    el->setConstVartype(varType);
                    el->setStringValue(varValue);
                }
            }
            return {item,"OK"};
        }else if(type.contains("library:")) {

            QString fName;
            stream >> fName;
            fName=ElementLibrary::getLibraryPath()+"/"+fName;
            if(QFile::exists(fName)) {
                LibraryElement *item = new LibraryElement(cell_width,cell_height,fName);
                setLDElementProperties(*item,properties);
                return {item,"OK"};
            }else {
                return {nullptr,"<b><font color=\"red\">отсутствует библиотека </font></b>"  +fName};
            }
        }
        return {nullptr,"<b><font color=\"red\">ошибка чтения элемента </font></b>"  +type};
    }
    return {nullptr,"<b><font color=\"red\">ошибка чтения элемента, неверный формат файла</font></b>"};
}


/*-------------------------------------------------------COMMENT ELEMENT----------------------------------------------------------------*/

QDataStream &operator<<(QDataStream &stream, const CommentElement &item)
{
    stream << item.getProjectComment();
    return stream;
}

/*-------------------------------------------------------CONST VAR----------------------------------------------------------------------*/

QDataStream &operator<<(QDataStream &stream, const ConstVar &item)
{
    stream << item.getConstVarType();
    stream << item.getStringValue();
    return stream;
}

void setCellConfig(qreal width, qreal height, int col_count)
{
    cell_width = width;
    cell_height = height;
    column_count = col_count;
}
