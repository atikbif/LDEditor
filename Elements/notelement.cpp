#include "notelement.h"
#include <QSvgRenderer>
#include <QPainter>

class NotWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    NotWidget(qreal width, qreal height,  LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
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
        QSvgRenderer renderer(QString(":/images/not.svg"));
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
        //painter->drawText(QRectF(width*0.1,height*0.05,width*0.8,height*0.9), Qt::AlignLeft, " " + el.connectedVar.name);
        QFont font("Calibri", 10, QFont::Bold);
        font.setPixelSize(static_cast<int>(height*0.4));
        painter->setFont(font);
        painter->setPen(Qt::darkBlue);
        painter->drawText(QRectF(width*0.1,height*0,width*0.8,height*0.9), Qt::AlignLeft, el.getName());

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};

NotElement::NotElement(qreal cell_width, qreal cell_height):LDElement (cell_width,cell_height)
{
    item = new NotWidget(cell_width,cell_height,*this);
    item->setToolTip("Отрицание");
    row_span = 1;
    setType("not");
    needConnect = false;
    input_count=1;
}

LDElement *NotElement::clone()
{
    auto* el = new NotElement(cell_width,cell_height);
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
