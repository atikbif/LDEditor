#include "plustwoinputs.h"
#include <QPainter>
#include <QSvgRenderer>

class PlusTwoInputsWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    PlusTwoInputsWidget(qreal width, qreal height,  LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
    {

    }

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override
    {
        return QRectF(0,-height,width,height*2);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        QSvgRenderer renderer(QString(":/images/plus_two.svg"),this);
        renderer.render(painter,QRectF(0,-height,width,height*2));
        //QPixmap pixmap(":/images/plus_two.png");
        //painter->drawPixmap(0,0,static_cast<int>(round(width)),static_cast<int>(round(height*2)),pixmap);
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
            painter->drawRect(QRectF(0,-height,width,height*2));
            painter->setBrush(br);
        }
        painter->setOpacity(1);
        //painter->drawText(QRectF(0,height*0.1,width,height*0.3), Qt::AlignCenter, el.getName());
        QFont font("Calibri", 10, QFont::Bold);
        font.setPixelSize(static_cast<int>(height*0.4));
        painter->setFont(font);
        painter->setPen(Qt::darkBlue);
        painter->drawText(QRectF(width*0.1,height*-1,width*0.8,height*0.9), Qt::AlignLeft, el.getName());

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};

PlusTwoInputs::PlusTwoInputs(qreal cell_width, qreal cell_height):LDElement (cell_width,cell_height)
{
    item = new PlusTwoInputsWidget(cell_width,cell_height,*this);
    item->setToolTip("Сложение");
    row_span = 2;
    input_count=2;
    setType("plus two inputs");
}

LDElement *PlusTwoInputs::clone()
{
    auto* el = new PlusTwoInputs(cell_width,cell_height);
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
