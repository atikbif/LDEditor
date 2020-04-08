#include "connectionfinder.h"
#include "vartypespecifier.h"
#include "Elements/libraryelement.h"

ConnectionFinder::ConnectionFinder(LDScene *scene, int page_num):sc(scene),page_num(page_num)
{
    int rowCount = sc->getRowCount();
    int colCount = sc->getColumnCount();
    for(int i=0;i<rowCount;i++) {
        std::vector<int> row(static_cast<std::size_t>(colCount+1));
        row[0]=1;
        points.push_back(row);
    }
}

PageCode ConnectionFinder::scanCircuits()
{
    PageCode page;
    // формирование карты соединений для линий
    int circuit_num=2;
    std::vector<LDElement*> elements = sc->getAllelements();
    int rowCount = sc->getRowCount();
    int colCount = sc->getColumnCount();
    for(LDElement *el:elements) {
        if(el->isEnabled())
        {
            if(el->getType()=="horizontal line") {
                std::size_t col = static_cast<std::size_t>(el->getColNum());
                std::size_t row = static_cast<std::size_t>(el->getRowNum());
                int leftValue = points[row-1][col-1];
                int rightValue = points[row-1][col];

                for(std::size_t c=0;c<static_cast<std::size_t>(colCount+1);c++) {
                    for(std::size_t r=0;r<static_cast<std::size_t>(rowCount);r++) {
                        if(leftValue && points[r][c]==leftValue) points[r][c] = circuit_num;
                        else if(rightValue && points[r][c]==rightValue) points[r][c] = circuit_num;
                    }
                }
                points[row-1][col-1] = circuit_num;
                points[row-1][col] = circuit_num;
                circuit_num++;
            }else if(el->getType()=="vertical line") {
                std::size_t col = static_cast<std::size_t>(el->getColNum());
                std::size_t row = static_cast<std::size_t>(el->getRowNum());
                int topValue = points[row-2][col];
                int bottomValue = points[row-1][col];
                for(std::size_t c=0;c<static_cast<std::size_t>(colCount+1);c++) {
                    for(std::size_t r=0;r<static_cast<std::size_t>(rowCount);r++) {
                        if(topValue && points[r][c]==topValue) points[r][c] = circuit_num;
                        else if(bottomValue && points[r][c]==bottomValue) points[r][c] = circuit_num;
                    }
                }
                points[row-2][col] = circuit_num;
                points[row-1][col] = circuit_num;
                circuit_num++;
            }
        }
    }

    // формирование карты соединений для контактов элементов
    for(LDElement *el:elements) {
        int inputCnt = el->getInputCount();
        if(inputCnt && el->isEnabled()) {
            CircuitElement circEl;
            circEl.ptr = el;
            int inpOffset = -inputCnt+1;//el->getRowSpan() - inputCnt;
            int outOffset = 0;//el->getRowSpan() - 1;
            int col = el->getColNum();
            int row = el->getRowNum();
            for(int i=0;i<inputCnt;i++) {
                if(points[static_cast<std::size_t>(row-1+inpOffset+i)][static_cast<std::size_t>(col-1)]==0) {
                    points[static_cast<std::size_t>(row-1+inpOffset+i)][static_cast<std::size_t>(col-1)]=circuit_num;
                    circEl.inputCircuits.push_back(circuit_num);
                    circuit_num++;
                }else {
                    circEl.inputCircuits.push_back(points[static_cast<std::size_t>(row-1+inpOffset+i)][static_cast<std::size_t>(col-1)]);
                }
            }
            if(points[static_cast<std::size_t>(row-1+outOffset)][static_cast<std::size_t>(col)]==0) {
                points[static_cast<std::size_t>(row-1+outOffset)][static_cast<std::size_t>(col)]=circuit_num;
                circEl.outCircuit = circuit_num;
                circuit_num++;
            }else {
                circEl.outCircuit = points[static_cast<std::size_t>(row-1+outOffset)][static_cast<std::size_t>(col)];
            }
            els.push_back(circEl);
        }
    }

    // поиск элементов подключенных к входной цепи
    connectedCircuits.clear();
    findConnectedElements(points[0][0]);

    for(auto& el:els) {
        bool connected = true;
        for(int inp:el.inputCircuits) {
            if(std::find(connectedCircuits.begin(),connectedCircuits.end(),inp)==connectedCircuits.end()) connected = false;
        }
        el.connected = connected;
    }
    //qDebug() << "CONNECTED CIRCUITS:" << std::vector(connectedCircuits.begin(),connectedCircuits.end());

    // sort connected elements by position from left to right
    std::vector<CircuitElement> sortedConnectedElements;
    for(auto &el:els) {
        if(el.connected) sortedConnectedElements.push_back(el);
    }
    std::sort(sortedConnectedElements.begin(),sortedConnectedElements.end(),[](const CircuitElement &el1, const CircuitElement &el2){return  el1.ptr->getColNum()<el2.ptr->getColNum();});

    // Список номеров соединенй, замыкающих электрические цепи
    std::set<int> endCircuits;
    for(std::vector<int> &nums:points) {
        if(!nums.empty()) endCircuits.insert(nums.back());
    }
    endCircuits.erase(0);
    //qDebug() << "Замыкающие цепи:" << std::vector(endCircuits.begin(),endCircuits.end());

    std::set<int> vars;
    std::vector<std::vector<CircuitElement>> circuits;
    while(!sortedConnectedElements.empty()) {

        // связанная цепь элементов подключенных к началу
        std::vector<CircuitElement> link = getCircuitElements(sortedConnectedElements,sortedConnectedElements[0]);

        std::sort(link.begin(),link.end(),[](const CircuitElement &el1, const CircuitElement &el2){return  el1.ptr->getColNum()<el2.ptr->getColNum();});
        //std::stable_sort(link.begin(),link.end(),[](const CircuitElement &el1, const CircuitElement &el2){return  el1.ptr->getRowNum()<el2.ptr->getRowNum();});
        // удаление дубликатов
        link.erase(std::unique(link.begin(), link.end(),[](const CircuitElement &el1, const CircuitElement &el2){
            return ((el1.ptr->getColNum()==el2.ptr->getColNum()) &&(el1.ptr->getRowNum()==el2.ptr->getRowNum()));
        }), link.end());

        // отсеивание элементов не подключенных к концу
        for(CircuitElement &el:link) {el.connected=false;}
        std::vector<CircuitElement> connectedLink;
        while(!link.empty()) {
            CircuitElement el = link.back();
            link.pop_back();
            bool connectFlag = false;
            if(std::find(endCircuits.begin(),endCircuits.end(),el.outCircuit)!=endCircuits.end()) connectFlag = true;
            if(connectFlag==false) {
                for(CircuitElement &element:connectedLink) {
                    if(areElementsIOLinked(el,element)) {
                        connectFlag = true;
                        break;
                    }
                }
            }
            if(connectFlag) connectedLink.push_back(el);
        }
        std::sort(connectedLink.begin(),connectedLink.end(),[](const CircuitElement &el1, const CircuitElement &el2){return  el1.ptr->getRowNum()<el2.ptr->getRowNum();});
        std::stable_sort(connectedLink.begin(),connectedLink.end(),[](const CircuitElement &el1, const CircuitElement &el2){return  el1.ptr->getColNum()<el2.ptr->getColNum();});
        connectedLink.erase(std::unique(connectedLink.begin(), connectedLink.end(),[](const CircuitElement &el1, const CircuitElement &el2){
            return ((el1.ptr->getColNum()==el2.ptr->getColNum()) &&(el1.ptr->getRowNum()==el2.ptr->getRowNum()));
        }), connectedLink.end());

        if(!connectedLink.empty()) {
            for(auto &el:connectedLink) {
                vars.insert(el.inputCircuits.begin(),el.inputCircuits.end());
                vars.insert(el.outCircuit);
            }

            circuits.push_back(connectedLink);
        }
    }

    std::map<int,QString> vTypes;
    for(int v:vars) vTypes[v]="unsigned short";

    std::vector<QString> intTimers;
    int tmrNum=0;
    std::vector<QString> counterVars;
    int counterNum=0;

    std::sort(circuits.begin(),circuits.end(),[](const std::vector<CircuitElement> &c1, const std::vector<CircuitElement> &c2){return  c1[0].ptr->getRowNum()<c2[0].ptr->getRowNum();});


    QStringList progBody;
    QStringList varBody;
    for(auto &circ:circuits) {
        auto it = circ.begin();
        while(it!=circ.end()) {
            CircuitElement el = *it;

            // проверка на объединение выходов - или

            bool orFlag = false;
            auto beginIt = circ.begin();
            if(it!=beginIt) {
                auto curIt = it-1;
                while(true) {
                    CircuitElement element = *curIt;
                    if(element.outCircuit==el.outCircuit) {
                        orFlag = true;
                        break;
                    }
                    if(curIt==beginIt) break;
                    curIt--;
                }
            }

            if(el.ptr->getType().contains("library:")) {
                auto libEl = dynamic_cast<LibraryElement*>(el.ptr);
                if(libEl) {
                    vTypes[el.outCircuit] = libEl->getResultType();
                    QString res = "p"+QString::number(page_num)+"_v"+QString::number(el.outCircuit)+"=";
                    if(orFlag) res+="p"+QString::number(page_num)+"_v"+QString::number(el.outCircuit) + " | ";
                    QString funcName = libEl->getFuncName() + "(";
                    res+=funcName;
                    for(int i=0;i<static_cast<int>(el.inputCircuits.size());i++) {
                        res+="p"+QString::number(page_num)+"_v" + QString::number(el.inputCircuits[static_cast<std::size_t>(i)]);
                        if(i!=static_cast<int>(el.inputCircuits.size())-1) res+=",";
                    }
                    res+=");";
                    progBody.push_back(res);
                    page.libHeader.insert(libEl->getPrototype());
                    page.libBody.insert(libEl->getFunctionBody());
                }
            }else {

                // определение типа выхода
                std::vector<QString> inpTypes;
                for(int inpC:el.inputCircuits) inpTypes.push_back(vTypes[inpC]);
                QString outType = VarTypeSpecifier::getOuType(*(el.ptr),inpTypes);
                vTypes[el.outCircuit] = outType;



                QString res = "p"+QString::number(page_num)+"_v"+QString::number(el.outCircuit)+"=";
                if(orFlag) res+="p"+QString::number(page_num)+"_v"+QString::number(el.outCircuit) + " | ";
                QString funcName = VarTypeSpecifier::getFuncName(*(el.ptr),inpTypes);
                if(funcName.contains("sr_trig") || funcName.contains("rs_trig")) {
                    funcName+="p"+QString::number(page_num)+"_v"+QString::number(el.outCircuit)+",";
                }else if(funcName.contains("delay_on") || funcName.contains("delay_off")) {
                    tmrNum++;
                    QString tmrVar = "p"+QString::number(page_num)+"_ms_tmr"+QString::number(tmrNum);
                    QString prevIP = "p"+QString::number(page_num)+"_filter_on"+QString::number(tmrNum);
                    intTimers.push_back(tmrVar);
                    varBody.push_back("unsigned short " + prevIP + "=0;");
                    funcName+="&"+tmrVar+", " + "&"+prevIP+", ";

                }else if(funcName.contains("counter")) {
                    counterNum++;
                    QString cVar = "struct counter_state p"+QString::number(page_num)+"_c_st" + QString::number(counterNum) + "={0,0,0};";
                    counterVars.push_back(cVar);
                    funcName+="&p"+QString::number(page_num)+"_c_st" + QString::number(counterNum) + ",";
                }
                res+=funcName;
                for(int i=0;i<static_cast<int>(el.inputCircuits.size());i++) {
                    res+="p"+QString::number(page_num)+"_v" + QString::number(el.inputCircuits[static_cast<std::size_t>(i)]);
                    if(i!=static_cast<int>(el.inputCircuits.size())-1) res+=",";
                }
                res+=");";
                progBody.push_back(res);
            }
            it++;
        }
    }

    for(int v:vars) {
        int startCircuitNum = points[0][0];
        QString vStr = vTypes[v];
        if(vStr=="bool") vStr="unsigned char";
        vStr += " p"+QString::number(page_num)+"_v" + QString::number(v) + "=" + ((v==startCircuitNum)?"1;":"0;");
        varBody.push_back(vStr);
    }

    for(const QString &s:counterVars) varBody.push_back(s);


    for(const QString &t:intTimers) {
        varBody.push_back("unsigned short " + t + "=0;");
        QString frontVar = t;
        frontVar.replace(QString("_ms_tmr"),QString("_filter_on"));
        page.functionsBody.push_back("if(" + frontVar + ") " + t + "++;");
    }

    std::vector<LDElement*> usedElements;

    for(auto &circ:circuits) {
        for(auto el:circ) {
            usedElements.push_back(el.ptr);
            if(el.outCircuit==points[0][0]) {
                page.shortCircuitElements.push_back(el.ptr);
            }
        }
    }

    for(LDElement* el:elements) {
        if(el->getInputCount() && el->isEnabled()) {
            if(std::find(usedElements.begin(),usedElements.end(),el)==usedElements.end()) {
                page.notUsedElements.push_back(el);
            }
        }
    }



    page.varBody = varBody;
    page.progBody = progBody;




    /*qDebug() << "MAP";
    for(auto& row:points) {
        QString circString;
        for(int v:row) {
            QString num = QString::number(v);
            while(num.length()<2) num = '0' + num;
            num = " " + num;
            circString += num;
        }
        qDebug() << circString;
    }*/

    return page;
}

std::set<int> ConnectionFinder::getOutCircuitsForInputCircuit(int inpCurcuitNum)
{
    std::set<int> result;
    if(std::find(connectedCircuits.begin(),connectedCircuits.end(),inpCurcuitNum)!=connectedCircuits.end()) return result;
    connectedCircuits.insert(inpCurcuitNum);
    for(auto& el:els) {
        //if(!el.connected) {
            std::vector<int> inputs = el.inputCircuits;
            if(std::find(inputs.begin(),inputs.end(),inpCurcuitNum)!=inputs.end())
            {
                if(el.outCircuit) {
                    result.insert(el.outCircuit);
                }
            }
        //}
    }

    result.erase(inpCurcuitNum);
    //qDebug() << "INPUT" << inpCurcuitNum << "RESULT:" << std::vector(result.begin(),result.end());

    return result;
}

void ConnectionFinder::findConnectedElements(int value)
{
    std::set<int> outs = getOutCircuitsForInputCircuit(value);
    for(int out:outs) findConnectedElements(out);
}

bool ConnectionFinder::areElementsLinked(CircuitElement &el1, CircuitElement &el2)
{
    std::set<int> c1;
    std::set<int> c2;
    for(int c:el1.inputCircuits) {if(c!=points[0][0]) c1.insert(c);}
    if(el1.outCircuit!=points[0][0]) c1.insert(el1.outCircuit);
    for(int c:el2.inputCircuits) {if(c!=points[0][0]) c2.insert(c);}
    if(el2.outCircuit!=points[0][0]) c2.insert(el2.outCircuit);

    std::vector<int> v_intersection;

    std::set_intersection(c1.begin(),c1.end(),c2.begin(),c2.end(),std::back_inserter(v_intersection));
    if(v_intersection.size()) return true;
    return false;
}

bool ConnectionFinder::areElementsIOLinked(CircuitElement &el1, CircuitElement &el2)
{
    std::set<int> c1;
    for(int c:el1.inputCircuits) {if(c!=points[0][0]) c1.insert(c);}
    if(c1.find(el2.outCircuit)!=c1.end()) return true;

    c1.clear();
    for(int c:el2.inputCircuits) {if(c!=points[0][0]) c1.insert(c);}
    if(c1.find(el1.outCircuit)!=c1.end()) return true;

    return false;
}

std::vector<CircuitElement> ConnectionFinder::getCircuitElements(std::vector<CircuitElement> &inpElements, CircuitElement el)
{
    std::vector<CircuitElement> result;
    result.push_back(el);
    auto it = std::find_if(inpElements.begin(),inpElements.end(),[el](const CircuitElement &element){
        return ((element.ptr->getColNum()==el.ptr->getColNum())&&(element.ptr->getRowNum()==el.ptr->getRowNum()));});
    if(it==inpElements.end()) return result;
    inpElements.erase(it);
    std::vector<CircuitElement> linkedElements;
    for(CircuitElement &element:inpElements) {
        if(areElementsLinked(el,element)) linkedElements.push_back(element);
    }
    for(const CircuitElement &element:linkedElements) {
        std::vector<CircuitElement> elRes = getCircuitElements(inpElements,element);
        result.insert(result.end(),elRes.begin(),elRes.end());
    }
    return result;
}
