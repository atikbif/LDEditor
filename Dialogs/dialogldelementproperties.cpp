#include "dialogldelementproperties.h"
#include "ui_dialogldelementproperties.h"
#include <QDebug>

DialogLDElementProperties::DialogLDElementProperties(LDElement *el, QWidget *parent) :
    QDialog(parent),el(el),
    ui(new Ui::DialogLDElementProperties)
{
    ui->setupUi(this);


    ui->lineEditName->setText(el->getName());
    ui->lineEditVarName->setReadOnly(true);

    grName = el->connectedVar.group;
    varName = el->connectedVar.name;

    ui->lineEditComment->setText(el->getComment());
    if(!el->isNeedConnect()) {
        ui->groupBoxVarConnect->setVisible(false);
        setFixedHeight(minimumSizeHint().height());
    }else {

        std::optional<PLCVar> cVar = PLCVarContainer::getInstance().getVarByGroupAndName(el->connectedVar.group,el->connectedVar.name);
        if(cVar) {
            ui->lineEditVarName->setText(grName + ": " + varName);
        }

        ui->treeWidget->setHeaderLabels({"имя","тип","комментарий"});
        std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups();
        QTreeWidgetItem *curItem = nullptr;
        for(const QString &gr:groups) {
            QTreeWidgetItem *grItem = new QTreeWidgetItem(QStringList{gr});

            for(const auto & var:PLCVarContainer::getInstance().getVarsByGroup(gr)) {
                bool blockVar = false;
                if(el->isOnlyRead() && !var.isReadable()) blockVar = true;
                if(el->isOnlyWrite() && !var.isWriteable()) blockVar = true;
                if(!blockVar) {
                    QTreeWidgetItem *varItem = new QTreeWidgetItem(QStringList{var.getName(),var.getType(),var.getComment()});
                    grItem->addChild(varItem);
                    if(cVar) {
                        if(cVar->getName()==var.getName() && cVar->getGroup()==var.getGroup()) curItem=varItem;
                    }
                }
            }
            if(grItem->childCount()) {
                ui->treeWidget->addTopLevelItem(grItem);
            }
        }
        ui->treeWidget->resizeColumnToContents(0);
        ui->treeWidget->resizeColumnToContents(1);
        ui->treeWidget->resizeColumnToContents(2);
        ui->treeWidget->setMinimumWidth(ui->treeWidget->columnWidth(0)+ui->treeWidget->columnWidth(1)+ui->treeWidget->columnWidth(2));

        if(curItem) {
            ui->treeWidget->expandItem(curItem);
            ui->treeWidget->setCurrentItem(curItem);
        }
    }
}

DialogLDElementProperties::~DialogLDElementProperties()
{
    delete ui;
}

void DialogLDElementProperties::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if(item && item->parent() && el) {
        grName = item->parent()->text(0);
        varName = item->text(0);
        comment = item->text(2);
        ui->lineEditVarName->setText(grName + ": " + varName);
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
        QString name = ui->lineEditName->text();
        auto v = PLCVarContainer::getInstance().getVarByGroupAndName(grName,varName);
        if(v) {
            PLCVarContainer::getInstance().updateComment(grName,varName,ui->lineEditName->text());
        }
        el->setName(ui->lineEditName->text());
        el->setComment(ui->lineEditComment->text());
        if(!comment.isEmpty()) el->setName(comment);
    }
}
