#ifndef CMDSTACK_H
#define CMDSTACK_H

#include "ldelement.h"

class LDScene;

class CmdStack
{
    struct Operation{
        enum class OpType {ADD_ELEMENT,REMOVE_ELEMENT};
        std::vector<int> intParams;
        std::vector<double> doubleParams;
        std::vector<LDElement*> elements;
        OpType type;
        void removeElements() {
            for(LDElement *el:elements) {delete el;}
            elements.clear();
        }
    };
    LDScene *scene;
    using StackPoint = std::pair<std::vector<Operation>,std::vector<Operation>>;
    std::size_t cur_pos=0;
    std::size_t group_pos = 0;
    std::vector<StackPoint> stack;
    StackPoint groupPoint;
    bool groupflag = false;
    void addPoint(StackPoint &point);
    void doOperations(std::vector<Operation> &ops);
    bool work = true;
public:

    explicit CmdStack(LDScene *scene);
    std::size_t getLength() const {return stack.size();}
    std::size_t getPos() const {return cur_pos;}
    bool isGroupMode() const {return groupflag;}
    void startGroupCmd();
    void stopGroupCmd();
    void addElement(LDElement *el);
    void removeElement(LDElement *el);
    void removeElements(std::vector<LDElement*> items);
    bool undo();
    bool redo();
    void setWork(bool value) {work=value;}
    bool isWorking() const {return work;}
    void clear();
};

#endif // CMDSTACK_H
