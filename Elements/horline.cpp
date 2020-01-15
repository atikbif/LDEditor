#include "horline.h"
#include <QPainter>

class HorLineWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    HorLineWidget(qreal width, qreal height, LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
    {

    }

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override
    {
        qreal y = height/4*3;
        return QRectF(0,y-5,width,10);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setRenderHint(QPainter::Antialiasing);
        qreal y = height/4*3;

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

        painter->setPen(QPen(Qt::black,3));
        painter->drawLine(QPointF(0,y),QPointF(width,y));
    }
};

HorLine::HorLine(qreal cell_width, qreal cell_height):LDElement (cell_width,cell_height)
{
    item = (new HorLineWidget(cell_width,cell_height,*this));
    setType("horizontal line");
}

LDElement *HorLine::clone()
{
    auto* el = new HorLine(cell_width,cell_height);
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
