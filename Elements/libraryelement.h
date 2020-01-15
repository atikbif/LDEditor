#ifndef LIBRARYELEMENT_H
#define LIBRARYELEMENT_H

#include "ldelement.h"

class LibraryElement : public LDElement
{
    QString fName;
    std::vector<QString> inpNames;
    std::vector<QString> inpTypes;
    QString prototype;
    std::vector<QString> funcBody;
    QString resType;
    QString funcName;
    int version;
public:
    LibraryElement(qreal cell_width, qreal cell_height, const QString &fName);
    QString getInputName(int num) const;
    QString getFileName() const {return fName;}
    std::vector<QString> getInputNames() const {return inpNames;}
    std::vector<QString> getinputTypes() const {return inpTypes;}
    QString getPrototype() const {return prototype;}
    QString getFunctionBody() const {
        QString result;
        for(const QString &s:funcBody) result+=s+"\n";
        return result;
    }
    QString getResultType() const {return resType;}
    int getVersion() const {return version;}
    QString getFuncName() const {return funcName;}
    // LDElement interface
public:
    LDElement *clone();
};

#endif // LIBRARYELEMENT_H
