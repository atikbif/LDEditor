#include "dialogldelementproperties.h"
#include "ui_dialogldelementproperties.h"
#include <QDebug>
#include <QRegExp>
#include "plcutils.h"

DialogLDElementProperties::DialogLDElementProperties(LDElement *el, QWidget *parent) :
    QDialog(parent),el(el),
    ui(new Ui::DialogLDElementProperties)
{
    ui->setupUi(this);

    ui->lineEditName->setText(el->getName());
    ui->lineEditVarName->setReadOnly(true);

    grName = el->connectedVar.group;
    varName = el->connectedVar.name;
    parGrName = el->connectedVar.parentGroup;

    ui->lineEditComment->setText(el->getComment());
    if(!el->isNeedConnect()) {
        ui->groupBoxVarConnect->setVisible(false);
        setFixedHeight(minimumSizeHint().height());
    }else {

        std::optional<PLCVar> cVar = PLCVarContainer::getInstance().getVarByGroupAndName(el->connectedVar.group,el->connectedVar.name,el->connectedVar.parentGroup);
        if(cVar) {
            if(parGrName.isEmpty()) ui->lineEditVarName->setText(grName + ": " + varName);
            else ui->lineEditVarName->setText(parGrName + ": " + grName + ": " + varName);
            ui->lineEditInitValue->setText(cVar->getValueAsString());
        }
        QTreeWidgetItem *curItem = nullptr;
        std::vector<QTreeWidgetItem*> topLevelItems;
        ui->treeWidget->clear();
        ui->treeWidget->setHeaderLabels({"сист. имя","тип","имя пользов.","нач.значение"});
        std::vector<QString> parGroups = PLCVarContainer::getInstance().getParentGroups();
        QTreeWidgetItem *parGrItem = nullptr;
        for(const QString &parGroup:parGroups) {
            if(!parGroup.isEmpty()) parGrItem = new QTreeWidgetItem(QStringList{parGroup});
                std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups(parGroup);

                for(const QString &gr:groups) {
                    QTreeWidgetItem *grItem = new QTreeWidgetItem(QStringList{gr});

                    for(const auto & var:PLCVarContainer::getInstance().getVarsByGroup(gr,parGroup)) {
                        QTreeWidgetItem *varItem = new QTreeWidgetItem(QStringList{var.getName(),var.getType(),var.getComment(),var.getValueAsString()});
                        bool blockVar = false;
                        if(el->isOnlyRead() && !var.isReadable()) blockVar = true;
                        if(el->isOnlyWrite() && !var.isWriteable()) blockVar = true;
                        if(!blockVar) {
                            grItem->addChild(varItem);
                            if(cVar) {
                                if(cVar->getName()==var.getName() && cVar->getGroup()==var.getGroup() && cVar->getParentGroup()==var.getParentGroup()) curItem=varItem;
                            }
                        }

                    }
                    if(parGrItem) {
                        if(grItem->childCount()) {parGrItem->addChild(grItem);}
                    }else {
                        if(grItem->childCount()) topLevelItems.push_back(grItem);
                    }

                }
                if(parGrItem && parGrItem->childCount()) {topLevelItems.push_back(parGrItem);}
            }
            std::sort(topLevelItems.begin(),topLevelItems.end(),[](QTreeWidgetItem *item1, QTreeWidgetItem *item2){return item1->text(0)<item2->text(0);});
            for(QTreeWidgetItem *item:topLevelItems) {ui->treeWidget->addTopLevelItem(item);}

            if(curItem) {
                ui->treeWidget->expandItem(curItem);
                ui->treeWidget->setCurrentItem(curItem);
            }
        }

        ui->treeWidget->resizeColumnToContents(0);
        ui->treeWidget->resizeColumnToContents(1);
        ui->treeWidget->resizeColumnToContents(2);
        ui->treeWidget->setMinimumWidth(ui->treeWidget->columnWidth(0)+ui->treeWidget->columnWidth(1)+ui->treeWidget->columnWidth(2));

}

DialogLDElementProperties::~DialogLDElementProperties()
{
    delete ui;
}

void DialogLDElementProperties::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if(item && (item->childCount()==0) && el) {
        parGrName = "";
        grName = item->parent()->text(0);
        varName = item->text(0);
        comment = item->text(2);
        if(item->parent()->parent()) parGrName = item->parent()->parent()->text(0);
        if(parGrName.isEmpty()) ui->lineEditVarName->setText(grName + ": " + varName);
        else ui->lineEditVarName->setText(parGrName + ": " + grName + ": " + varName);
        ui->lineEditInitValue->setText(item->text(3));

        if(!comment.isEmpty()) ui->lineEditName->setText(comment);
        //el->connectedVar.group = grName;
        //el->connectedVar.name = varName;
    }
}

void DialogLDElementProperties::on_buttonBox_accepted()
{
    if(el) {
        el->connectedVar.group = grName;
        el->connectedVar.name = varName;
        el->connectedVar.parentGroup = parGrName;
        QString name = ui->lineEditName->text();
        auto v = PLCVarContainer::getInstance().getVarByGroupAndName(grName,varName,parGrName);
        if(v) {
            if(v->isSystem()) PLCUtils::updateSystemVarComment(varName,grName,ui->lineEditName->text(),parGrName);
            else PLCVarContainer::getInstance().updateComment(grName,varName,ui->lineEditName->text(),parGrName);
            PLCVarContainer::getInstance().updateInitValue(grName,varName,ui->lineEditInitValue->text(),parGrName);
        }
        el->setName(ui->lineEditName->text());
        el->setComment(ui->lineEditComment->text());
        if(!comment.isEmpty()) el->setName(comment);
    }
}

void DialogLDElementProperties::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item && (item->childCount()==0) && el) {
        on_treeWidget_itemClicked(item,column);
        on_buttonBox_accepted();
        accept();
    }
}
