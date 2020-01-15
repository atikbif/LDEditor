#include "finder.h"
#include "plcvarcontainer.h"
#include <QDebug>
#include <algorithm>

bool Finder::searchText(QString &searchWord, QString &str, bool fullWord, bool registerFlag)
{
    if(searchWord.isEmpty()) return false;
    if(!registerFlag) {
        searchWord = searchWord.toLower();
        str = str.toLower();
    }
    if(fullWord) {
        if(str.contains(QRegExp("\\b"+searchWord + "\\b"))) return true;
    }else if(str.contains(searchWord)) return true;
    return false;
}

Finder::Finder(std::vector<LDScene *> scenes, QObject *parent):QObject (parent),scenes(scenes)
{

}

void Finder::search(const SearchConfig &conf, int &pageNum, int &col, int &row)
{
    auto pageCount = static_cast<int>(scenes.size());
    std::vector<QString> findResult;
    if(conf.fullDocument) {
        pageNum=1;
        for(LDScene *scene:scenes) {
            scene->setLastRow(0);
            scene->setLastCol(0);
            std::vector<LDElement*> elements = scene->getAllelements();
            for(LDElement *el:elements) {
                el->setSelected(false);
                QString param;
                QString searchWord = conf.searchWord;
                if(conf.searchNames) {
                    param = el->getName();
                    if(searchText(searchWord,param,conf.fullWord,conf.registerFlag)) {
                        el->setSelected(true);
                        el->getItem()->update();
                        QString elString = "страница:"+ QString::number(pageNum)+" строка:"+QString::number(el->getRowNum()) + " колонка:" + QString::number(el->getColNum()) + " " + el->getName();
                        findResult.push_back(elString);
                    }
                }
                if(!el->isSelected() && conf.searchComments) {
                    param = el->getComment();
                    if(searchText(searchWord,param,conf.fullWord,conf.registerFlag)) {
                        el->setSelected(true);
                        el->getItem()->update();
                        QString elString = "страница:"+ QString::number(pageNum)+" строка:"+QString::number(el->getRowNum()) + " колонка:" + QString::number(el->getColNum()) + " " + el->getName();
                        findResult.push_back(elString);
                    }
                }
                if(!el->isSelected() && conf.searchConnections) {
                    param = el->connectedVar.name;
                    if(searchText(searchWord,param,conf.fullWord,conf.registerFlag)) {
                        el->setSelected(true);
                        el->getItem()->update();
                        QString elString = "страница:"+ QString::number(pageNum)+" строка:"+QString::number(el->getRowNum()) + " колонка:" + QString::number(el->getColNum()) + " " + el->getName();
                        findResult.push_back(elString);
                    }
                }
            }
            scene->update();pageNum++;
        }
        emit findInfo(findResult);
    }else if(pageNum<pageCount) {

        LDScene *scene = scenes.at(pageNum);
        int columnCount = scene->getColumnCount();
        std::vector<LDElement*> inpElements = scene->getAllelements();
        std::vector<LDElement*> elements;
        for(LDElement *el:inpElements) {
            if(el->isSelected()) {
                el->setSelected(false);
                el->getItem()->update();
            }
            if(el->getInputCount()) elements.push_back(el);
        }
        std::sort(elements.begin(),elements.end(),[](LDElement *leftEl, LDElement *rightEl){return leftEl->getColNum()<rightEl->getColNum();});
        std::stable_sort(elements.begin(),elements.end(),[](LDElement *leftEl, LDElement *rightEl){return leftEl->getRowNum()<rightEl->getRowNum();});
        std::vector<LDElement*> revElements = elements;
        if(col<columnCount) {col++;}
        else if(row<scene->getRowCount()) {row++;col=1;}
        else {row=1;col=1;}
        auto it = std::find_if(elements.begin(),elements.end(),[columnCount,conf,col,row](LDElement *el){

            int pos = (el->getRowNum()-1)*columnCount + el->getColNum();
            int basePos = (row-1)*columnCount + col;
            if(pos>=basePos) {
                QString param;
                QString searchWord = conf.searchWord;
                if(conf.searchNames) {
                    param = el->getName();
                    if(searchText(searchWord,param,conf.fullWord,conf.registerFlag)) {return true;}
                }
                if(!el->isSelected() && conf.searchComments) {
                    param = el->getComment();
                    if(searchText(searchWord,param,conf.fullWord,conf.registerFlag)) {return true;}
                }
                if(!el->isSelected() && conf.searchConnections) {
                    param = el->connectedVar.name;
                    if(searchText(searchWord,param,conf.fullWord,conf.registerFlag)) {return true;}
                }
            }
            return false;
        });
        if(it!=elements.end()) {
            row = (*it)->getRowNum();
            col = (*it)->getColNum();
            scene->setLastRow(row);
            scene->setLastCol(col);

            (*it)->setSelected(true);
            (*it)->getItem()->update();
            QString elString = "страница:"+ QString::number(pageNum)+" строка:"+QString::number((*it)->getRowNum()) + " колонка:" + QString::number((*it)->getColNum()) + " " + (*it)->getName();
            findResult.push_back(elString);
        }else {
            scene->setLastRow(1);
            scene->setLastCol(1);
            col=1;
            row=1;
        }
        emit findInfo(findResult);
    }


}

void Finder::replace(const QString &varName, const QString &newVarName)
{
    int pNum = 1;
    std::vector<QString> replResult;
    if(varName.isEmpty() || newVarName.isEmpty()) {
        replResult.push_back("Некорректные имена для замены");
        emit findInfo(replResult);
        return;
    }
    auto var = PLCVarContainer::getInstance().getVarByName(newVarName);
    if(var) {
        for(LDScene *scene:scenes) {
            std::vector<LDElement*> elements = scene->getAllelements();
            for(LDElement *el:elements) {
                if(el->getInputCount()) {
                    if(el->connectedVar.name==varName) {
                        el->connectedVar.name = newVarName;
                        el->getItem()->update();
                        QString elString = "Замена привязки - страница:"+ QString::number(pNum)+" строка:"+QString::number(el->getRowNum()) + " колонка:" + QString::number(el->getColNum())+" " + el->getName();
                        replResult.push_back(elString);
                    }
                }
            }
            pNum++;
        }
        if(replResult.empty()) replResult.push_back("Замена привязки. Элементы не найдены");

    }else {
        replResult.push_back("Переменная " + newVarName + " отсутствует в проекте. Замена невозможна");
    }
    emit findInfo(replResult);
}

void Finder::changeVar(const QString &oldGroup, const QString &oldVar, const QString &newGroup, const QString &newVar)
{
    for(LDScene *scene:scenes) {
        std::vector<LDElement*> elements = scene->getAllelements();
        for(LDElement *el:elements) {
            if(el->getInputCount()) {
                if(el->connectedVar.group==oldGroup && el->connectedVar.name==oldVar) {
                    el->connectedVar.group = newGroup;
                    el->connectedVar.name = newVar;
                    el->getItem()->update();
                }
            }
        }
    }
}

void Finder::changeGroup(const QString &oldGroup, const QString &newGroup)
{
    for(LDScene *scene:scenes) {
        std::vector<LDElement*> elements = scene->getAllelements();
        for(LDElement *el:elements) {
            if(el->getInputCount()) {
                if(el->connectedVar.group==oldGroup) {
                    el->connectedVar.group = newGroup;
                }
            }
        }
    }
}
