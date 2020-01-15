#include "ldchecker.h"
#include "plcvarcontainer.h"

std::vector<QString> LDChecker::checkVarConnectedElements(std::vector<LDElement *> &els)
{
    std::vector<QString> res;
    for(LDElement *el:els) {
        if(el->isNeedConnect()) {
            if(el->connectedVar.name.isEmpty()) {
                res.push_back("строка:" + QString::number(el->getRowNum()) + " столбец:" + QString::number(el->getColNum()) +  " <b><font color=\"red\">элемент не подключен к переменной</font></b>");
                el->setSelected(true);
                el->getItem()->update();
            }else {
                std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(el->connectedVar.group,el->connectedVar.name);
                if(!v) {
                    res.push_back("строка:" + QString::number(el->getRowNum()) + " столбец:" + QString::number(el->getColNum()) +  " <b><font color=\"red\">элемент подключен к несуществующей переменной</font></b>");
                    el->setSelected(true);
                    el->getItem()->update();
                }
            }
        }
    }
    return res;
}
