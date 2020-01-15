#include "cmdstack.h"
#include "ldscene.h"

#include<QDebug>

CmdStack::CmdStack(LDScene *scene):scene(scene)
{

}

void CmdStack::startGroupCmd()
{
    group_pos = 0;
    groupflag = true;
    groupPoint = std::make_pair<std::vector<Operation>,std::vector<Operation>>({},{});
}

void CmdStack::stopGroupCmd()
{
    if(groupflag && group_pos) {
        groupflag = false;
        group_pos=0;
        addPoint(groupPoint);
        groupPoint.first.clear();
        groupPoint.second.clear();
    }else {
        groupflag = false;
        group_pos = 0;
    }
}

void CmdStack::addElement(LDElement *el)
{
    Operation op;
    op.type = Operation::OpType::ADD_ELEMENT;
    op.intParams.push_back(el->getColNum());
    op.intParams.push_back(el->getRowNum());
    op.elements.push_back(el->clone());
    Operation undoOp;
    undoOp.type = Operation::OpType::REMOVE_ELEMENT;
    undoOp.intParams.push_back(el->getColNum());
    undoOp.intParams.push_back(el->getRowNum());
    undoOp.elements.push_back(el->clone());
    StackPoint point = std::make_pair<std::vector<Operation>,std::vector<Operation>>({op},{undoOp});
    addPoint(point);
}

void CmdStack::removeElement(LDElement *el)
{
    Operation undoOp;
    undoOp.type = Operation::OpType::ADD_ELEMENT;
    undoOp.intParams.push_back(el->getColNum());
    undoOp.intParams.push_back(el->getRowNum());
    undoOp.elements.push_back(el->clone());
    Operation op;
    op.type = Operation::OpType::REMOVE_ELEMENT;
    op.intParams.push_back(el->getColNum());
    op.intParams.push_back(el->getRowNum());
    op.elements.push_back(el->clone());
    StackPoint point = std::make_pair<std::vector<Operation>,std::vector<Operation>>({op},{undoOp});
    addPoint(point);
}

void CmdStack::removeElements(std::vector<LDElement *> items)
{

    std::vector<Operation> ops;
    std::vector<Operation> undoOps;

    for(LDElement *el: items) {
        Operation undoOp,op;
        undoOp.type = Operation::OpType::ADD_ELEMENT;
        op.type = Operation::OpType::REMOVE_ELEMENT;
        undoOp.intParams.push_back(el->getColNum());
        undoOp.intParams.push_back(el->getRowNum());
        undoOp.elements.push_back(el->clone());
        op.intParams.push_back(el->getColNum());
        op.intParams.push_back(el->getRowNum());
        op.elements.push_back(el->clone());
        ops.push_back(op);
        undoOps.push_back(undoOp);
    }
    StackPoint point = std::make_pair(ops,undoOps);
    addPoint(point);
}

void CmdStack::addPoint(StackPoint &point)
{
    if(cur_pos!=stack.size()) {
        for(std::size_t i=cur_pos;i<stack.size();i++) {
            StackPoint &p = stack.at(i);
            std::vector<Operation> &ops = p.first;
            std::vector<Operation> &undoOps = p.second;
            for(auto &op:ops) op.removeElements();
            for(auto &op:undoOps) op.removeElements();
        }
        stack.resize(cur_pos);
    }

    if(groupflag) {
        std::copy(point.first.begin(),point.first.end(),std::back_inserter(groupPoint.first));
        std::copy(point.second.begin(),point.second.end(),std::back_inserter(groupPoint.second));
        group_pos++;
    }else {
        stack.push_back(point);
        cur_pos++;
    }
}

void CmdStack::doOperations(std::vector<Operation> &ops)
{
    work = false;
    for(Operation &op: ops) {
        switch (op.type) {
            case Operation::OpType::ADD_ELEMENT:
                scene->addElement(op.elements.at(0)->clone(),op.intParams.at(0),op.intParams.at(1));
                break;
            case Operation::OpType::REMOVE_ELEMENT:
                if(!op.elements.empty()) {
                    scene->removeElement(op.intParams.at(0),op.intParams.at(1),op.elements.at(0)->getType());
                }else {
                    scene->removeElement(op.intParams.at(0),op.intParams.at(1));
                }
                break;
        }
    }
    work = true;
}

bool CmdStack::undo()
{
    if(cur_pos) {
        StackPoint point = stack.at(cur_pos-1);
        std::vector<Operation> ops = point.second;
        std::reverse(ops.begin(),ops.end());
        doOperations(ops);//point.second);
        cur_pos--;
        return true;
    }
    return false;
}

bool CmdStack::redo()
{
    if(cur_pos<stack.size()) {
        StackPoint point = stack.at(cur_pos);
        doOperations(point.first);
        cur_pos++;
        return true;
    }
    return false;
}

void CmdStack::clear()
{
    //for(std::size_t i=0;i<stack.size();i++) {
    for(StackPoint &p:stack) {
        //StackPoint &p = stack.at(i);
        std::vector<Operation> &ops = p.first;
        std::vector<Operation> &undoOps = p.second;
        for(auto &op:ops) op.removeElements();
        for(auto &op:undoOps) op.removeElements();
    }
    stack.resize(0);
    std::vector<Operation> &ops = groupPoint.first;
    std::vector<Operation> &undoOps = groupPoint.second;
    for(auto &op:ops) op.removeElements();
    for(auto &op:undoOps) op.removeElements();
    cur_pos=0;
    group_pos = 0;
    groupflag = false;
}
