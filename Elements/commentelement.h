#ifndef COMMENTELEMENT_H
#define COMMENTELEMENT_H

#include "ldelement.h"

class CommentElement: public LDElement
{
    int cell_count;
    QString prComment="комментарий";
public:
    CommentElement(qreal cell_width, qreal cell_height, int cell_count=1);
    QString getProjectComment() const {return prComment;}
    void setProjectComment(const QString &comment) {prComment=comment;}
    // LDElement interface
public:
    LDElement *clone();
    void updateProperties();
};

#endif // COMMENTELEMENT_H
