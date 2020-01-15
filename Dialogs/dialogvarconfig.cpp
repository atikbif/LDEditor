#include "dialogvarconfig.h"
#include "ui_dialogvarconfig.h"
#include <QMessageBox>
#include "dialogaddvar.h"
#include <QDebug>
#include "dialogeditnotsystemvar.h"
#include "dialogeditsystemvar.h"
#include "dialogrenamegroup.h"

DialogVarConfig::DialogVarConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVarConfig)
{
    ui->setupUi(this);

    drawVarTree();
    PLCVarContainer::getInstance().saveState();
}

DialogVarConfig::~DialogVarConfig()
{
    delete ui;
}

void DialogVarConfig::on_pushButtonAddVar_clicked()
{
    auto *dialog = new DialogAddVar();
    if(dialog->exec()==QDialog::Accepted) {
        drawVarTree(dialog->getGroupName(),dialog->getVarName());
    }
}

void DialogVarConfig::on_DialogVarConfig_rejected()
{
    PLCVarContainer::getInstance().recoveryState();
}

void DialogVarConfig::drawVarTree(const QString &grName, const QString &varName)
{
    //ui->treeWidget->setSortingEnabled(true);
    //ui->treeWidget->sortByColumn(0,Qt::SortOrder::AscendingOrder);
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabels({"имя","тип","комментарий"});
    std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups();
    QTreeWidgetItem *curItem = nullptr;
    for(const QString &gr:groups) {
        QTreeWidgetItem *grItem = new QTreeWidgetItem(QStringList{gr});

        for(const auto & var:PLCVarContainer::getInstance().getVarsByGroup(gr)) {
            QTreeWidgetItem *varItem = new QTreeWidgetItem(QStringList{var.getName(),var.getType(),var.getComment()});
            grItem->addChild(varItem);
            if(grName==gr && varName==var.getName()) curItem = varItem;
        }
        ui->treeWidget->addTopLevelItem(grItem);
    }
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->setMinimumWidth(ui->treeWidget->columnWidth(0)+ui->treeWidget->columnWidth(1)+ui->treeWidget->columnWidth(2));
    if(curItem) {
        ui->treeWidget->setCurrentItem(curItem);
        on_treeWidget_itemClicked(curItem,0);
        curItem->setExpanded(true);
    }else {
        ui->pushButtonEditVar->setEnabled(false);
        ui->pushButtonAddVar->setEnabled(true);
        ui->pushButtonDelVar->setEnabled(false);
        ui->pushButtonDeleteGroup->setEnabled(false);
        ui->pushButtonRenameGroup->setEnabled(false);
    }
}

void DialogVarConfig::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if(item->parent()) {
        QString group = item->parent()->text(0);
        ui->pushButtonEditVar->setEnabled(true);
        std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
        if(std::find(groups.begin(),groups.end(),group)!=groups.end()) {
            ui->pushButtonDelVar->setEnabled(true);
        }else {
            ui->pushButtonDelVar->setEnabled(false);
        }
        ui->pushButtonDeleteGroup->setEnabled(false);
        ui->pushButtonRenameGroup->setEnabled(false);
    }else {
        QString group = item->text(0);
        ui->pushButtonEditVar->setEnabled(false);
        ui->pushButtonDelVar->setEnabled(false);
        std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
        if(std::find(groups.begin(),groups.end(),group)!=groups.end()) {
            ui->pushButtonDeleteGroup->setEnabled(true);
            ui->pushButtonRenameGroup->setEnabled(true);
        }else {
            ui->pushButtonDeleteGroup->setEnabled(false);
            ui->pushButtonRenameGroup->setEnabled(false);
        }
    }
}

void DialogVarConfig::on_pushButtonEditVar_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item && item->parent()) {
        QString group = item->parent()->text(0);
        std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
        if(std::find(groups.begin(),groups.end(),group)!=groups.end()) {
            QString varName = item->text(0);
            std::optional<PLCVar> var = PLCVarContainer::getInstance().getVarByGroupAndName(group,varName);
            if(var) {
                DialogEditNotSystemVar *dialog = new DialogEditNotSystemVar(varName,var->getType(),var->getComment());
                if(dialog->exec()==QDialog::Accepted) {
                    QString newVarName = dialog->getVarName();
                    PLCVar updVar(newVarName,group);
                    updVar.setComment(dialog->getVarComment());
                    QString vType = dialog->getVarType();
                    if(vType=="short") updVar.setValue(static_cast<short>(0));
                    else if(vType=="long") updVar.setValue(0l);
                    else if(vType=="double") updVar.setValue(0.0);
                    else if(vType=="bool") updVar.setValue(false);
                    updVar.setReadable(true);
                    updVar.setWriteable(true);
                    if(!varName.isEmpty()) {
                        PLCVarContainer::getInstance().deleteVar(group,varName);
                        PLCVarContainer::getInstance().addVar(updVar);
                        drawVarTree(group,newVarName);
                    }
                }
                delete dialog;
            }
        }else {
            QString varName = item->text(0);
            std::optional<PLCVar> var = PLCVarContainer::getInstance().getVarByGroupAndName(group,varName);
            if(var) {
                DialogEditSystemVar *dialog = new DialogEditSystemVar(var->getComment());
                if(dialog->exec()==QDialog::Accepted) {
                    QString newComment = dialog->getComment();
                    PLCVarContainer::getInstance().updateComment(var->getGroup(),var->getName(),newComment);
                    drawVarTree(var->getGroup(),var->getName());
                }
                delete dialog;
            }
        }
    }
}

void DialogVarConfig::on_pushButtonDelVar_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item && item->parent()) {
        QString group = item->parent()->text(0);
        QString name = item->text(0);
        if (QMessageBox::Yes == QMessageBox::question(this,
                                                      tr("Подтверждение операции"),
                                                      tr("Вы действительно хотите удалить эту переменную ?"))) {
            PLCVarContainer::getInstance().deleteVar(group,name);
            drawVarTree();
        }
    }
}

void DialogVarConfig::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    on_treeWidget_itemClicked(item, column);
    if(ui->pushButtonEditVar->isEnabled()) ui->pushButtonEditVar->click();
}

void DialogVarConfig::on_pushButtonRenameGroup_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item && !item->parent()) {
        QString group = item->text(0);

        std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
        if(std::find(groups.begin(),groups.end(),group)!=groups.end()) {
            auto *dialog = new DialogRenameGroup();
            if(dialog->exec()==QDialog::Accepted) {
                QString newGroupName = dialog->getGroupName();
                PLCVarContainer::getInstance().renameGroup(group,newGroupName);
                drawVarTree();
            }
        }
    }
}

void DialogVarConfig::on_pushButtonDeleteGroup_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item && !item->parent()) {
        QString group = item->text(0);
        std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
        if(std::find(groups.begin(),groups.end(),group)!=groups.end()) {
            if (QMessageBox::Yes == QMessageBox::question(this,
                                                          tr("Подтверждение операции"),
                                                          tr("Вы действительно хотите удалить эту группу ?"))) {
                PLCVarContainer::getInstance().delGroup(group);
                drawVarTree();
            }
        }

    }
}
