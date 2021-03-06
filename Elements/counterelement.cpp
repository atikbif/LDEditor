#include "counterelement.h"

#include <QSvgRenderer>
#include <QPainter>

class CounterWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    CounterWidget(qreal width, qreal height,  LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
    {

    }

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override
    {
        return QRectF(0,-3*height,width,height*4);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        QSvgRenderer renderer(QString(":/images/counter.svg"),this);
        renderer.render(painter,QRectF(0,-3*height,width,height*4));
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
            painter->drawRect(QRectF(0,-3*height,width,height*4));
            painter->setBrush(br);
        }
        painter->setOpacity(1);
        //painter->drawText(QRectF(0,height*0.1,width,height*0.3), Qt::AlignCenter, el.getName());

        QFont font("Calibri", 10, QFont::Bold);
        font.setPixelSize(static_cast<int>(height*0.4));
        painter->setFont(font);
        painter->setPen(Qt::darkBlue);
        painter->drawText(QRectF(width*0.1,height*-3,width*0.8,height*0.9), Qt::AlignLeft, el.getName());

        Q_UNUSED(option)
        Q_UNUSED(widget)
    }
};


CounterElement::CounterElement(qreal cell_width, qreal cell_height): LDElement(cell_width,cell_height)
{
    item = new CounterWidget(cell_width,cell_height,*this);
    item->setToolTip("Счётчик");
    row_span = 4;
    input_count=4;
    setType("counter");
}

LDElement *CounterElement::clone()
{
    auto* el = new CounterElement(cell_width,cell_height);
    el->setVisible(isVisible());
    el->setColNum(col_num);
    el->setRowNum(row_num);
    el->connectedVar = connectedVar;
    el->setName(getName());
    el->setComment(getComment());
    el->setEnabled(isEnabled());
    return el;
}
