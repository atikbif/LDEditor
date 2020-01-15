#include "notequalelement.h"
#include <QSvgRenderer>
#include <QPainter>

class NotEqualWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    NotEqualWidget(qreal width, qreal height,  LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
    {

    }

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override
    {
        return QRectF(0,0,width,height*2);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        QSvgRenderer renderer(QString(":/images/not_equal.svg"),this);
        renderer.render(painter,QRectF(0,0,width,height*2));
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
        painter->drawText(QRectF(width*0.1,height*0,width*0.8,height*0.9), Qt::AlignLeft, el.getName());

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};

NotEqualElement::NotEqualElement(qreal cell_width, qreal cell_height):LDElement (cell_width,cell_height)
{
    item = new NotEqualWidget(cell_width,cell_height,*this);
    item->setToolTip("Неравенство");
    row_span = 2;
    input_count=2;
    setType("not equal");
}

LDElement *NotEqualElement::clone()
{
    auto* el = new NotEqualElement(cell_width,cell_height);
    el->setVisible(isVisible());
    el->setColNum(col_num);
    el->setRowNum(row_num);
    el->connectedVar = connectedVar;
    el->setName(getName());
    el->setComment(getComment());
    el->setEnabled(isEnabled());
    return el;
}
