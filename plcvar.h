#ifndef PLCVAR_H
#define PLCVAR_H

#include <QString>
#include <variant>
#include <vector>
#include <QDebug>

class PLCVar
{
public:
    using varType = std::variant<bool,unsigned short,unsigned long,double,QString>;
private:
    QString group;
    QString name;
    QString parentGroup;
    QString comment;
    bool readable=false;
    bool writeable=false;
    varType value;
    bool system=false;
public:
    PLCVar(const QString &vName,QString vGroup="",QString vParentGroup=""):group(vGroup),name(vName),parentGroup(vParentGroup) {value=static_cast<unsigned short>(0);}
    void setComment(const QString &vComment) {comment = vComment;}
    void setGroup(const QString &name) {group=name;}
    void setParentGroup(const QString &name) {parentGroup=name;}
    void setSystem(bool value) {system = value;}
    bool isSystem() const {return system;}
    QString getComment() const {return comment;}
    QString getGroup() const {return group;}
    QString getParentGroup() const {return parentGroup;}
    QString getName() const {return name;}
    static std::vector<QString> getSupportedTypes() {
        return {"unsigned short","unsigned long","double","bool"};
    }
    QString getType() const {
        QString res;
        if(std::holds_alternative<unsigned short>(value)) res = "unsigned short";
        else if(std::holds_alternative<unsigned long>(value)) res = "unsigned long";
        else if(std::holds_alternative<double>(value)) res = "double";
        else if(std::holds_alternative<QString>(value)) res = "string";
        else if(std::holds_alternative<bool>(value)) res = "bool";
        return res;
    }
    void setReadable(bool vReadable) {readable=vReadable;}
    void setWriteable(bool vWriteable) {writeable=vWriteable;}
    bool isReadable() const {return  readable;}
    bool isWriteable() const {return writeable;}
    varType getValue() {return value;}
    void setValue(const varType &v) {value = v;}
    QString getStringValue() const {if(auto pval = std::get_if<QString>(&value)) return *pval;else return QString();}
    unsigned int getIntValue() const {if(auto pval = std::get_if<unsigned short>(&value)) return *pval;else return 0;}
    unsigned long getLongValue() const {if(auto pval = std::get_if<unsigned long>(&value)) return *pval;else return 0;}
    double getDoubleValue() const {if(auto pval = std::get_if<double>(&value)) return *pval;else return 0;}
    bool getBoolvalue() const {if(auto pval = std::get_if<bool>(&value)) return *pval;else return false;}
    QString getValueAsString() const
    {
        QString res;
        if(auto pval = std::get_if<unsigned short>(&value)) return QString::number(*pval);
        if(auto pval = std::get_if<unsigned long>(&value)) return QString::number(*pval);
        if(auto pval = std::get_if<double>(&value)) return QString::number(*pval);
        if(auto pval = std::get_if<bool>(&value)) {
            if(*pval) return "1";else return "0";
        }
        return QString();
    }
    bool setValueAsString(const QString &inp)
    {
        bool check = false;
        if(std::holds_alternative<unsigned short>(value)) {
            ushort v = inp.toUShort(&check);
            if(check) value = v;
        }else if(std::holds_alternative<unsigned long>(value)) {
            ulong v = inp.toULong(&check);
            if(check) value = v;
        }else if(std::holds_alternative<double>(value)) {
            double v = inp.toDouble(&check);
            if(check) value = v;
        }
        else if(std::holds_alternative<QString>(value)) {
            value = inp;
        }else if(std::holds_alternative<bool>(value)) {
             int v = inp.toInt(&check);
             if(check) {
                 if(v) value = true;else value=false;
             }else {
                 if(inp.toLower().contains("true")) {value = true;check=true;}
                 else if(inp.toLower().contains("false")) {value = false;check=true;}
             }
        }
        return check;
    }
};

#endif // PLCVAR_H
