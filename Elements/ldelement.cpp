#include "ldelement.h"
#include "dialogldelementproperties.h"
#include <QDebug>
//#include "serialize.h"

const std::map<QString,QString> LDInfo::types = {{"closed contact","норм. замкн. контакт"},
                                                 {"const variable","константа"},
                                                 {"divide","деление"},
                                                 {"equal","равенство"},
                                                 {"greather than","больше"},
                                                 {"greather or equal","больше или равно"},
                                                 {"horizontal line","гориз. линия"},
                                                 {"vertical line","верт. линия"},
                                                 {"less than","меньше"},
                                                 {"less or equal","меньше или равно"},
                                                 {"minus","вычитание"},
                                                 {"multiple two inputs","умножение"},
                                                 {"not equal","не равно"},
                                                 {"open contact","норм. разомкн. контакт"},
                                                 {"plus two inputs","сложение"},
                                                 {"relay","реле"},
                                                 {"variable","переменная"},
                                                 {"delay on","задержка по пер. фронту"},
                                                 {"delay off","задержка по задн. фронту"},
                                                 {"rs trigger","RS триггер"},
                                                 {"sr trigger","SR триггер"},
                                                 {"comment","Комментарий"}
                                                };

LDElement::LDElement(qreal cell_width, qreal cell_height):cell_width(cell_width), cell_height(cell_height)
{

}

QGraphicsWidget *LDElement::getItem()
{
    return item;
}

void LDElement::updateProperties()
{
    auto *dialog = new DialogLDElementProperties(this);
    dialog->exec();
}

void LDElement::setShowNameFlag(bool value)
{
    showName = value;
    if(item) item->update();
}

LDElement::~LDElement(){delete item;}
