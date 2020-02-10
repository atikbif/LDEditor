#ifndef LDSCENE_H
#define LDSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsGridLayout>

#include "ldelement.h"
#include "cmdstack.h"

#include <QPainter>
#include <QDebug>

#include "copybufitem.h"

struct LDPage {
    int row_count = 69;
    int column_count = 15;
    qreal cell_width = 150;
    qreal cell_height = 50;
};

struct LineMode {
    enum class cmdState {FIRST_POINT,SECOND_POINT};
    cmdState state;
    int firsPointCol = 0;
    int firstPointRow = 0;
    int secondPointCol = 0;
    int secondPointRow = 0;
    bool on_flag = false;
    QGraphicsLineItem *lineItem=nullptr;
    QGraphicsEllipseItem *firstPointItem=nullptr;
    QGraphicsEllipseItem *secondPointItem=nullptr;
    void clear() {
        firsPointCol = 0;
        firstPointRow = 0;
        secondPointCol = 0;
        secondPointRow  =0;
        on_flag = false;
        state = cmdState::FIRST_POINT;
        if(lineItem) {
            lineItem->setVisible(false);
            lineItem->setLine(0,0,0,0);
        }
        if(firstPointItem) {
            firstPointItem->setVisible(false);
            firstPointItem->setRect(0,0,0,0);
        }
        if(secondPointItem) {
            secondPointItem->setVisible(false);
            secondPointItem->setRect(0,0,0,0);
        }
    }
};

class LDScene : public QGraphicsScene
{
    int selElRow = 0;
    int selElCol = 0;
    LDPage page;
    LineMode lMode;
    qreal cell_width,cell_height;
    int row_count = 69;
    int column_count = 15;
    qreal last_x=0,last_y=0;
    qreal cur_x=0, cur_y=0;
    int last_row=0, last_col=0;
    bool insertLine = false;
    Q_OBJECT
    QGraphicsRectItem *selectionItem;
    //CopyItem *copyItem = nullptr;

    std::vector<LDElement*> elements;
    std::vector<LDElement*> selectedElements;

    LDElement *element = nullptr;
    LDElement *cursorElement = nullptr;
    CmdStack *stack = nullptr;
    CopyBufItem *copyItem = nullptr;
    bool showNames = false;

    void createHorLine(int row, int col1, int col2);
    void createVerLine(int col, int row1, int row2);
    void setSelectionArea(qreal x1,qreal y1, qreal x2, qreal y2);
    void drawInsertElement(qreal x, qreal y);
    void drawCopyBuf(qreal x, qreal y);
    void drawConnectionLine(qreal x, qreal y, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
    void selectAreaElements();
    void getFreePlace(int colStart, int rowEnd, int colEnd, int rowStart);
    void toggleSelect(qreal x, qreal y);
    LDElement* getElementByPos(qreal x, qreal y);


public:
    int getRowCount() const {return page.row_count;}
    int getColumnCount() const {return page.column_count;}
    LDScene(LDPage& page, QObject *parent = nullptr);
    void empty_page();
    void setInsertElement(LDElement *element, bool delete_previous=true);
    bool addElement(LDElement *el, int col, int row);
    void addElements(std::vector<LDElement*> & elementList);
    std::vector<LDElement *> getElement(int col, int row, bool anyPoint=true);
    std::vector<LDElement*> getAllelements() {return  elements;}
    int getElementCount() const {return static_cast<int>(elements.size());}
    void removeElement(int col, int row, const QString &type="");
    void removeElements(std::vector<LDElement*> & elementList);
    void deselectAll();
    void setLineInsertMode(bool value=true);
    void setStack(CmdStack *stack) {this->stack = stack;}
    void setCopyBuf(CopyBufItem *copyBuf) {copyItem=copyBuf;copyItem->setVisible(false); addItem(copyItem);}
    void createConnection(int col1, int row1, int col2, int row2, bool hor_mode=true);
    void setNamesFlag(bool value);
    int getLastCol() const;
    int getLastRow() const;
    void setLastCol(int value) {selElCol = value;}
    void setLastRow(int value) {selElRow = value;}

signals:
    void updateName(LDInfo &ldInfo);
    void createPageBefore();
    void createPageAfter();
    void deletePage(bool confirmation);
    void sceneChanged();

    // QGraphicsScene interface
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);

    // QGraphicsScene interface
protected:
    void wheelEvent(QGraphicsSceneWheelEvent *event);

    // QGraphicsScene interface
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // LDSCENE_H
