#include "closedcontact.h"
#include <QDebug>

#include <QPainter>
#include <QSvgRenderer>

const QString ClosedContact::elementType = "closed contact";

class ClosedContactWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    ClosedContactWidget(qreal width, qreal height, LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
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
        Q_UNUSED(option);
        Q_UNUSED(widget);
        QSvgRenderer renderer(QString(":/images/closed_contact.svg"));
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

        QFont font("Calibri", 10, QFont::Bold);
        font.setPixelSize(static_cast<int>(height*0.4));
        painter->setFont(font);
        painter->setPen(Qt::darkBlue);
        if(el.getShowNameFlag()) {
            if(!el.getName().isEmpty()) painter->drawText(QRectF(width*0.1,height*0,width*0.8,height*0.9), Qt::AlignLeft, el.getName());
            else painter->drawText(QRectF(width*0.1,height*0,width*0.8,height*0.9), Qt::AlignLeft, el.connectedVar.name.isEmpty()?"?":el.connectedVar.name);
        }else{
            painter->drawText(QRectF(width*0.1,height*0,width*0.8,height*0.9), Qt::AlignLeft, el.connectedVar.name.isEmpty()?"?":el.connectedVar.name);
        }
    }
};

ClosedContact::ClosedContact(qreal cell_width, qreal cell_height): LDElement (cell_width,cell_height)
{
    item = (new ClosedContactWidget(cell_width,cell_height,*this));
    item->setToolTip("Нормально замкнутый контакт");
    setType("closed contact");
    needConnect = true;
    onlyRead = true;
    input_count=1;
}

LDElement *ClosedContact::clone()
{
    auto* el = new ClosedContact(cell_width,cell_height);
    //el->setSelected(isSelected());
    el->setVisible(isVisible());
    el->setColNum(col_num);
    el->setRowNum(row_num);
    el->connectedVar = connectedVar;
    el->setName(getName());
    el->setComment(getComment());
    el->setEnabled(isEnabled());
    return el;
}
