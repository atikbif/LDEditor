#ifndef COPYBUFITEM_H
#define COPYBUFITEM_H

#include <QGraphicsItem>
#include "ldelement.h"

#include <QDebug>

struct BufData{
    std::vector<LDElement*> elements;

    int leftCol = 0;
    int rightCol = 0;
    int topRow = 0;
    int bottomRow = 0;
    int col = 0;
    int row = 0;
};

class CopyBufItem:public QGraphicsItem
{
    static BufData buf;
    qreal cell_width;
    qreal cell_height;

public:
    CopyBufItem(qreal cell_width, qreal cell_height):cell_width(cell_width),cell_height(cell_height){}
    int getColumn() const {return buf.col;}
    int getRow() const {return buf.row;}
    void setColumn(int value) {buf.col = value;}
    void setRow(int value) {buf.row = value;}
    void setElements(std::vector<LDElement*> els);
    std::vector<LDElement*> getElements();
    std::size_t count() const {return buf.elements.size();}
    int getWidthInColumns() const {return buf.rightCol-buf.leftCol+1;}
    int getHeightInRow() const {return buf.bottomRow-buf.topRow+1;}

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // COPYBUFITEM_H
