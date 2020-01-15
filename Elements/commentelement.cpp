#include "commentelement.h"
#include <QPainter>
#include "dialogaddeditcomment.h"
#include <QFont>

class CommentWidget: public QGraphicsWidget
{
    qreal width, height;
    LDElement &el;
public:
    CommentWidget(qreal width, qreal height, LDElement &el, QGraphicsItem *parent=nullptr): QGraphicsWidget(parent),width(width),height(height), el(el)
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
        auto *ptr = dynamic_cast<CommentElement*>(&el);


        if(el.isSelected()) {
            painter->setOpacity(0.3);
            painter->fillRect(QRectF(0,0,width,height),QBrush(Qt::red));

        }else {
            painter->setOpacity(0.9);
            painter->fillRect(QRectF(0,0,width,height),QBrush(QColor(0x0E,0xFF,0x00,100)));
        }
        if(ptr) {
            QFont font("Times", 10, QFont::Bold);
            font.setPixelSize(static_cast<int>(height*0.5));
            painter->setFont(font);
            painter->drawText(QPointF(10,height*0.8),ptr->getProjectComment());
        }
        painter->setOpacity(1);

    }
};

CommentElement::CommentElement(qreal cell_width, qreal cell_height, int cell_count):LDElement(cell_width,cell_height),cell_count(cell_count)
{
    item = (new CommentWidget(cell_width*cell_count,cell_height,*this));
    item->setToolTip("Нормально замкнутый контакт");
    col_span = cell_count;
    setType("comment");
}

LDElement *CommentElement::clone()
{
    auto* el = new CommentElement(cell_width,cell_height,cell_count);
    el->setVisible(isVisible());
    el->setColNum(col_num);
    el->setRowNum(row_num);
    el->prComment = prComment;
    el->connectedVar = connectedVar;
    el->setName(getName());
    el->setComment(getComment());
    el->setEnabled(isEnabled());
    return el;
}

void CommentElement::updateProperties()
{
    auto *dialog = new DialogAddEditComment(prComment);
    if(dialog->exec()==QDialog::Accepted) {
        prComment=dialog->getComment();
    }
    delete dialog;
}
