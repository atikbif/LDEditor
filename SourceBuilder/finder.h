#ifndef FINDER_H
#define FINDER_H

#include "dialogsearch.h"
#include <QObject>
#include "ldscene.h"

class Finder: public QObject
{
    std::vector<LDScene*> scenes;
    Q_OBJECT
    static bool searchText(QString &searchWord, QString &str, bool fullWord, bool registerFlag);
public:
    Finder(std::vector<LDScene*> scenes, QObject *parent = nullptr);
    void search(const SearchConfig &conf, int &pageNum, int &col, int &row);
    void replace(const QString &varName, const QString &newVarName);
    void changeVar(const QString &oldGroup, const QString &oldVar, const QString &newGroup, const QString &newVar);
    void changeGroup(const QString &oldGroup, const QString &newGroup);
signals:
    void findInfo(const std::vector<QString> &findResult);
};

#endif // FINDER_H
