#include "twoinputelement.h"

#include <QSvgRenderer>
#include <QPainter>
#include <utility>

class TwoInputWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
    QString iconFile;
public:
    TwoInputWidget(qreal width, qreal height,  LDElement &el, const QString &iconFile, QGraphicsItem *parent=nullptr):
        QGraphicsWidget(parent),width(width),height(height), el(el), iconFile(iconFile)
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
        //QPixmap pixmap(":/images/multiple_two.png");
        //painter->drawPixmap(0,0,static_cast<int>(round(width)),static_cast<int>(round(height*2)),pixmap);
        QSvgRenderer renderer(iconFile,this);
        renderer.render(painter,QRectF(0,-height,width,height*2));
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

        QFont font("Calibri", 10, QFont::Bold);
        font.setPixelSize(static_cast<int>(height*0.4));
        painter->setFont(font);
        painter->setPen(Qt::darkBlue);
        if(el.getShowNameFlag()) {
            painter->drawText(QRectF(width*0.1,height*-1,width*0.8,height*0.9), Qt::AlignLeft, el.getName());
        }

        //painter->drawText(QRectF(0,height*0.1,width,height*0.3), Qt::AlignCenter, el.getName());

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};

TwoInputElement::TwoInputElement(qreal cell_width, qreal cell_height, const QString icon):
    LDElement (cell_width,cell_height), iconFile(std::move(icon))
{
    item = new TwoInputWidget(cell_width,cell_height,*this,iconFile);
    row_span = 2;
    input_count=2;
    //setType("two input");
}

LDElement *TwoInputElement::clone()
{
    auto* el = new TwoInputElement(cell_width,cell_height,iconFile);
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
