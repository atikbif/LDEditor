#include "libraryelement.h"
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class LibraryWidget: public QGraphicsWidget
{
    qreal width, height;
    LibraryElement &el;
public:
    LibraryWidget(qreal width, qreal height,  LibraryElement &el, QGraphicsItem *parent=nullptr):
        QGraphicsWidget(parent),width(width),height(height), el(el)
    {

    }

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override
    {
        return QRectF(0,-(el.getRowSpan()-1)*height,width,el.getRowSpan()*height);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(QPen(Qt::black,3,Qt::SolidLine));
        painter->drawRect(width*0.2,-(el.getRowSpan()-1)*height+height*0.5,width*0.6,el.getRowSpan()*height-height*0.55);
        QFont font("Calibri", 10, QFont::Bold);
        font.setPixelSize(static_cast<int>(height*0.4));
        painter->setFont(font);
        painter->setPen(QPen(Qt::black,2.5,Qt::SolidLine));
        for(int i=0;i<el.getRowSpan();i++) {
            painter->drawLine(QPointF(0,(i-(el.getRowSpan()-1)+0.75)*height),QPointF(width*0.2-1,(i-(el.getRowSpan()-1)+0.75)*height));
            painter->drawText(QRectF(width*0.25,(i-(el.getRowSpan()-1)+0.50)*height,width*0.6,height*0.4), Qt::AlignLeft, el.getInputName(i));
        }
        painter->drawLine(width*0.8,0.75*height,width,0.75*height);

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
            painter->drawRect(boundingRect());
            painter->setBrush(br);
        }
        painter->setOpacity(1);

        painter->setPen(Qt::darkBlue);
        painter->drawText(QRectF(width*0.1,-(el.getRowSpan()-1)*height+height*0,width*0.8,height*0.9), Qt::AlignLeft, el.getName());


        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};

LibraryElement::LibraryElement(qreal cell_width, qreal cell_height, const QString &fName):LDElement (cell_width,cell_height),
    fName(fName)
{
    setType("library: ");

    QFile file(fName);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray saveData = file.readAll();
        QJsonDocument elDoc(QJsonDocument::fromJson(saveData));
        auto elOb = elDoc.object();
        if(elOb.contains("Input vars") && elOb["Input vars"].isArray()) {
             auto inputs = elOb["Input vars"].toArray();
             for(auto input:inputs) {
                 auto inpOb = input.toObject();
                 if(inpOb.contains("name") && inpOb["name"].isString()) inpNames.push_back(inpOb["name"].toString());
                 if(inpOb.contains("type") && inpOb["type"].isString()) inpTypes.push_back(inpOb["type"].toString());
             }
        }
        if(elOb.contains("version") && elOb["version"].isDouble()) {version = elOb["version"].toInt();}
        if(elOb.contains("Prototype") && elOb["Prototype"].isString()) {prototype=elOb["Prototype"].toString();}
        if(elOb.contains("Result") && elOb["Result"].isString()) {resType=elOb["Result"].toString();}
        if(elOb.contains("Body") && elOb["Body"].isArray()) {
            auto funcStrings = elOb["Body"].toArray();
            for(auto funcStr:funcStrings) {
                funcBody.push_back(funcStr.toString());
            }
        }
        if(elOb.contains("Short name") && elOb["Short name"].isString()) {
            setName(elOb["Short name"].toString());
        }
        if(elOb.contains("Name") && elOb["Name"].isString()) {
            setComment(elOb["Name"].toString());
            setType(getType() + getComment());
        }
        if(elOb.contains("Function name") && elOb["Function name"].isString()) {
            funcName = elOb["Function name"].toString();
        }
    }
    if(inpNames.empty()) {
        row_span = 1;
        input_count = 1;
        inpNames.push_back("?");
    }else {
        row_span = inpNames.size();
        input_count = row_span;
    }

    item = new LibraryWidget(cell_width,cell_height,*this);
}

QString LibraryElement::getInputName(int num) const
{
    if(num>=0 && num<inpNames.size()) return inpNames.at(num);
    return QString();
}

LDElement *LibraryElement::clone()
{
    auto* el = new LibraryElement(cell_width,cell_height,fName);
    el->setVisible(isVisible());
    el->setColNum(col_num);
    el->setRowNum(row_num);
    el->setType(type);
    el->connectedVar = connectedVar;
    el->setName(getName());
    el->setComment(getComment());
    el->setEnabled(isEnabled());
    return el;
}

