#ifndef LDELEMENT_H
#define LDELEMENT_H

#include <QGraphicsWidget>
#include "plcvarcontainer.h"

class LDElement
{
protected:
    int row_num=0;
    int col_num=0;
    int row_span=1;
    int col_span=1;
    int input_count=0;
    bool visible=true;
    bool selected=false;
    qreal cell_width=0;
    qreal cell_height=0;
    QGraphicsWidget *item = nullptr;
    QString name = "";
    QString type = "";
    bool needConnect = false;
    bool onlyRead = false;
    bool onlyWrite = false;
    QString comment="";
    bool enabled=true;
    bool showName=false;
public:
    friend QDataStream &operator<<(QDataStream &stream, const LDElement &item);
    struct {
        QString name;
        QString group;
        QString parentGroup;
    }connectedVar;
    LDElement(qreal cell_width=0, qreal cell_height=0);
    QGraphicsWidget* getItem();
    virtual LDElement* clone() = 0;
    QString getName() const {return name;}
    void setName(const QString &value) {name=value;}
    QString getType() const {return type;}
    void setType(const QString &value) {type = value;}
    int getRowNum() const {return row_num;}
    void setRowNum(int value) {row_num=value;}
    int getColNum() const {return col_num;}
    void setColNum(int value) {col_num=value;}
    int getRowSpan() const {return row_span;}
    int getColSpan() const {return col_span;}
    int getInputCount() const {return input_count;}
    bool isVisible() const {return visible;}
    bool isSelected() const {return selected;}
    void setVisible(bool value) {visible = value;}
    void setSelected(bool value) {selected = value;}
    bool isNeedConnect() const {return needConnect;}
    virtual void updateProperties();
    QString getComment() const {return comment;}
    void setComment(const QString &value) {comment=value;}
    bool isOnlyRead() const {return onlyRead;}
    bool isOnlyWrite() const {return  onlyWrite;}
    void setEnabled(bool value) {enabled=value;}
    bool isEnabled() const { return enabled;}
    void setShowNameFlag(bool value);
    bool getShowNameFlag() const {return showName;}
    virtual ~LDElement();
};

struct LDInfo{
    static const std::map<QString,QString> types;
    LDInfo(LDElement &el) {
        if(types.find(el.getType())!=types.end()) type = types.at(el.getType());
        else type = el.getType();
        name = el.getName();
        if(el.connectedVar.parentGroup.isEmpty()) info=""; else info = el.connectedVar.parentGroup+": ";
        info += el.connectedVar.group.isEmpty()?el.connectedVar.name: el.connectedVar.group + ": " + el.connectedVar.name;
        std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(el.connectedVar.group,el.connectedVar.name);
        if(v && !v->getComment().isEmpty()) info+= " (" + v->getComment() + ")";
        comment = el.getComment();
        col = el.getColNum();
        row = el.getRowNum();
    }
    LDInfo() {
        name="";
        type="";
        info="";
        comment="";
        col = 1;
        row = 1;
    }
    QString name;
    QString type;
    QString info;
    QString comment;
    int col;
    int row;
};


#endif // LDELEMENT_H
