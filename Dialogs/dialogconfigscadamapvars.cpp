#include "dialogconfigscadamapvars.h"
#include "ui_dialogconfigscadamapvars.h"
#include "plcvarcontainer.h"
#include <vector>
#include <QCheckBox>
#include <QComboBox>
#include <QRegExp>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>

void DialogConfigScadaMapVars::createDITable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    QRegExp diExp("^DI(\\d+)$");

    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("состояние", "Дискретные входы");
    ui->tableWidgetDI->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetDI->setSelectionMode(QAbstractItemView::NoSelection);
    for(const auto &var:vars) {
        if(diExp.indexIn(var.getName())!=-1) {
            int num = diExp.cap(1).toInt();
            if(num>0) {
                num--;
                bool inpType = static_cast<bool>(plcConfig.getSettings().at(num));
                if(inpType) {
                    int row = ui->tableWidgetDI->rowCount();
                    ui->tableWidgetDI->insertRow(row);
                    ui->tableWidgetDI->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
                    ui->tableWidgetDI->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));

                    QComboBox *box1 = new QComboBox();
                    box1->setStyleSheet("background-color: white;");
                    for(auto state:states) {
                        box1->addItem(state.first);
                        box1->addItem(state.second);
                    }
                    connect(box1,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,row](int index){
                        QComboBox *box = dynamic_cast<QComboBox*>(ui->tableWidgetDI->cellWidget(row,3));
                        if(box) {
                            if(index&0x01) index--;else index++;
                            box->setCurrentIndex(index);
                        }
                    });
                    box1->setCurrentText("OPEN");
                    ui->tableWidgetDI->setCellWidget(row,2,box1);

                    QComboBox *box2 = new QComboBox();
                    box2->setStyleSheet("background-color: white;");
                    for(auto state:states) {
                        box2->addItem(state.first);
                        box2->addItem(state.second);
                    }
                    box2->setCurrentText("CLOSED");
                    ui->tableWidgetDI->setCellWidget(row,3,box2);

                    // add checkbox
                    QWidget* w = new QWidget();
                    auto *l = new QHBoxLayout();
                    l->setMargin(5);
                    auto *check = new QCheckBox();
                    l->addWidget(check,0,Qt::AlignCenter);
                    w->setLayout(l);
                    ui->tableWidgetDI->setCellWidget(row,4,w);
                    connect(check,&QCheckBox::clicked,[this,row](bool clicked){
                        QBrush br(QColor(0,255,0,100));
                        if(clicked) {
                            ui->tableWidgetDI->item(row,0)->setBackground(br);
                            ui->tableWidgetDI->item(row,1)->setBackground(br);
                            ui->tableWidgetDI->cellWidget(row,2)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                            ui->tableWidgetDI->cellWidget(row,3)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                            ui->tableWidgetDI->cellWidget(row,4)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                        }else {
                            ui->tableWidgetDI->item(row,0)->setBackground(Qt::white);
                            ui->tableWidgetDI->item(row,1)->setBackground(Qt::white);
                            ui->tableWidgetDI->cellWidget(row,2)->setStyleSheet("background: white;");
                            ui->tableWidgetDI->cellWidget(row,3)->setStyleSheet("background: white;");
                            ui->tableWidgetDI->cellWidget(row,4)->setStyleSheet("background: white;");
                        }
                    });
                    auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
                    if(it!=chVars.cend()) {
                        check->click();
                        box1->setCurrentText(it->properties.at("FALSE"));
                        box2->setCurrentText(it->properties.at("TRUE"));
                    }
                }
            }
        }
    }
    ui->tableWidgetDI->resizeColumnToContents(1);
    ui->tableWidgetDI->resizeColumnToContents(2);
    ui->tableWidgetDI->resizeColumnToContents(3);
}

void DialogConfigScadaMapVars::createDOTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Дискретные выходы");
    ui->tableWidgetDO->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetDO->setSelectionMode(QAbstractItemView::NoSelection);
    for(const auto &var:vars) {
        int row = ui->tableWidgetDO->rowCount();
        ui->tableWidgetDO->insertRow(row);
        ui->tableWidgetDO->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
        ui->tableWidgetDO->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));

        // add checkbox
        QWidget* w = new QWidget();
        auto *l = new QHBoxLayout();
        l->setMargin(5);
        auto *check = new QCheckBox();
        l->addWidget(check,0,Qt::AlignCenter);
        w->setLayout(l);
        ui->tableWidgetDO->setCellWidget(row,2,w);
        connect(check,&QCheckBox::clicked,[this,row](bool clicked){
            QBrush br(QColor(0,255,0,100));
            if(clicked) {
                ui->tableWidgetDO->item(row,0)->setBackground(br);
                ui->tableWidgetDO->item(row,1)->setBackground(br);
                ui->tableWidgetDO->cellWidget(row,2)->setStyleSheet("background: rgba(0, 255, 0, 100);");
            }else {
                ui->tableWidgetDO->item(row,0)->setBackground(Qt::white);
                ui->tableWidgetDO->item(row,1)->setBackground(Qt::white);
                ui->tableWidgetDO->cellWidget(row,2)->setStyleSheet("background: white;");
            }
        });
        auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
        if(it!=chVars.cend()) {
            check->click();
        }
    }
    ui->tableWidgetDO->resizeColumnToContents(1);
    ui->tableWidgetDO->resizeColumnToContents(2);
}

void DialogConfigScadaMapVars::createAITable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    QRegExp aiExp("^AI(\\d+)$");
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("состояние","Аналоговые входы");
    ui->tableWidgetAI->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetAI->setSelectionMode(QAbstractItemView::NoSelection);
    for(const auto &var:vars) {
        int row = ui->tableWidgetAI->rowCount();
        if(aiExp.indexIn(var.getName())!=-1) {
            int num = aiExp.cap(1).toInt();
            int sensCount = plcConfig.getADCSensorsCount();
            int sensNum = plcConfig.getSensorType(num-1);
            if(sensNum && sensNum<=sensCount) {
                auto sens = plcConfig.getADCSensor(sensNum-1);
                QString tdu = sens.getName() + "\t" + sens.getLowLimit() + "-" + sens.getHighLimit();
                QString measUnit = sens.getUnit();
                if(!measUnit.isEmpty()) tdu += " " + measUnit;
                int sensType = plcConfig.getSensorTypeCode(num-1);
                switch(sensType) {
                    case 0: tdu+=" (0.4-2В)";break;
                    case 1: tdu+=" (0-20мА)";break;
                    case 2: tdu+=" (4-20мА)";break;
                    case 3: tdu+=" (2-10мА)";break;
                    case 4:tdu+=" (0-2.5В)";break;
                }
                ui->tableWidgetAI->insertRow(row);
                ui->tableWidgetAI->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
                ui->tableWidgetAI->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));
                ui->tableWidgetAI->setItem(row,2,new QTableWidgetItem(QString("  ") + tdu + "  "));

                // add checkbox
                QWidget* w = new QWidget();
                auto *l = new QHBoxLayout();
                l->setMargin(5);
                auto *check = new QCheckBox();
                l->addWidget(check,0,Qt::AlignCenter);
                w->setLayout(l);
                ui->tableWidgetAI->setCellWidget(row,3,w);
                connect(check,&QCheckBox::clicked,[this,row](bool clicked){
                    QBrush br(QColor(0,255,0,100));
                    if(clicked) {
                        ui->tableWidgetAI->item(row,0)->setBackground(br);
                        ui->tableWidgetAI->item(row,1)->setBackground(br);
                        ui->tableWidgetAI->item(row,2)->setBackground(br);
                        ui->tableWidgetAI->cellWidget(row,3)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                    }else {
                        ui->tableWidgetAI->item(row,0)->setBackground(Qt::white);
                        ui->tableWidgetAI->item(row,1)->setBackground(Qt::white);
                        ui->tableWidgetAI->item(row,2)->setBackground(Qt::white);
                        ui->tableWidgetAI->cellWidget(row,3)->setStyleSheet("background: white;");
                    }
                });
                auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
                if(it!=chVars.cend()) {
                    check->click();
                }
            }
        }
    }
    ui->tableWidgetAI->resizeColumnToContents(1);
    ui->tableWidgetAI->resizeColumnToContents(2);
    ui->tableWidgetAI->resizeColumnToContents(3);
}

void DialogConfigScadaMapVars::createClustBitsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups("Биты кластера");

    ui->tableWidgetClustBits->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetClustBits->setSelectionMode(QAbstractItemView::NoSelection);

    int rowCount = 0;
    for(const QString &group: groups) {
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group,"Биты кластера");
        rowCount += vars.size();
    }

    int row = 0;
    for(const QString &group: groups) {
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group,"Биты кластера");
        for(const PLCVar &var: vars) {
            ui->tableWidgetClustBits->insertRow(row);
            ui->tableWidgetClustBits->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getGroup() + "  "));
            ui->tableWidgetClustBits->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
            ui->tableWidgetClustBits->setItem(row,2,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));

            QComboBox *box1 = new QComboBox();
            box1->addItems(falseTrueStates);
            box1->setStyleSheet("background-color: white;");
            connect(box1,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,row](int index){
                QComboBox *box = dynamic_cast<QComboBox*>(ui->tableWidgetClustBits->cellWidget(row,4));
                if(box) {
                    if(index&0x01) index--;else index++;
                    box->setCurrentIndex(index);
                }
            });
            box1->setCurrentText("OPEN");
            ui->tableWidgetClustBits->setCellWidget(row,3,box1);

            QComboBox *box2 = new QComboBox();
            box2->addItems(falseTrueStates);
            box2->setStyleSheet("background-color: white;");
            box2->setCurrentText("CLOSED");
            ui->tableWidgetClustBits->setCellWidget(row,4,box2);

            // add checkbox
            QWidget* w = new QWidget();
            auto *l = new QHBoxLayout();
            l->setMargin(5);
            auto *check = new QCheckBox();
            l->addWidget(check,0,Qt::AlignCenter);
            w->setLayout(l);
            ui->tableWidgetClustBits->setCellWidget(row,5,w);
            connect(check,&QCheckBox::clicked,[this,row](bool clicked){
                QBrush br(QColor(0,255,0,100));
                if(clicked) {
                    ui->tableWidgetClustBits->item(row,0)->setBackground(br);
                    ui->tableWidgetClustBits->item(row,1)->setBackground(br);
                    ui->tableWidgetClustBits->item(row,2)->setBackground(br);
                    ui->tableWidgetClustBits->cellWidget(row,3)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                    ui->tableWidgetClustBits->cellWidget(row,4)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                    ui->tableWidgetClustBits->cellWidget(row,5)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                }else {
                    ui->tableWidgetClustBits->item(row,0)->setBackground(Qt::white);
                    ui->tableWidgetClustBits->item(row,1)->setBackground(Qt::white);
                    ui->tableWidgetClustBits->item(row,2)->setBackground(Qt::white);
                    ui->tableWidgetClustBits->cellWidget(row,3)->setStyleSheet("background: white;");
                    ui->tableWidgetClustBits->cellWidget(row,4)->setStyleSheet("background: white;");
                    ui->tableWidgetClustBits->cellWidget(row,5)->setStyleSheet("background: white;");
                }
            });
            auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
            if(it!=chVars.cend()) {
                check->click();
                box1->setCurrentText(it->properties.at("FALSE"));
                box2->setCurrentText(it->properties.at("TRUE"));
            }
            row++;
        }
    }
    ui->tableWidgetClustBits->resizeColumnToContents(0);
    ui->tableWidgetClustBits->resizeColumnToContents(1);
    ui->tableWidgetClustBits->resizeColumnToContents(2);
    ui->tableWidgetClustBits->resizeColumnToContents(3);
    ui->tableWidgetClustBits->resizeColumnToContents(4);
}

void DialogConfigScadaMapVars::createClustRegsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Регистры кластера");

    ui->tableWidgetClustRegs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetClustRegs->setSelectionMode(QAbstractItemView::NoSelection);

    for(const PLCVar &var: vars) {
        int row = ui->tableWidgetClustRegs->rowCount();
        ui->tableWidgetClustRegs->insertRow(row);
        ui->tableWidgetClustRegs->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
        ui->tableWidgetClustRegs->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));
        ui->tableWidgetClustRegs->setItem(row,2,new QTableWidgetItem(QString("  ") + var.getValueAsString() + "  "));

        // add checkbox
        QWidget* w = new QWidget();
        auto *l = new QHBoxLayout();
        l->setMargin(5);
        auto *check = new QCheckBox();
        l->addWidget(check,0,Qt::AlignCenter);
        w->setLayout(l);
        ui->tableWidgetClustRegs->setCellWidget(row,3,w);
        connect(check,&QCheckBox::clicked,[this,row](bool clicked){
            QBrush br(QColor(0,255,0,100));
            if(clicked) {
                ui->tableWidgetClustRegs->item(row,0)->setBackground(br);
                ui->tableWidgetClustRegs->item(row,1)->setBackground(br);
                ui->tableWidgetClustRegs->item(row,2)->setBackground(br);
                ui->tableWidgetClustRegs->cellWidget(row,3)->setStyleSheet("background: rgba(0, 255, 0, 100);");
            }else {
                ui->tableWidgetClustRegs->item(row,0)->setBackground(Qt::white);
                ui->tableWidgetClustRegs->item(row,1)->setBackground(Qt::white);
                ui->tableWidgetClustRegs->item(row,2)->setBackground(Qt::white);
                ui->tableWidgetClustRegs->cellWidget(row,3)->setStyleSheet("background: white;");
            }
        });
        auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
        if(it!=chVars.cend()) {
            check->click();
        }
    }
    ui->tableWidgetClustRegs->resizeColumnToContents(1);
}

void DialogConfigScadaMapVars::createNetBitsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups("Сетевые биты");

    ui->tableWidgetNetBits->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetNetBits->setSelectionMode(QAbstractItemView::NoSelection);

    int rowCount = 0;
    for(const QString &group: groups) {
        if(group=="TX") continue;
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group,"Сетевые биты");
        rowCount += vars.size();
    }
    ui->tableWidgetNetBits->setRowCount(rowCount);

    int row = 0;
    for(const QString &group: groups) {
        if(group=="TX") continue;
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group,"Сетевые биты");
        for(const PLCVar &var: vars) {
            //ui->tableWidgetNetBits->insertRow(row);
            ui->tableWidgetNetBits->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getGroup() + "  "));
            ui->tableWidgetNetBits->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
            ui->tableWidgetNetBits->setItem(row,2,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));

            QComboBox *box1 = new QComboBox();
            box1->setStyleSheet("background-color: white;");
            box1->addItems(falseTrueStates);
            connect(box1,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,row](int index){
                QComboBox *box = dynamic_cast<QComboBox*>(ui->tableWidgetNetBits->cellWidget(row,4));
                if(box) {
                    if(index&0x01) index--;else index++;
                    box->setCurrentIndex(index);
                }
            });
            box1->setCurrentText("OPEN");
            ui->tableWidgetNetBits->setCellWidget(row,3,box1);

            QComboBox *box2 = new QComboBox();
            box2->setStyleSheet("background-color: white;");
            box2->addItems(falseTrueStates);
            box2->setCurrentText("CLOSED");
            ui->tableWidgetNetBits->setCellWidget(row,4,box2);

            // add checkbox
            QWidget* w = new QWidget();
            auto *l = new QHBoxLayout();
            l->setMargin(5);
            auto *check = new QCheckBox();
            l->addWidget(check,0,Qt::AlignCenter);
            w->setLayout(l);
            ui->tableWidgetNetBits->setCellWidget(row,5,w);
            connect(check,&QCheckBox::clicked,[this,row](bool clicked){
                QBrush br(QColor(0,255,0,100));
                if(clicked) {
                    ui->tableWidgetNetBits->item(row,0)->setBackground(br);
                    ui->tableWidgetNetBits->item(row,1)->setBackground(br);
                    ui->tableWidgetNetBits->item(row,2)->setBackground(br);
                    ui->tableWidgetNetBits->cellWidget(row,3)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                    ui->tableWidgetNetBits->cellWidget(row,4)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                    ui->tableWidgetNetBits->cellWidget(row,5)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                }else {
                    ui->tableWidgetNetBits->item(row,0)->setBackground(Qt::white);
                    ui->tableWidgetNetBits->item(row,1)->setBackground(Qt::white);
                    ui->tableWidgetNetBits->item(row,2)->setBackground(Qt::white);
                    ui->tableWidgetNetBits->cellWidget(row,3)->setStyleSheet("background: white;");
                    ui->tableWidgetNetBits->cellWidget(row,4)->setStyleSheet("background: white;");
                    ui->tableWidgetNetBits->cellWidget(row,5)->setStyleSheet("background: white;");
                }
            });
            auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
            if(it!=chVars.cend()) {
                check->click();
                box1->setCurrentText(it->properties.at("FALSE"));
                box2->setCurrentText(it->properties.at("TRUE"));
            }
            ++row;
        }
    }
    ui->tableWidgetNetBits->resizeColumnToContents(0);
    ui->tableWidgetNetBits->resizeColumnToContents(1);
    ui->tableWidgetNetBits->resizeColumnToContents(2);
    ui->tableWidgetNetBits->resizeColumnToContents(3);
    ui->tableWidgetNetBits->resizeColumnToContents(4);
}

void DialogConfigScadaMapVars::createNetRegsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups("Сетевые регистры");

    ui->tableWidgetNetRegs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetNetRegs->setSelectionMode(QAbstractItemView::NoSelection);

    for(const QString &group: groups) {
        if(group=="TX") continue;
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group,"Сетевые регистры");
        for(const PLCVar &var: vars) {
            int row = ui->tableWidgetNetRegs->rowCount();
            ui->tableWidgetNetRegs->insertRow(row);
            ui->tableWidgetNetRegs->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getGroup() + "  "));
            ui->tableWidgetNetRegs->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
            ui->tableWidgetNetRegs->setItem(row,2,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));
            ui->tableWidgetNetRegs->setItem(row,3,new QTableWidgetItem(QString("  ") + var.getValueAsString() + "  "));

            // add checkbox
            QWidget* w = new QWidget();
            auto *l = new QHBoxLayout();
            l->setMargin(5);
            auto *check = new QCheckBox();
            l->addWidget(check,0,Qt::AlignCenter);
            w->setLayout(l);
            ui->tableWidgetNetRegs->setCellWidget(row,4,w);
            connect(check,&QCheckBox::clicked,[this,row](bool clicked){
                QBrush br(QColor(0,255,0,100));
                if(clicked) {
                    ui->tableWidgetNetRegs->item(row,0)->setBackground(br);
                    ui->tableWidgetNetRegs->item(row,1)->setBackground(br);
                    ui->tableWidgetNetRegs->item(row,2)->setBackground(br);
                    ui->tableWidgetNetRegs->item(row,3)->setBackground(br);
                    ui->tableWidgetNetRegs->cellWidget(row,4)->setStyleSheet("background: rgba(0, 255, 0, 100);");
                }else {
                    ui->tableWidgetNetRegs->item(row,0)->setBackground(Qt::white);
                    ui->tableWidgetNetRegs->item(row,1)->setBackground(Qt::white);
                    ui->tableWidgetNetRegs->item(row,2)->setBackground(Qt::white);
                    ui->tableWidgetNetRegs->item(row,3)->setBackground(Qt::white);
                    ui->tableWidgetNetRegs->cellWidget(row,4)->setStyleSheet("background: white;");
                }
            });
            auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
            if(it!=chVars.cend()) {
                check->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::createTelemetryBitsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Биты","Скада");

    ui->tableWidgetTelemBits->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetTelemBits->setSelectionMode(QAbstractItemView::NoSelection);

    for(const PLCVar &var: vars) {
        int row = ui->tableWidgetTelemBits->rowCount();
        ui->tableWidgetTelemBits->insertRow(row);
        ui->tableWidgetTelemBits->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
        ui->tableWidgetTelemBits->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));

        // add checkbox
        QWidget* w = new QWidget();
        auto *l = new QHBoxLayout();
        l->setMargin(5);
        auto *check = new QCheckBox();
        l->addWidget(check,0,Qt::AlignCenter);
        w->setLayout(l);
        ui->tableWidgetTelemBits->setCellWidget(row,2,w);
        connect(check,&QCheckBox::clicked,[this,row](bool clicked){
            QBrush br(QColor(0,255,0,100));
            if(clicked) {
                ui->tableWidgetTelemBits->item(row,0)->setBackground(br);
                ui->tableWidgetTelemBits->item(row,1)->setBackground(br);
                ui->tableWidgetTelemBits->cellWidget(row,2)->setStyleSheet("background: rgba(0, 255, 0, 100);");
            }else {
                ui->tableWidgetTelemBits->item(row,0)->setBackground(Qt::white);
                ui->tableWidgetTelemBits->item(row,1)->setBackground(Qt::white);
                ui->tableWidgetTelemBits->cellWidget(row,2)->setStyleSheet("background: white;");
            }
        });
        auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
        if(it!=chVars.cend()) {
            check->click();
        }
    }
}

void DialogConfigScadaMapVars::createTelemetryRegsTable(const std::vector<DialogConfigScadaMapVars::VarDescription> &chVars)
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Регистры","Скада");

    ui->tableWidgetTelemRegs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetTelemRegs->setSelectionMode(QAbstractItemView::NoSelection);

    for(const PLCVar &var: vars) {
        int row = ui->tableWidgetTelemRegs->rowCount();
        ui->tableWidgetTelemRegs->insertRow(row);
        ui->tableWidgetTelemRegs->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
        ui->tableWidgetTelemRegs->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));
        ui->tableWidgetTelemRegs->setItem(row,2,new QTableWidgetItem(QString("  ") + var.getValueAsString() + "  "));

        // add checkbox
        QWidget* w = new QWidget();
        auto *l = new QHBoxLayout();
        l->setMargin(5);
        auto *check = new QCheckBox();
        l->addWidget(check,0,Qt::AlignCenter);
        w->setLayout(l);
        ui->tableWidgetTelemRegs->setCellWidget(row,3,w);
        connect(check,&QCheckBox::clicked,[this,row](bool clicked){
            QBrush br(QColor(0,255,0,100));
            if(clicked) {
                ui->tableWidgetTelemRegs->item(row,0)->setBackground(br);
                ui->tableWidgetTelemRegs->item(row,1)->setBackground(br);
                ui->tableWidgetTelemRegs->item(row,2)->setBackground(br);
                ui->tableWidgetTelemRegs->cellWidget(row,3)->setStyleSheet("background: rgba(0, 255, 0, 100);");
            }else {
                ui->tableWidgetTelemRegs->item(row,0)->setBackground(Qt::white);
                ui->tableWidgetTelemRegs->item(row,1)->setBackground(Qt::white);
                ui->tableWidgetTelemRegs->item(row,2)->setBackground(Qt::white);
                ui->tableWidgetTelemRegs->cellWidget(row,3)->setStyleSheet("background: white;");
            }
        });
        auto it = std::find_if(chVars.cbegin(),chVars.cend(),[var](VarDescription v){return v.varName==var.getName();});
        if(it!=chVars.cend()) {
            check->click();
        }
    }
}

void DialogConfigScadaMapVars::markDITable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetDI->rowCount();
    int check_col = 4;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetDI->item(i,0)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetDI->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markDOTable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetDO->rowCount();
    int check_col = 2;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetDO->item(i,0)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetDO->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markAITable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetAI->rowCount();
    int check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetAI->item(i,0)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetAI->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markClustBitsTable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetClustBits->rowCount();
    int check_col = 5;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetClustBits->item(i,1)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetClustBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markClustRegsTable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetClustRegs->rowCount();
    int check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetClustRegs->item(i,0)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetClustRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markNetBitsTable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetNetBits->rowCount();
    int check_col = 5;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetNetBits->item(i,1)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetNetBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markNetRegsTable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetNetRegs->rowCount();
    int check_col = 4;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetNetRegs->item(i,1)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetNetRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markTelemetryBitsTable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetTelemBits->rowCount();
    int check_col = 2;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetTelemBits->item(i,0)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetTelemBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::markTelemetryRegsTable(const std::vector<QString> &names)
{
    int rowCount = ui->tableWidgetTelemRegs->rowCount();
    int check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetTelemRegs->item(i,0)->text();
        if(std::find(names.cbegin(),names.cend(),name.trimmed())!=names.cend()) {
            QWidget *w = ui->tableWidgetTelemRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if(box && !box->isChecked()) {
                box->click();
            }
        }
    }
}

void DialogConfigScadaMapVars::clearDITable()
{
    int rowCount = ui->tableWidgetDI->rowCount();
    int check_col = 4;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetDI->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearDOTable()
{
    int rowCount = ui->tableWidgetDO->rowCount();
    int check_col = 2;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetDO->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearAITable()
{
    int rowCount = ui->tableWidgetAI->rowCount();
    int check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetAI->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearClustBitsTable()
{
    int rowCount = ui->tableWidgetClustBits->rowCount();
    int check_col = 5;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetClustBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearClustRegsTable()
{
    int rowCount = ui->tableWidgetClustRegs->rowCount();
    int check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetClustRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearNetBitsTable()
{
    int rowCount = ui->tableWidgetNetBits->rowCount();
    int check_col = 5;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetNetBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearNetRegsTable()
{
    int rowCount = ui->tableWidgetNetRegs->rowCount();
    int check_col = 4;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetNetRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearTelemetryBitsTable()
{
    int rowCount = ui->tableWidgetTelemBits->rowCount();
    int check_col = 2;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetTelemBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

void DialogConfigScadaMapVars::clearTelemetryRegsTable()
{
    int rowCount = ui->tableWidgetTelemRegs->rowCount();
    int check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QWidget *w = ui->tableWidgetTelemRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            box->click();
        }
    }
}

DialogConfigScadaMapVars::DialogConfigScadaMapVars(const PLCConfig &plcConfig, const std::vector<VarDescription> &chVars, const std::vector<QString> &prVars, QWidget *parent) :
    QDialog(parent),plcConfig(plcConfig),prVars(prVars),
    ui(new Ui::DialogConfigScadaMapVars)
{
    ui->setupUi(this);

    states = {
        {"HEALTHY","TRIP"},
        {"FAILED", "WARNING"},
        {"OPEN", "CLOSED"},
        {"NOT OPERATED","OPERATED"},
        {"NOT SELECTED","SELECTED"},
        {"NOT ACTIVE","ACTIVE"},
        {"STOOD","RUNNING"},
        {"OUT","IN"},
        {"ВКЛ","ВЫКЛ"},
        {"ON","OFF"},
        {"0","1"}
    };

    for(auto state:states) {
        falseTrueStates.push_back(state.first);
        falseTrueStates.push_back(state.second);
    }

    createDITable(chVars);
    createDOTable(chVars);
    createAITable(chVars);
    createClustBitsTable(chVars);
    createClustRegsTable(chVars);
    createNetBitsTable(chVars);
    createNetRegsTable(chVars);
    createTelemetryBitsTable(chVars);
    createTelemetryRegsTable(chVars);

    //auto res = getCheckedVars();
}

DialogConfigScadaMapVars::~DialogConfigScadaMapVars()
{
    delete ui;
}

std::vector<DialogConfigScadaMapVars::VarDescription> DialogConfigScadaMapVars::getCheckedVars() const
{
    std::vector<DialogConfigScadaMapVars::VarDescription> res;

    // DI
    int rowCount = ui->tableWidgetDI->rowCount();
    int check_col = 4;
    int false_col = 2;
    int true_col = 3;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetDI->item(i,0)->text();
        QWidget *w = ui->tableWidgetDI->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            std::map<QString,QString> properties;
            QComboBox * box1 = dynamic_cast<QComboBox*>(ui->tableWidgetDI->cellWidget(i,false_col));
            QComboBox * box2 = dynamic_cast<QComboBox*>(ui->tableWidgetDI->cellWidget(i,true_col));
            if(box1 && box2) {
                properties["FALSE"] = box1->currentText();
                properties["TRUE"] = box2->currentText();
                var.properties = properties;
                res.push_back(var);
            }
        }
    }

    // DO
    rowCount = ui->tableWidgetDO->rowCount();
    check_col = 2;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetDO->item(i,0)->text();
        QWidget *w = ui->tableWidgetDO->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            var.properties = {};
            res.push_back(var);
        }
    }

    // AI
    rowCount = ui->tableWidgetAI->rowCount();
    check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetAI->item(i,0)->text();
        QWidget *w = ui->tableWidgetAI->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            var.properties = {};
            res.push_back(var);
        }
    }

    // Cluster bits
    rowCount = ui->tableWidgetClustBits->rowCount();
    check_col = 5;
    false_col = 3;
    true_col = 4;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetClustBits->item(i,1)->text();
        QWidget *w = ui->tableWidgetClustBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            std::map<QString,QString> properties;
            QComboBox * box1 = dynamic_cast<QComboBox*>(ui->tableWidgetClustBits->cellWidget(i,false_col));
            QComboBox * box2 = dynamic_cast<QComboBox*>(ui->tableWidgetClustBits->cellWidget(i,true_col));
            if(box1 && box2) {
                properties["FALSE"] = box1->currentText();
                properties["TRUE"] = box2->currentText();
                var.properties = properties;
                res.push_back(var);
            }
        }
    }

    // Cluster regs
    rowCount = ui->tableWidgetClustRegs->rowCount();
    check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetClustRegs->item(i,0)->text();
        QWidget *w = ui->tableWidgetClustRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            var.properties = {};
            res.push_back(var);
        }
    }

    // Net bits
    rowCount = ui->tableWidgetNetBits->rowCount();
    check_col = 5;
    false_col = 3;
    true_col = 4;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetNetBits->item(i,1)->text();
        QWidget *w = ui->tableWidgetNetBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            std::map<QString,QString> properties;
            QComboBox * box1 = dynamic_cast<QComboBox*>(ui->tableWidgetNetBits->cellWidget(i,false_col));
            QComboBox * box2 = dynamic_cast<QComboBox*>(ui->tableWidgetNetBits->cellWidget(i,true_col));
            if(box1 && box2) {
                properties["FALSE"] = box1->currentText();
                properties["TRUE"] = box2->currentText();
                var.properties = properties;
                res.push_back(var);
            }
        }
    }

    // Net regs
    rowCount = ui->tableWidgetNetRegs->rowCount();
    check_col = 4;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetNetRegs->item(i,1)->text();
        QWidget *w = ui->tableWidgetNetRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            var.properties = {};
            res.push_back(var);
        }
    }

    // Scada bits
    rowCount = ui->tableWidgetTelemBits->rowCount();
    check_col = 2;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetTelemBits->item(i,0)->text();
        QWidget *w = ui->tableWidgetTelemBits->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            var.properties = {};
            res.push_back(var);
        }
    }

    // Scada regs
    rowCount = ui->tableWidgetTelemRegs->rowCount();
    check_col = 3;
    for(int i=0;i<rowCount;i++) {
        QString name = ui->tableWidgetTelemRegs->item(i,0)->text();
        QWidget *w = ui->tableWidgetTelemRegs->cellWidget(i,check_col)->layout()->itemAt(0)->widget();
        QCheckBox *box = dynamic_cast<QCheckBox*>(w);
        if(box && box->isChecked()) {
            VarDescription var;
            var.varName = name;
            var.properties = {};
            res.push_back(var);
        }
    }
    return res;
}

QByteArray DialogConfigScadaMapVars::scadaMapToBytes(const std::vector<VarDescription> &vars)
{
    QByteArray res;
    QDataStream out(&res,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    int version = 1;
    out << version;
    out << vars.size();
    for(const auto &var: vars) {
        out << var.varName;
        out << var.properties.size();
        for(const auto &pr:var.properties) {
            out << pr.first;
            out << pr.second;
        }
    }
    return res;
}

std::vector<DialogConfigScadaMapVars::VarDescription> DialogConfigScadaMapVars::scadaMapFromBytes(QByteArray &value)
{
    std::vector<DialogConfigScadaMapVars::VarDescription> vars;
    if(value.size()>0) {
        QDataStream in(&value,QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_13);
        int version = 0;
        in >> version;
        if(version == 1) {
            std::size_t varCnt = 0;
            in >> varCnt;
            for(std::size_t i=0;i<varCnt;i++) {
                DialogConfigScadaMapVars::VarDescription var;
                QString varName;
                in >> varName;
                var.varName = varName.trimmed();
                std::size_t prCnt = 0;
                in >> prCnt;
                std::map<QString,QString> prop;
                for(std::size_t j=0;j<prCnt;j++) {
                    QString prName;
                    QString prValue;
                    in >> prName;
                    in >> prValue;
                    prop[prName.trimmed()] = prValue.trimmed();
                }
                var.properties = prop;
                vars.push_back(var);
            }
        }
    }
    return vars;
}

QDataStream &operator<<(QDataStream &stream, const DialogConfigScadaMapVars::VarDescription &item)
{
    stream << item.varName;
    stream << item.properties.size();
    for(const auto &pair:item.properties) {
        stream << pair.first;
        stream << pair.second;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DialogConfigScadaMapVars::VarDescription &item)
{
    stream >> item.varName;
    std::size_t cnt = 0;
    item.properties.clear();
    stream >> cnt;
    for(std::size_t i=0;i<cnt;i++) {
        QString prName;
        QString prValue;
        stream >> prName;
        stream >> prValue;
        item.properties[prName] = prValue;
    }
    return stream;
}

void DialogConfigScadaMapVars::on_pushButton_clicked()
{
    markDITable(prVars);
    markDOTable(prVars);
    markAITable(prVars);
    markClustBitsTable(prVars);
    markClustRegsTable(prVars);
    markNetBitsTable(prVars);
    markNetRegsTable(prVars);
    markTelemetryBitsTable(prVars);
    markTelemetryRegsTable(prVars);
}

void DialogConfigScadaMapVars::on_pushButton_2_clicked()
{
    clearDITable();
    clearDOTable();
    clearAITable();
    clearClustBitsTable();
    clearClustRegsTable();
    clearNetBitsTable();
    clearNetRegsTable();
    clearTelemetryBitsTable();
    clearTelemetryRegsTable();
}
