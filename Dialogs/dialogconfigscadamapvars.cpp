#include "dialogconfigscadamapvars.h"
#include "ui_dialogconfigscadamapvars.h"
#include "plcvarcontainer.h"
#include <vector>
#include <QCheckBox>
#include <QComboBox>
#include <QRegExp>

void DialogConfigScadaMapVars::createDITable()
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("состояние", "Дискретные входы");
    ui->tableWidgetDI->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetDI->setSelectionMode(QAbstractItemView::NoSelection);
    for(const auto &var:vars) {

        QRegExp diExp("^DI(\\d+)$");
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

                    QComboBox *box = new QComboBox();
                    box->setStyleSheet("background-color: white;");
                    for(auto state:states) {
                        box->addItem(state.first);
                        box->addItem(state.second);
                    }
                    connect(box,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,row](int index){
                        QComboBox *box = dynamic_cast<QComboBox*>(ui->tableWidgetDI->cellWidget(row,3));
                        if(box) {
                            if(index&0x01) index--;else index++;
                            box->setCurrentIndex(index);
                        }
                    });
                    box->setCurrentText("OPEN");
                    ui->tableWidgetDI->setCellWidget(row,2,box);

                    box = new QComboBox();
                    box->setStyleSheet("background-color: white;");
                    for(auto state:states) {
                        box->addItem(state.first);
                        box->addItem(state.second);
                    }
                    box->setCurrentText("CLOSED");
                    ui->tableWidgetDI->setCellWidget(row,3,box);

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
                }
            }
        }
    }
    ui->tableWidgetDI->resizeColumnToContents(1);
    ui->tableWidgetDI->resizeColumnToContents(2);
    ui->tableWidgetDI->resizeColumnToContents(3);
}

void DialogConfigScadaMapVars::createDOTable()
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
    }
    ui->tableWidgetDO->resizeColumnToContents(1);
    ui->tableWidgetDO->resizeColumnToContents(2);
}

void DialogConfigScadaMapVars::createAITable()
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("состояние","Аналоговые входы");
    ui->tableWidgetAI->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetAI->setSelectionMode(QAbstractItemView::NoSelection);
    for(const auto &var:vars) {
        int row = ui->tableWidgetAI->rowCount();

        QRegExp aiExp("^AI(\\d+)$");
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
            }
        }
    }
    ui->tableWidgetAI->resizeColumnToContents(1);
    ui->tableWidgetAI->resizeColumnToContents(2);
    ui->tableWidgetAI->resizeColumnToContents(3);
}

void DialogConfigScadaMapVars::createClustBitsTable()
{
    std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups("Биты кластера");
    for(const QString &group: groups) {
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group,"Биты кластера");
        for(const PLCVar &var: vars) {
            int row = ui->tableWidgetClustBits->rowCount();
            ui->tableWidgetClustBits->insertRow(row);
            ui->tableWidgetClustBits->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getGroup() + "  "));
            ui->tableWidgetClustBits->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
            ui->tableWidgetClustBits->setItem(row,2,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));

            QComboBox *box = new QComboBox();
            box->setStyleSheet("background-color: white;");
            for(auto state:states) {
                box->addItem(state.first);
                box->addItem(state.second);
            }
            connect(box,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,row](int index){
                QComboBox *box = dynamic_cast<QComboBox*>(ui->tableWidgetClustBits->cellWidget(row,4));
                if(box) {
                    if(index&0x01) index--;else index++;
                    box->setCurrentIndex(index);
                }
            });
            box->setCurrentText("OPEN");
            ui->tableWidgetClustBits->setCellWidget(row,3,box);

            box = new QComboBox();
            box->setStyleSheet("background-color: white;");
            for(auto state:states) {
                box->addItem(state.first);
                box->addItem(state.second);
            }
            box->setCurrentText("CLOSED");
            ui->tableWidgetClustBits->setCellWidget(row,4,box);

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
        }
    }
    ui->tableWidgetClustBits->resizeColumnToContents(0);
    ui->tableWidgetClustBits->resizeColumnToContents(1);
    ui->tableWidgetClustBits->resizeColumnToContents(2);
    ui->tableWidgetClustBits->resizeColumnToContents(3);
    ui->tableWidgetClustBits->resizeColumnToContents(4);
}

void DialogConfigScadaMapVars::createClustRegsTable()
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Регистры кластера");
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
    }
    ui->tableWidgetClustRegs->resizeColumnToContents(1);
}

void DialogConfigScadaMapVars::createNetBitsTable()
{
    std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups("Сетевые биты");
    for(const QString &group: groups) {
        if(group=="TX") continue;
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup(group,"Сетевые биты");
        for(const PLCVar &var: vars) {
            int row = ui->tableWidgetNetBits->rowCount();
            ui->tableWidgetNetBits->insertRow(row);
            ui->tableWidgetNetBits->setItem(row,0,new QTableWidgetItem(QString("  ") + var.getGroup() + "  "));
            ui->tableWidgetNetBits->setItem(row,1,new QTableWidgetItem(QString("  ") + var.getName() + "  "));
            ui->tableWidgetNetBits->setItem(row,2,new QTableWidgetItem(QString("  ") + var.getComment() + "  "));

            QComboBox *box = new QComboBox();
            box->setStyleSheet("background-color: white;");
            for(auto state:states) {
                box->addItem(state.first);
                box->addItem(state.second);
            }
            connect(box,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,row](int index){
                QComboBox *box = dynamic_cast<QComboBox*>(ui->tableWidgetNetBits->cellWidget(row,4));
                if(box) {
                    if(index&0x01) index--;else index++;
                    box->setCurrentIndex(index);
                }
            });
            box->setCurrentText("OPEN");
            ui->tableWidgetNetBits->setCellWidget(row,3,box);

            box = new QComboBox();
            box->setStyleSheet("background-color: white;");
            for(auto state:states) {
                box->addItem(state.first);
                box->addItem(state.second);
            }
            box->setCurrentText("CLOSED");
            ui->tableWidgetNetBits->setCellWidget(row,4,box);

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
        }
    }
    ui->tableWidgetNetBits->resizeColumnToContents(0);
    ui->tableWidgetNetBits->resizeColumnToContents(1);
    ui->tableWidgetNetBits->resizeColumnToContents(2);
    ui->tableWidgetNetBits->resizeColumnToContents(3);
    ui->tableWidgetNetBits->resizeColumnToContents(4);
}

void DialogConfigScadaMapVars::createNetRegsTable()
{
    std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups("Сетевые регистры");
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
        }
    }
}

void DialogConfigScadaMapVars::createTelemetryBitsTable()
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Биты","Скада");
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
    }
}

void DialogConfigScadaMapVars::createTelemetryRegsTable()
{
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Регистры","Скада");
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
    }
}

DialogConfigScadaMapVars::DialogConfigScadaMapVars(const PLCConfig &plcConfig, QWidget *parent) :
    QDialog(parent),plcConfig(plcConfig),
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
        {"ВКЛ","ВЫКЛ"}
    };

    createDITable();
    createDOTable();
    createAITable();
    createClustBitsTable();
    createClustRegsTable();
    createNetBitsTable();
    createNetRegsTable();
    createTelemetryBitsTable();
    createTelemetryRegsTable();
}

DialogConfigScadaMapVars::~DialogConfigScadaMapVars()
{
    delete ui;
}
