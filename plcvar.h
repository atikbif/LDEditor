#ifndef PLCVAR_H
#define PLCVAR_H

#include <QString>
#include <variant>
#include <vector>

class PLCVar
{
public:
    using varType = std::variant<bool,short,long,double,QString>;
private:
    QString group;
    QString name;
    QString comment;
    bool readable=false;
    bool writeable=false;
    varType value;
    bool system=false;
public:
    PLCVar(const QString &vName,QString vGroup=""):group(vGroup),name(vName) {value=static_cast<short>(0);}
    void setComment(const QString &vComment) {comment = vComment;}
    void setGroup(const QString &name) {group=name;}
    void setSystem(bool value) {system = value;}
    bool isSystem() const {return system;}
    QString getComment() const {return comment;}
    QString getGroup() const {return group;}
    QString getName() const {return name;}
    static std::vector<QString> getSupportedTypes() {
        return {"short","long","double","bool"};
    }
    QString getType() const {
        QString res;
        if(std::holds_alternative<short>(value)) res = "short";
        else if(std::holds_alternative<long>(value)) res = "long";
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
    int getIntValue() const {if(auto pval = std::get_if<short>(&value)) return *pval;else return 0;}
    long getLongValue() const {if(auto pval = std::get_if<long>(&value)) return *pval;else return 0;}
    double getDoubleValue() const {if(auto pval = std::get_if<double>(&value)) return *pval;else return 0;}
    bool getBoolvalue() const {if(auto pval = std::get_if<bool>(&value)) return *pval;else return false;}
};

#endif // PLCVAR_H
