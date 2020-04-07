#include "copybufitem.h"
#include <QPainter>
#include "plcvarcontainer.h"

BufData CopyBufItem::buf;

void CopyBufItem::setElements(std::vector<LDElement*> els) {
    if(!els.empty()) {
        LDElement *el = els.at(0);
        buf.leftCol = el->getColNum();
        buf.rightCol = el->getColSpan()-1+buf.leftCol;
        buf.topRow = el->getRowNum() - el->getRowSpan() + 1;
        buf.bottomRow = el->getRowSpan()-1+buf.topRow;
        for(LDElement *el:buf.elements) delete el;
        buf.elements.clear();
        for(LDElement *el:els) {
            LDElement *copyEl = el->clone();
            copyEl->setSelected(false);
            int elLeftCol = copyEl->getColNum();
            int elRightCol = copyEl->getColSpan()-1+elLeftCol;
            int elTopRow = copyEl->getRowNum() - el->getRowSpan() + 1;
            int elBottomRow = copyEl->getRowSpan()-1+elTopRow;
            if(elLeftCol<buf.leftCol) buf.leftCol=elLeftCol;
            if(elRightCol>buf.rightCol) buf.rightCol=elRightCol;
            if(elTopRow<buf.topRow) buf.topRow=elTopRow;
            if(elBottomRow>buf.bottomRow) buf.bottomRow=elBottomRow;
            buf.elements.push_back(copyEl);
        }
        for(LDElement *el:buf.elements) {
            el->setColNum(el->getColNum()-buf.leftCol);
            el->setRowNum(el->getRowNum()-buf.topRow);
        }
        buf.rightCol-=buf.leftCol;
        buf.bottomRow-=buf.topRow;
        buf.leftCol = 0;
        buf.topRow = 0;
    }else {
        buf.leftCol=buf.rightCol=buf.topRow=buf.bottomRow=0;
        setVisible(false);
    }

}

std::vector<LDElement*> CopyBufItem::getElements() {
    std::vector<LDElement*> res;
    for(LDElement *el:buf.elements) {
        res.push_back(el->clone());
    }
    return  res;
}

void CopyBufItem::updateElementVarName(LDElement *el, const QString &newVarName)
{
    for(LDElement *element:buf.elements) {
        if(element->isNeedConnect()) {
            if(element->getColNum()==el->getColNum() && element->getRowNum()==el->getRowNum()) {
                element->connectedVar.name = newVarName;
                auto var = PLCVarContainer::getInstance().getVarByName(newVarName);
                if(var) element->setName(var->getComment());
            }
        }
    }
}

QRectF CopyBufItem::boundingRect() const
{
    //return QRectF(-10000,-10000,100000,100000);
    qreal offset = 50;
    return QRectF(-offset,-offset,cell_width*(buf.rightCol-buf.leftCol+1)+offset,cell_height*(buf.bottomRow-buf.topRow+1)+offset);
}

void CopyBufItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    for(LDElement *el:buf.elements) {
        QGraphicsItem *item = el->getItem();
        qreal offset_x = el->getColNum()*cell_width;
        qreal offset_y = el->getRowNum()*cell_height;
        painter->translate(offset_x,offset_y);
        painter->setOpacity(0.3);
        item->paint(painter, option, widget);
        painter->translate(-offset_x,-offset_y);
    }
}
