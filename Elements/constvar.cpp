#include "constvar.h"

#include <QSvgRenderer>
#include <QPainter>
#include "dialogconstvarproperties.h"

class ConstVariableWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    ConstVariableWidget(qreal width, qreal height,  LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
    {

    }

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override
    {
        return QRectF(0,0,width,height);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        //QPixmap pixmap(":/images/open_contact.png");
        //painter->drawPixmap(0,0,static_cast<int>(round(width)),static_cast<int>(round(height)),pixmap);
        QSvgRenderer renderer(QString(":/images/constant_var.svg"));
        renderer.render(painter,QRectF(0,0,width,height));

        if(el.isSelected() || !el.isEnabled()) {
            QBrush br = painter->brush();
            if(!el.isEnabled()) {
                if(el.isSelected()) {
                    painter->setBrush(Qt::lightGray);
                    painter->setPen(QPen(Qt::red,2,Qt::DotLine));
                }else {
                    painter->setBrush(Qt::darkGray);
                    painter->setPen(Qt::lightGray);
                }
            }else {
                painter->setBrush(Qt::yellow);
                painter->setPen(QPen(Qt::red,2,Qt::DotLine));
            }

            painter->setOpacity(0.3);
            painter->drawRect(QRectF(0,0,width,height));
            painter->setBrush(br);
        }

        painter->setOpacity(1);
        auto *ptr = dynamic_cast<ConstVar*>(&el);
        if(ptr) {
            QString stringValue = ptr->getStringValue();
            if(!stringValue.isEmpty()) {
                QFont font("Calibri", 10, QFont::Bold);
                font.setPixelSize(static_cast<int>(height*0.35));
                painter->setFont(font);
                painter->setPen(Qt::darkBlue);
                QString varType = ptr->getConstVarType();
                varType.replace("unsigned ","u");
                if(!varType.isEmpty()) stringValue += "(" + varType + ")";
                painter->drawText(QRectF(width*0.1,height*0,width*0.8,height*0.9), Qt::AlignLeft, stringValue);
            }
        }


        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};

QString ConstVar::getStringValue() const
{
    QString stringValue;
    if(auto pval = std::get_if<unsigned short>(&value)) stringValue = QString::number(*pval);
    if(auto pval = std::get_if<unsigned long>(&value)) stringValue = QString::number(*pval);
    if(auto pval = std::get_if<double>(&value)) stringValue = QString::number(*pval);
    if(auto pval = std::get_if<bool>(&value)) {
        stringValue = (*pval?"1":"0");
    }
    if(auto pval = std::get_if<QString>(&value)) stringValue = (*pval);
    return stringValue;
}

void ConstVar::setStringValue(const QString &varValue)
{
    if(constVarType=="unsigned short") {
        auto res = static_cast<quint16>(varValue.toUInt());
        value = static_cast<unsigned short>(res);
    }else if(constVarType=="unsigned long") {
        quint32 res = varValue.toULong();
        value = static_cast<unsigned long>(res);
    }else if(constVarType=="double") {
        double res = varValue.toDouble();
        value = res;
    }else if(constVarType=="bool") {
        bool res = varValue.toShort();
        value = res;
    }
}

ConstVar::ConstVar(qreal cell_width, qreal cell_height): LDElement (cell_width,cell_height)
{
    item = new ConstVariableWidget(cell_width,cell_height,*this);
    item->setToolTip("Константа");
    setType("const variable");
    input_count=1;
}

void ConstVar::setValue(const PLCVar::varType &value)
{
    this->value = value;
    if(std::holds_alternative<unsigned short>(value)) constVarType = "unsigned short";
    else if(std::holds_alternative<unsigned long>(value)) constVarType = "unsigned long";
    else if(std::holds_alternative<double>(value)) constVarType = "double";
    else if(std::holds_alternative<QString>(value)) constVarType = "string";
    else if(std::holds_alternative<bool>(value)) constVarType = "bool";
}

LDElement *ConstVar::clone()
{
    auto* el = new ConstVar(cell_width,cell_height);
    //el->setSelected(isSelected());
    el->setVisible(isVisible());
    el->setColNum(col_num);
    el->setRowNum(row_num);
    el->connectedVar = connectedVar;
    el->setName(getName());
    el->setComment(getComment());
    el->setEnabled(isEnabled());
    el->setValue(value);
    return el;
}

void ConstVar::updateProperties()
{
    DialogConstVarProperties *dialog = new DialogConstVarProperties(value,constVarType);
    if(dialog->exec()==QDialog::Accepted) {
        constVarType = dialog->getType();
        value = dialog->getValue();
    }
    delete dialog;
}
