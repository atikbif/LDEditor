#ifndef CONNECTIONFINDER_H
#define CONNECTIONFINDER_H

#include "ldscene.h"
#include <set>
#include <utility>

struct CircuitElement {
    std::vector<int> inputCircuits;
    int outCircuit = 0;
    LDElement *ptr = nullptr;
    bool connected = false;
};

struct PageCode{
    QStringList progBody;
    QStringList varBody;
    QStringList functionsBody;
    QStringList errBody;
    std::set<QString> libHeader;
    std::set<QString> libBody;
    std::vector<LDElement*> notUsedElements;
    std::vector<LDElement*> shortCircuitElements;
};

class ConnectionFinder
{
    LDScene *sc;
    std::vector<std::vector<int>> points;
    std::vector<CircuitElement> els;
    std::set<int> connectedCircuits;
    std::set<int> getOutCircuitsForInputCircuit(int inpCurcuitNum);
    void findConnectedElements(int value);
    bool areElementsLinked(CircuitElement &el1, CircuitElement &el2);
    bool areElementsIOLinked(CircuitElement &el1, CircuitElement &el2);
    std::vector<CircuitElement> getCircuitElements(std::vector<CircuitElement> &inpElements, CircuitElement el);
    int page_num;
public:
    explicit ConnectionFinder(LDScene *scene,int page_num=1);
    PageCode scanCircuits();

};

#endif // CONNECTIONFINDER_H
