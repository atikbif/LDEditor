#include "ldscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QtMath>
#include <QDebug>
#include <QString>
#include <QLCDNumber>
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QList>
#include <QMenu>
#include "horline.h"
#include "verline.h"

#include <algorithm>
#include <QList>
#include <QScrollBar>
#include "dialogaddeditcomment.h"
#include <QApplication>


void LDScene::createHorLine(int row, int col1, int col2)
{
    std::vector<LDElement*> appendElements;
    std::vector<LDElement*> appendOverElements;

    int col = 0;
    int target_col = 0;
    bool col_direction_right = col1>col2?false:true;

    if(col_direction_right) {col = col1+1;target_col=col2+1;}
    else {col = col1;target_col = col2;}
    while(col!=target_col) {
        std::vector<LDElement*> els = getElement(col,row);
        bool content_widget = false;
        bool over_flag = false;
        for(LDElement *el:els) {
            if(el) {
                if((el->getType()=="vertical line")||(el->getType()=="comment")) over_flag = true;
                else content_widget = true;
            }
        }
        if(!content_widget) {
            auto *line = new HorLine(page.cell_width,page.cell_height);
            line->setColNum(col);line->setRowNum(row);
            if(over_flag) appendOverElements.push_back(line);
            else appendElements.push_back(line);
        }
        if(col_direction_right) col++;else col--;
    }
    addElements(appendElements);
    addElements(appendOverElements);
}

void LDScene::createVerLine(int col, int row1, int row2)
{
    std::vector<LDElement*> appendOverElements;

    bool row_direction_up = row1<row2?false:true;

    int row = 0;
    int target_row = 0;
    if(row_direction_up) {row = row1;target_row=row2;}
    else {row = row1+1;target_row = row2+1;}

    while(row!=target_row) {

        std::vector<LDElement*> els = getElement(col,row);
        bool content_ver_line = false;
        for(LDElement *el:els) {
            if(el) {
                if(el->getType()=="vertical line") content_ver_line = true;
            }
        }
        if(!content_ver_line) {
            auto *line = new VerLine(page.cell_width,page.cell_height);
            line->setColNum(col);line->setRowNum(row);
            appendOverElements.push_back(line);
        }
        if(row_direction_up) row--;else row++;
    }

    addElements(appendOverElements);
}

void LDScene::setSelectionArea(qreal x1, qreal y1, qreal x2, qreal y2)
{
    if(x2<x1) {
        if(y2<y1) {
            selectionItem->setRect(x2,y2,x1-x2,y1-y2);
        }else {
            selectionItem->setRect(x2,y1,x1-x2,y2-y1);
        }
    }else {
        if(y2<y1) {
            selectionItem->setRect(x1,y2,x2-x1,y1-y2);
        }else {
            selectionItem->setRect(x1,y1,x2-x1,y2-y1);
        }
    }
}

void LDScene::drawInsertElement(qreal x, qreal y)
{
    if(element) {
        int col = qFloor(x/page.cell_width);
        int row = qFloor(y/page.cell_height);

        while(col>page.column_count+1-element->getColSpan()) col--;
        while(row>page.row_count+1/*-element->getRowSpan()*/) row--;

        x = col*page.cell_width;
        y = row*page.cell_height;

        if((col>0)&&(col<=page.column_count)&&(row>0)&&(row<=page.row_count)) {
            if((last_row!=row)||(last_col!=col)) {
                QGraphicsItem *item = element->getItem()->graphicsItem();
                item->setVisible(true);
                item->setOpacity(0.4);
                item->setPos(x,y);
                last_row = row;
                last_col = col;
            }
        }else {
            QGraphicsItem *item = element->getItem()->graphicsItem();
            item->setVisible(false);
        }

    }
}

void LDScene::drawCopyBuf(qreal x, qreal y)
{
    if(copyItem && copyItem->isVisible()) {
        int col = qFloor(x/page.cell_width);
        int row = qFloor(y/page.cell_height);
        while(col>page.column_count+1-copyItem->getWidthInColumns()) col--;
        while(row>page.row_count+1-copyItem->getHeightInRow()) row--;
        copyItem->setColumn(col);
        copyItem->setRow(row);
        x = col*page.cell_width;
        y = row*page.cell_height;
        if((col>0)&&(row>0)) {
            copyItem->setPos(x,y);
        }
    }
}

void LDScene::drawConnectionLine(qreal x, qreal y, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
    if(lMode.on_flag) {
        int col = qFloor((x-page.cell_width*0.5)/page.cell_width);
        int row = qFloor((y-page.cell_height*0.25)/page.cell_height);
        qreal radius = page.cell_height/8;
        if((col>=0)&&(col<=page.column_count)&&(row>0)&&(row<=page.row_count)) {

            x = col*page.cell_width+page.cell_width;
            y = row*page.cell_height+page.cell_height/4*3;
            if(lMode.state==LineMode::cmdState::FIRST_POINT) {
                lMode.firstPointItem->setRect(x-radius,y-radius,radius*2,radius*2);
                lMode.firstPointItem->setBrush(Qt::lightGray);
                lMode.firstPointItem->setVisible(true);
            }else if(lMode.state==LineMode::cmdState::SECOND_POINT) {
                lMode.secondPointItem->setRect(x-radius,y-radius,radius*2,radius*2);
                lMode.secondPointItem->setBrush(Qt::lightGray);
                lMode.secondPointItem->setVisible(true);
                lMode.lineItem->setLine(QLineF(lMode.firstPointItem->rect().center(),lMode.secondPointItem->rect().center()));
                lMode.lineItem->setVisible(true);
            }
        }

        if (buttons & Qt::LeftButton) {
            if(stack && !stack->isGroupMode()) stack->startGroupCmd();
            if((col>=0)&&(col<=page.column_count)&&(row>0)&&(row<=page.row_count)) {
                if(lMode.state==LineMode::cmdState::FIRST_POINT) {
                    lMode.state=LineMode::cmdState::SECOND_POINT;
                    lMode.firsPointCol = col;
                    lMode.firstPointRow = row;
                }else if(lMode.state==LineMode::cmdState::SECOND_POINT) {
                    bool horMode = true;
                    if(modifiers & Qt::ControlModifier) horMode = false;
                    lMode.secondPointCol = col;
                    lMode.secondPointRow = row;
                    createConnection(lMode.firsPointCol,lMode.firstPointRow,lMode.secondPointCol,lMode.secondPointRow,horMode);
                    lMode.clear();
                    lMode.firsPointCol = col;
                    lMode.firstPointRow = row;
                    lMode.firstPointItem->setRect(x-radius,y-radius,radius*2,radius*2);
                    lMode.firstPointItem->setBrush(Qt::lightGray);
                    lMode.firstPointItem->setVisible(true);
                    lMode.state=LineMode::cmdState::SECOND_POINT;
                    lMode.on_flag = true;
                }
            }
        }else if(buttons & Qt::RightButton) {
            setLineInsertMode(false);
        }
    }
}

void LDScene::selectAreaElements()
{
    for(LDElement *el:elements) {

        QPointF p1 = el->getItem()->boundingRect().topLeft();
        QPointF scP1 = el->getItem()->mapToScene(p1);
        QPointF p2 = el->getItem()->boundingRect().bottomRight();
        QPointF scP2 = el->getItem()->mapToScene(p2);

        QRectF itRect = QRectF(scP1,scP2);


        if(selectionItem->rect().contains(itRect)) {
            el->setSelected(true);
            selectedElements.push_back(el);
            el->getItem()->graphicsItem()->update();
        }
    }
    selectionItem->setRect(0,0,0,0);
}

void LDScene::getFreePlace(int colStart, int rowEnd, int colEnd, int rowStart)
{
    std::vector<LDElement*> delElements;
    for(int col=colStart;col<=colEnd;col++) {
        for(int row=rowStart;row<=rowEnd;row++) {
            std::vector<LDElement*> els = getElement(col,row);
            els.erase(std::remove_if(els.begin(), els.end(), [](LDElement *ptr){return (ptr->getType()=="vertical line" || ptr->getType()=="comment");}),els.end());
            std::copy(els.begin(),els.end(),std::back_inserter(delElements));
        }
    }
    removeElements(delElements);
}

void LDScene::toggleSelect(qreal x, qreal y)
{
    LDElement *cursorElement = getElementByPos(x,y);
    selElCol = 0;
    selElRow = 0;
    if(cursorElement) {
        if(cursorElement->isSelected()) {
            cursorElement->setSelected(false);
            auto it = std::find(selectedElements.begin(),selectedElements.end(),cursorElement);
            if(it!=selectedElements.end()) selectedElements.erase(it);
        }else {
            selElCol = cursorElement->getColNum();
            selElRow = cursorElement->getRowNum();
            cursorElement->setSelected(true);
            selectedElements.push_back(cursorElement);
        }
        cursorElement->getItem()->update();
    }
}

LDElement *LDScene::getElementByPos(qreal x, qreal y)
{
    LDElement *cursorElement = nullptr;
    qreal elementArea = sceneRect().width()*sceneRect().height();
    for(LDElement *el:elements) {
        QPointF p1 = el->getItem()->boundingRect().topLeft();
        QPointF scP1 = el->getItem()->mapToScene(p1);
        QPointF p2 = el->getItem()->boundingRect().bottomRight();
        QPointF scP2 = el->getItem()->mapToScene(p2);

        QRectF itRect = QRectF(scP1,scP2);
        qreal area = itRect.width()*itRect.height();
        if(itRect.contains(QPointF(x,y))) {
            if(area<elementArea) {
                elementArea = area;
                cursorElement = el;
            }
        }
    }
    return cursorElement;
}

void LDScene::setNamesFlag(bool value)
{
    showNames = value;
    for(LDElement *el:elements) {
        el->setShowNameFlag(value);
        el->getItem()->update();
    }
}

int LDScene::getLastCol() const
{
    return selElCol;
}

int LDScene::getLastRow() const
{
    return selElRow;
}

LDScene::LDScene(LDPage &page, QObject *parent):
    QGraphicsScene (parent), page(page),cell_width(1),cell_height(1)
{
    //setBackgroundBrush(QColor(0xF2,0xF5,0xC9));
    setSceneRect(0,0,page.cell_width*(page.column_count+2),page.cell_height*(page.row_count+2));

    QGraphicsLineItem *item = addLine(page.cell_width,page.cell_height,page.cell_width,height()-page.cell_height,QPen(Qt::darkRed,4));
    item->setOpacity(1);
    QGraphicsLineItem *item2 = addLine(page.cell_width*(page.column_count+1),page.cell_height,page.cell_width*(page.column_count+1),height()-page.cell_height,QPen(Qt::darkRed,4));
    item2->setOpacity(1);

    for(int i=0;i<page.column_count+1;i++) {
        QGraphicsLineItem *item = addLine((i+1)*page.cell_width,page.cell_height,(i+1)*page.cell_width,height()-page.cell_height,QPen(Qt::darkGray));
        item->setOpacity(0.15);
    }
    for(int i=0;i<page.row_count+1;i++) {
        QGraphicsLineItem *item = addLine(page.cell_width,(i+1)*page.cell_height,page.cell_width*(page.column_count+1),(i+1)*page.cell_height,QPen(Qt::darkGray));
        item->setOpacity(0.15);
    }


    for(int i=0;i<page.row_count;i++)  {
        auto *ptr = new QLCDNumber(4);
        ptr->setSegmentStyle(QLCDNumber::Flat);
        ptr->display(i+1);
        ptr->setFixedHeight(static_cast<int>(page.cell_height*0.7));
        ptr->setFixedWidth(static_cast<int>(page.cell_width*0.4));

        QPalette palette = ptr->palette();

        palette.setColor(palette.WindowText, Qt::white);
        palette.setColor(palette.Background, Qt::lightGray);//QColor(0, 170, 255));
        palette.setColor(palette.Light, Qt::white);
        palette.setColor(palette.Dark, Qt::black);

        ptr->setPalette(palette);

        QGraphicsWidget *item = addWidget(ptr);
        item->setPos(page.cell_width*0.5,page.cell_height*(i+1.25));
        //QGraphicsItem *item = addText(QString::number(i+1),QFont("Times",static_cast<int>(page.cell_height*0.5), QFont::Bold));
        //item->setPos(page.cell_width*0.5,page.cell_height*(i+1));
    }

    selectionItem = addRect(0,0,0,0,QPen(Qt::blue,1),QBrush(Qt::green));
    selectionItem->setOpacity(0.15);

    lMode.lineItem = addLine(0,0,0,0);
    lMode.lineItem->setVisible(false);

    lMode.firstPointItem = addEllipse(0,0,0,0);
    lMode.firstPointItem->setVisible(false);

    lMode.secondPointItem = addEllipse(0,0,0,0);
    lMode.secondPointItem->setVisible(false);

}

void LDScene::empty_page()
{
    delete element;
    element = nullptr;
    if((stack)&&(stack->isWorking())) {
        stack->removeElements(elements);
    }
    for(LDElement *el:elements) {
        removeItem(el->getItem()->graphicsItem());
        delete el;
    }
    elements.clear();
    selectedElements.clear();
    lMode.clear();
    update(sceneRect());
    //if(views().size()) connect(views().at(0)->horizontalScrollBar(),&QScrollBar::valueChanged,this,[this](){update(sceneRect());});
}

void LDScene::setInsertElement(LDElement *element, bool delete_previous)
{
    lMode.on_flag = false;
    if(delete_previous) delete this->element;
    if(element!=nullptr) {
        copyItem->setVisible(false);
        QGraphicsItem *item = element->getItem();
        addItem(item);
        item->setVisible(false);
    }
    this->element = element;
}

bool LDScene::addElement(LDElement *el, int col, int row)
{
    if(el==nullptr) return false;
    if((col>0)&&(col<=page.column_count)&&(row>0)&&(row<=page.row_count)) {
        QGraphicsWidget* widget = el->getItem();
        widget->graphicsItem()->setOpacity(1);
        widget->setVisible(false);
        widget->setPos(page.cell_width*col,page.cell_height*row);
        addItem(widget);
        widget->setVisible(true);
        el->setColNum(col);
        el->setRowNum(row);
        elements.push_back(el);
        if((stack)&&(stack->isWorking())) stack->addElement(el);
        el->setShowNameFlag(showNames);
        //el->getItem()->ensureVisible();
        update(sceneRect());
        return true;
    }
    return false;
}

void LDScene::addElements(std::vector<LDElement *> &elementList)
{
    for(LDElement *el:elementList) {
        addElement(el,el->getColNum(),el->getRowNum());
    }
}

std::vector<LDElement *> LDScene::getElement(int col, int row, bool anyPoint)
{
    std::vector<LDElement*> els;
    for(LDElement *el:elements) {
        int leftBottomElCol = el->getColNum();
        int leftBottomElRow = el->getRowNum();
        if(anyPoint) {
            int rightTopElCol = leftBottomElCol + el->getColSpan() - 1;
            int rightTopElRow = leftBottomElRow-el->getRowSpan()+1;
            if((col>=leftBottomElCol)&&(col<=rightTopElCol)&&(row>=rightTopElRow)&&(row<=leftBottomElRow)) {
                els.push_back(el);
            }
        }else {
            if((col==leftBottomElCol)&&(row==leftBottomElCol)) {
                els.push_back(el);
            }
        }
    }
    return els;
}

void LDScene::removeElement(int col, int row, const QString &type)
{
    std::vector<LDElement*> els = getElement(col,row);//,false);
    for(LDElement *el:els) {
        if(el!=nullptr) {
            if(type==el->getType() || type=="") {
                auto it = std::find(elements.begin(),elements.end(),el);
                if(it!=elements.end()) elements.erase(it);
                QGraphicsWidget *item = el->getItem();
                removeItem(item->graphicsItem());
                update(sceneRect());
                if((stack)&&(stack->isWorking())) stack->removeElement(el);
                delete el;
            }
        }
    }
}

void LDScene::removeElements(std::vector<LDElement *> &elementList)
{
    std::vector<LDElement*> delElements;
    for(LDElement *el:elementList) {
        auto it = std::find(elements.begin(),elements.end(),el);
        if(it!=elements.end()) {
            delElements.push_back(el->clone());
            elements.erase(it);
            QGraphicsWidget *item = el->getItem();
            removeItem(item->graphicsItem());
            delete el;
        }
    }
    update(sceneRect());
    if((stack)&&(stack->isWorking())) stack->removeElements(delElements);
}

void LDScene::deselectAll()
{
    for(LDElement *el:elements) {
        if(el->isSelected()) {
            el->setSelected(false);
            el->getItem()->graphicsItem()->update();
        }
    }
    selectedElements.clear();
}

void LDScene::setLineInsertMode(bool value)
{
    setInsertElement(nullptr);
    lMode.clear();
    lMode.on_flag = value;
}

void LDScene::createConnection(int col1, int row1, int col2, int row2,bool hor_mode)
{
    bool startFlag =false;
    if(stack && !stack->isGroupMode()) {startFlag=true;stack->startGroupCmd();}
    if(hor_mode) {
        createHorLine(row1,col1,col2);
        createVerLine(col2,row1,row2);
    }else {
        createVerLine(col1,row1,row2);
        createHorLine(row2,col1,col2);
    }
    if(stack && startFlag) stack->stopGroupCmd();
}

void LDScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    cur_x = event->scenePos().x();
    cur_y = event->scenePos().y();
    if(!lMode.on_flag) {
        lMode.clear();


        if ((event->buttons() & Qt::LeftButton)&&(element==nullptr)) {
            setSelectionArea(event->scenePos().x(),event->scenePos().y(),last_x,last_y);
        }else {
            LDElement *ptr = getElementByPos(event->scenePos().x(),event->scenePos().y());
            //if(cursorElement!=ptr)
            {
                if(ptr) {
                    LDInfo ldInfo(*ptr);
                    ldInfo.col = qFloor(event->scenePos().x()/page.cell_width);
                    ldInfo.row = qFloor(event->scenePos().y()/page.cell_height);
                    emit updateName(ldInfo);
                }
                else {
                    LDInfo ldInfo;
                    ldInfo.col = qFloor(event->scenePos().x()/page.cell_width);
                    ldInfo.row = qFloor(event->scenePos().y()/page.cell_height);
                    if(ldInfo.col<1) ldInfo.col = 1;
                    if(ldInfo.col>page.column_count) ldInfo.col = page.column_count;
                    if(ldInfo.row<1) ldInfo.row=1;
                    if(ldInfo.row>page.row_count) ldInfo.row = page.row_count;
                    emit updateName(ldInfo);
                }
            }
            cursorElement = ptr;
            selectionItem->setRect(0,0,0,0);
            drawInsertElement(event->scenePos().x(),event->scenePos().y());
            drawCopyBuf(event->scenePos().x(),event->scenePos().y());
            QGraphicsScene::mouseMoveEvent(event);
        }

    }else {
        drawConnectionLine(event->scenePos().x(),event->scenePos().y(),event->buttons(),event->modifiers());
    }
}

void LDScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    last_x = event->scenePos().x();
    last_y = event->scenePos().y();

    int col = qFloor(last_x/page.cell_width);
    int row = qFloor(last_y/page.cell_height);

    if(!lMode.on_flag) {

        if (event->buttons() & Qt::LeftButton) {
            if(!(event->modifiers() & Qt::ControlModifier)) deselectAll();
            toggleSelect(last_x,last_y);

            if(element!=nullptr) {

                while(col>page.column_count+1-element->getColSpan()) col--;
                while(row>page.row_count+1-element->getRowSpan()) row--;

                if(stack) stack->startGroupCmd();
                int rightTopElCol = col + element->getColSpan() - 1;
                int rightTopElRow = row - element->getRowSpan() + 1;
                getFreePlace(col,row,rightTopElCol,rightTopElRow);
                LDElement *el = element->clone();
                addElement(el,col,row);
                if(el->getType()=="comment") {
                    setInsertElement(nullptr);
                    copyItem->setVisible(false);
                    mouseDoubleClickEvent(event);
                }
                emit sceneChanged();
                if(stack) stack->stopGroupCmd();
            }else if(copyItem->isVisible()) {
                if(!stack->isGroupMode()) stack->startGroupCmd();
                std::vector<LDElement*> copyEls = copyItem->getElements();

                for(LDElement *el:copyEls) {
                    int col = copyItem->getColumn()+el->getColNum();
                    int row = copyItem->getRow()+el->getRowNum();
                    int rightTopElCol = col + el->getColSpan() - 1;
                    int rightTopElRow = row - el->getRowSpan() + 1;
                    getFreePlace(col,row,rightTopElCol,rightTopElRow);
                    el->setColNum(col);
                    el->setRowNum(row);
                }
                addElements(copyEls);
                stack->stopGroupCmd();
                copyItem->setVisible(false);
                emit sceneChanged();
            }
        }else if(event->buttons() & Qt::RightButton) {
            if(element!=nullptr) {
                setInsertElement(nullptr);
            }else if(!copyItem->isVisible()) {
                auto *menu= new QMenu();
                LDElement *el = getElementByPos(last_x,last_y);
                if(el) {
                    if(selectedElements.size()<=1) menu->addAction(QIcon(":/images/config.png"),"свойства",[el](){el->updateProperties();});
                    menu->addAction(QIcon(":/images/disable.ico"),"закомментировать",[el,this](){
                        if(selectedElements.empty()) {
                            el->setEnabled(false);
                            el->getItem()->update();
                        }else {
                            for(LDElement *selEl:selectedElements) {
                                selEl->setEnabled(false);
                                selEl->getItem()->update();
                            }
                        }
                    });
                    menu->addAction(QIcon(":/images/enable.ico"),"раскомментировать",[el,this](){
                        if(selectedElements.empty()) {
                            el->setEnabled(true);
                            el->getItem()->update();
                        }else {
                            for(LDElement *selEl:selectedElements) {
                                selEl->setEnabled(true);
                                selEl->getItem()->update();
                            }
                        }
                    });
                    if(el->isSelected()) {

                        menu->addAction(QIcon(":/images/copy.png"),"копировать",[this](){
                            copyItem->setElements(selectedElements);
                            deselectAll();
                        });
                        menu->addAction(QIcon(":/images/cut.png"),"вырезать",[this](){
                            copyItem->setElements(selectedElements);
                            stack->startGroupCmd();
                            removeElements(selectedElements);
                            selectedElements.clear();
                            stack->stopGroupCmd();
                            emit sceneChanged();
                        });
                        menu->addAction(QIcon(":/images/move.png"),"переместить",[this](){
                            copyItem->setElements(selectedElements);
                            stack->startGroupCmd();
                            removeElements(selectedElements);
                            setInsertElement(nullptr);
                            copyItem->setVisible(true);
                            drawCopyBuf(last_x,last_y);
                            //stack->stopGroupCmd();
                            selectedElements.clear();
                            deselectAll();
                            emit sceneChanged();
                        });
                    }
                }
                if(copyItem->count()) {
                    menu->addAction(QIcon(":/images/paste.png"),"вставить",[this,xx=event->scenePos().x(),yy=event->scenePos().y()](){
                        deselectAll();
                        setInsertElement(nullptr);
                        copyItem->setVisible(true);
                        drawCopyBuf(xx,yy);
                        emit sceneChanged();
                    });
                }
                menu->addAction(QIcon(":/images/go_top.png"),"Добавить страницу перед",[this](){emit createPageBefore();});
                menu->addAction(QIcon(":/images/go_bottom.png"),"Добавить страницу после",[this](){emit createPageAfter();});
                menu->addAction(QIcon(":/images/remove.png"),"Удалить страницу",[this](){emit deletePage(true);});
                menu->addAction(QIcon(":/images/clear.png"),"Очистить страницу",this,&LDScene::empty_page);

                if(menu->children().count()) menu->exec(event->screenPos());
                delete menu;
            }else {
                copyItem->setVisible(false);
            }
        }
    }else {
        drawConnectionLine(event->scenePos().x(),event->scenePos().y(),event->buttons(),event->modifiers());
    }
    //QGraphicsScene::mousePressEvent(event);
}

void LDScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    last_x = event->scenePos().x();
    last_y = event->scenePos().y();
    if(lMode.on_flag) stack->stopGroupCmd();
    //if ((event->buttons() & Qt::LeftButton)&&(stack && stack->isGroupMode()))
    //{stack->stopGroupCmd();}
    selectAreaElements();
    //QGraphicsScene::mouseReleaseEvent(event);
}

void LDScene::keyPressEvent(QKeyEvent *event)
{
    // обработка перемещений выделенных объектов
    // копирование, вырезание удаление выделенных объектов

    switch(event->key()) {
        case Qt::Key_C:
            if (QApplication::keyboardModifiers() && Qt::ControlModifier) {
                copyItem->setElements(selectedElements);
                deselectAll();
            }
            break;
        case Qt::Key_X:
            if (QApplication::keyboardModifiers() && Qt::ControlModifier) {
                copyItem->setElements(selectedElements);
                stack->startGroupCmd();
                removeElements(selectedElements);
                selectedElements.clear();
                stack->stopGroupCmd();
                emit sceneChanged();
            }
            break;
        case Qt::Key_V:
            if (QApplication::keyboardModifiers() && Qt::ControlModifier) {
                if(copyItem->count()) {
                    deselectAll();
                    setInsertElement(nullptr);
                    copyItem->setVisible(true);
                    drawCopyBuf(cur_x,cur_y);
                    emit sceneChanged();
                }
            }
            break;
        case Qt::Key_Delete:
            if(!selectedElements.empty()) {
                stack->startGroupCmd();
                removeElements(selectedElements);
                selectedElements.clear();
                stack->stopGroupCmd();
                emit sceneChanged();
            }
            break;
        case Qt::Key_Escape:
            setInsertElement(nullptr);
            copyItem->setVisible(false);
            break;
    }

    QGraphicsScene::keyPressEvent(event);
}

void LDScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    //update(sceneRect());
}

void LDScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    LDElement *el = getElementByPos(last_x,last_y);
    if(el) {
        el->updateProperties();
        emit sceneChanged();
    }else {
        QGraphicsItem *item = itemAt(event->scenePos(),QTransform());
        if(item) {
            auto *textItem = dynamic_cast<QGraphicsTextItem*>(item);
            if(textItem) {
                DialogAddEditComment *dialog = new DialogAddEditComment(textItem->toPlainText());
                if(dialog->exec()==QDialog::Accepted) {
                    if(!dialog->getComment().isEmpty()) {
                        textItem->setHtml("<i style=\"color:green;\">" + dialog->getComment() + "</i>");
                    }else {
                        removeItem(textItem);
                    }
                    emit sceneChanged();
                }
                delete dialog;
            }
        }
    }
}
