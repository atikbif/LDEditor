#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QGraphicsGridLayout>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QScreen>
#include <QThread>
#include <QComboBox>
#include <QTreeWidget>

#include <QDebug>

#include "opencontact.h"
#include "closedcontact.h"
#include "plustwoinputs.h"
#include "multipletwoinputs.h"
#include "greaterelement.h"
#include "lesselement.h"
#include "greaterorequal.h"
#include "lessorequal.h"
#include "equalelement.h"
#include "notequalelement.h"
#include "variable.h"
#include "constvar.h"
#include "relay.h"
#include "relayenabled.h"
#include "minuselement.h"
#include "divideelement.h"
#include "rstrigger.h"
#include "srtrigger.h"
#include "delayon.h"
#include "delayoff.h"
#include "commentelement.h"
#include "notelement.h"
#include "libraryelement.h"
#include "elementlibrary.h"
#include "counterelement.h"

#include <QScrollBar>
#include <QLineEdit>
#include <QMessageBox>

#include "plcvarcontainer.h"
#include "dialogvarconfig.h"

#include "serialize.h"
#include <QFileDialog>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include "SourceBuilder/connectionfinder.h"
#include "SourceBuilder/compiler.h"

#include <QDateTime>
#include <QPushButton>
#include <QTimer>
#include "Checker/ldchecker.h"
#include "Loader/plcfinder.h"
#include "SourceBuilder/plcparams.h"
#include "SourceBuilder/finder.h"
#include "Dialogs/dialogprojectconfig.h"

#include "portconfig.h"
#include "plcutils.h"
#include <QWidgetAction>
#include "dialogplcconfig.h"
#include "dialogcycleconfig.h"
#include "dialogadcconfig.h"
#include "dialogdiconfig.h"
#include "dialogdoconfig.h"
#include "Modbus/dialogmodbusmaster.h"

#include <QSettings>
#include <QtConcurrent/QtConcurrent>

#include "dialogsearchvar.h"
#include "dialogmapcreator.h"
#include "mapcontent.h"
#include "mapcreator.h"

#include <QMessageBox>
#include <QDateTime>
#include <dialogapplicationconfig.h>
#include "dialogvarinitlist.h"
#include "dialogconfigscadamapvars.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *horToolBar = new QToolBar(this);
    addToolBar(Qt::ToolBarArea::TopToolBarArea,horToolBar);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();

    page.column_count = 10;
    page.row_count = 49;
    page.cell_width = round(screenGeometry.width()*0.95/qreal(page.column_count+2));
    page.cell_height = round(page.cell_width/3);
    while(static_cast<int>(page.cell_height)%4) page.cell_height++;
    while(static_cast<int>(page.cell_width)%4) page.cell_width++;

    for(int i=0;i<page_count;i++) {
        auto *sc = new LDScene(page);
        auto *st = new CmdStack(sc);
        sc->setStack(st);
        sc->setCopyBuf(new CopyBufItem(page.cell_width,page.cell_height));
        sc->empty_page();
        prPages.push_back(std::make_pair(sc,st));
    }


    scene = prPages[0].first;


    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setScene(scene);

    connect(ui->graphicsView->horizontalScrollBar(),&QScrollBar::valueChanged,scene,[this](){scene->update(scene->sceneRect());});
    connect(ui->graphicsView->verticalScrollBar(),&QScrollBar::valueChanged,scene,[this](){scene->update(scene->sceneRect());});

    connect(ui->graphicsView->verticalScrollBar(),&QScrollBar::actionTriggered,scene,[this](int value){
        if(value==QAbstractSlider::SliderMove) {
            if(ui->graphicsView->verticalScrollBar()->minimum()==ui->graphicsView->verticalScrollBar()->value()) {
                if(otherPage==0) {
                    otherPage=12;
                    if(page_num) {
                        page_num--;
                        pageNumWidget->setValue(static_cast<int>(page_num+1));
                    }
                }else otherPage--;
            }else if(ui->graphicsView->verticalScrollBar()->maximum()==ui->graphicsView->verticalScrollBar()->value()) {
                if(otherPage==0) {
                    otherPage=12;
                    if(page_num<prPages.size()-1) {
                        page_num++;
                        pageNumWidget->setValue(static_cast<int>(page_num+1));
                    }
                }else otherPage--;
            }

        }
    });


    setWindowState(Qt::WindowMaximized);

    stack = prPages[0].second;

    ui->toolBar->setAllowedAreas(Qt::ToolBarArea::LeftToolBarArea);
    ui->toolBar->addAction(QIcon(":/images/arrow.png"),"указатель",[this](){scene->setInsertElement(nullptr);});
    ui->toolBar->addAction(QIcon(":/images/line.png"),"Соединения",[this](){scene->setLineInsertMode();});
    ui->toolBar->addAction(QIcon(":/images/comment.png"),"Комментарий",[this](){auto *item = new CommentElement(page.cell_width,page.cell_height,scene->getColumnCount());scene->setInsertElement(item);});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/open_contact_icon.png"),"разомкнутый контакт",[this](){auto *contact = new OpenContact(page.cell_width,page.cell_height);scene->setInsertElement(contact);});
    ui->toolBar->addAction(QIcon(":/images/closed_contact_icon.png"),"замкнутый контакт",[this](){auto *contact = new ClosedContact(page.cell_width,page.cell_height);scene->setInsertElement(contact);});
    ui->toolBar->addAction(QIcon(":/images/variable_icon.svg"),"переменная",[this](){auto *item = new Variable(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/const_var_icon.svg"),"константа",[this](){auto *item = new ConstVar(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/rele_icon.svg"),"реле",[this](){auto *item = new Relay(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/rele_enable_icon.svg"),"реле с разрешением",[this](){auto *item = new RelayEnabled(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/not_icon.svg"),"NOT",[this](){auto *item = new NotElement(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction("RS",[this](){auto *item = new RSTrigger(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction("SR",[this](){auto *item = new SRTrigger(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction("CTR",[this](){auto *item = new CounterElement(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/delayOnIcon.svg"),"Delay ON",[this](){auto *item = new DelayOn(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/delayOffIcon.svg"),"Delay OFF",[this](){auto *item = new DelayOff(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/add.png"),"Сложение",[this](){auto *plus = new PlusTwoInputs(page.cell_width,page.cell_height);scene->setInsertElement(plus);});
    ui->toolBar->addAction(QIcon(":/images/minus.png"),"Вычитание",[this](){auto *item = new MinusElement(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/multiple.png"),"Умножение",[this](){auto *item = new MultipleTwoInputs(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addAction(QIcon(":/images/divide.png"),"Деление",[this](){auto *item = new DivideElement(page.cell_width,page.cell_height);scene->setInsertElement(item);});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/greater.png"),"Больше",[this](){auto *el = new GreaterElement(page.cell_width,page.cell_height);scene->setInsertElement(el);});
    ui->toolBar->addAction(QIcon(":/images/less.png"),"Больше",[this](){auto *el = new LessElement(page.cell_width,page.cell_height);scene->setInsertElement(el);});
    ui->toolBar->addAction(QIcon(":/images/greater_or_equal.png"),"Больше или равно",[this](){auto *el = new GreaterOrEqual(page.cell_width,page.cell_height);scene->setInsertElement(el);});
    ui->toolBar->addAction(QIcon(":/images/less_or_equal.png"),"Меньше или равно",[this](){auto *el = new LessOrEqual(page.cell_width,page.cell_height);scene->setInsertElement(el);});
    ui->toolBar->addAction(QIcon(":/images/equal.png"),"равенство",[this](){auto *el = new EqualElement(page.cell_width,page.cell_height);scene->setInsertElement(el);});
    ui->toolBar->addAction(QIcon(":/images/not_equal.png"),"неравенство",[this](){auto *el = new NotEqualElement(page.cell_width,page.cell_height);scene->setInsertElement(el);});
    ui->toolBar->addSeparator();


    horToolBar->addAction(QIcon(":/images/open.png"),"Открыть",this,&MainWindow::openProject);
    horToolBar->addAction(QIcon(":/images/save.png"),"Сохранить",this,&MainWindow::saveProject);
    horToolBar->addSeparator();

    auto undoAct = horToolBar->addAction(QIcon(":/images/undo.png"),"Отменить",[this](){stack->undo();});
    undoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    addAction(undoAct);

    auto redoAct = horToolBar->addAction(QIcon(":/images/redo.png"),"Повторить",[this](){stack->redo();});
    redoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    addAction(redoAct);
    horToolBar->addSeparator();

    horToolBar->addAction(QIcon(":/images/zoom_in.png"),"Приблизить",[this](){if(zoomLevel<3) {zoomLevel++;ui->graphicsView->scale(1.25,1.25);this->update();}});
    horToolBar->addAction(QIcon(":/images/zoom_out.png"),"Отдалить",[this](){if(zoomLevel>-5) {zoomLevel--;ui->graphicsView->scale(0.8,0.8);this->update();}});


    horToolBar->addSeparator();
    horToolBar->addAction(QIcon(":/images/variable.png"),"Конфигурация переменных",[this](){auto *dialog = new DialogVarConfig();if(dialog->exec()==QDialog::Accepted) prChanged=true;delete dialog;});
    horToolBar->addSeparator();
    buildAction = horToolBar->addAction(QIcon(":/images/compile2.png"),"Собрать проект",this,&MainWindow::build);

    horToolBar->addSeparator();
    horToolBar->addAction(QIcon(":/images/program_flash.png"),"Загрузить программу в ПЛК",[this](){
        QString binFileName = prDir  + "prog.bin";
        if(QFile::exists(binFileName)){
            portConfig portConf;
            portConf.netAddress = netAddr;
            portConf.baudrate = baudrate;
            portConf.parity = parity;
            portConf.stopBits = stopBits;
            portConf.ip = progIP;
            if(PLCUtils::isPLCSupportEth(plcType->currentText())) portConf.isEthDefault = ethAsDefault;
            else portConf.isEthDefault = false;
            PLCFinder *finder=new PLCFinder(plcType->currentText(),"",portConf,binFileName);Q_UNUSED(finder)
            //QTimer::singleShot(70000,this,[finder](){delete finder;});
        }else {
            QString info = "Необходимо скомпилировать проект.\nНе найден бинарный файл программы:\n" + binFileName;
            QMessageBox::information(this,tr("Сообщение"),tr(info.toStdString().c_str()));
        }
    });
    horToolBar->addSeparator();

    configAction = horToolBar->addAction(QIcon(":/images/read_write_config.png"),"Настройки ПЛК",this,&MainWindow::readWriteConfig);

    horToolBar->addSeparator();

    modbusAction = horToolBar->addAction(QIcon(":/images/modbus.ico"),"Modbus настройка",[this](){
        auto *dialog = new DialogModbusMaster(this);
        if(dialog->exec()==QDialog::Accepted) {
        }
    });

    horToolBar->addSeparator();

    pageNumWidget = new QSpinBox(this);

    QString stSheet = "QSpinBox {"
            "border: 1px solid #ABABAB;"
            "border-radius: 12px;"
            "height: 24px;"
            "width: 48px;"
          "}"

          "QSpinBox::down-button  {"
            "subcontrol-origin: margin;"
            "subcontrol-position: center left;"
            "image: url(:/images/left.png);"
            "left: 1px;"
            "height: 24px;"
            "width: 24px;"
          "}"

          "QSpinBox::up-button  {"
            "subcontrol-origin: margin;"
            "subcontrol-position: center right;"
            "image: url(:/images/right.png);"
            "right: 1px;"
            "height: 24px;"
            "width: 24px;"
          "}";


    pageNumWidget->setStyleSheet(stSheet);
    pageNumWidget->setAlignment(Qt::AlignHCenter);


    pageNumWidget->setValue(1);
    pageNumWidget->setMinimum(1);
    pageNumWidget->setMaximum(page_count);
    connect(pageNumWidget,QOverload<int>::of(&QSpinBox::valueChanged),[this](int value){
        page_num = static_cast<std::size_t>(value)-1;
        scene = prPages.at(page_num).first;
        stack = prPages.at(page_num).second;
        ui->graphicsView->setScene(scene);
        ui->graphicsView->ensureVisible(0,0,10,10);
        this->update();
    });
    horToolBar->addWidget(new QLabel("Страница: "));
    horToolBar->addWidget(pageNumWidget);
    maxPageNumWidget = new QLabel("  из "+QString::number(page_count));
    horToolBar->addWidget(maxPageNumWidget);

    horToolBar->addSeparator();
    horToolBar->addWidget(new QLabel("ПЛК: "));
    plcType = new QComboBox();

    std::vector<QString> plcNames = PLCParams::readPLCNames();
    for(const QString &plcName:plcNames) {
        plcType->addItem(plcName);
    }
    if(!plcNames.empty()) plcChanged(plcType->currentText());

    horToolBar->addWidget(plcType);
    connect(plcType,&QComboBox::currentTextChanged,this,&MainWindow::plcChanged);
    horToolBar->addSeparator();

    horToolBar->addAction(QIcon(":/images/search.ico"),"Поиск",[this](){search();});
    horToolBar->addSeparator();
    horToolBar->addAction(QIcon(":/images/print.png"),"Печать",this,&MainWindow::previewAction);
    horToolBar->addSeparator();



    /*QComboBox *box = new QComboBox();
    for(int i=1;i<100;i++) {
        box->addItem(QString::number(i));
    }
    connect(box, QOverload<const QString&>::of(&QComboBox::activated),
        [=](const QString &text){ auto *item = new LibraryElement(page.cell_width,page.cell_height,text);scene->setInsertElement(item);});
    horToolBar->addWidget(new QLabel("Библиотека: "));
    horToolBar->addWidget(box);*/


    ElementLibrary *lib = new ElementLibrary();
    auto libFolders = lib->getFolders();
    for(const QString &folder:libFolders) {
        QTreeWidgetItem *folderItem = new QTreeWidgetItem({folder});
        auto elements = lib->getElementsByFolder(folder);
        for(const QString &elFileName:elements) {
            QTreeWidgetItem *item = new QTreeWidgetItem({elFileName});
            folderItem->addChild(item);
        }
        ui->treeWidgetLibrary->addTopLevelItem(folderItem);
    }
    /*QTreeWidgetItem *item1 = new QTreeWidgetItem({"арифметика"});
    QTreeWidgetItem *item2 = new QTreeWidgetItem({"логика"});
    ui->treeWidgetLibrary->addTopLevelItem(item1);
    ui->treeWidgetLibrary->addTopLevelItem(item2);
    for(int i=1;i<5;i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem({QString::number(i)});
        item1->addChild(item);
    }
    for(int i=1;i<8;i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem({QString::number(i)});
        item2->addChild(item);
    }*/
    connect(ui->treeWidgetLibrary,&QTreeWidget::itemClicked,[this,lib](QTreeWidgetItem *item, int column){
        Q_UNUSED(column)
        if(item->childCount()==0) {
            QString elFile = lib->getFileByElementName(item->text(0));
            if(!elFile.isEmpty()) {
                auto *lditem = new LibraryElement(page.cell_width,page.cell_height,elFile);
                scene->setInsertElement(lditem);
            }
        }
    });

    ui->graphicsView->ensureVisible(0,0,10,10);

    ui->statusbar->addWidget(new QLabel("строка:"));
    mouseRow = new QLineEdit("");
    mouseRow->setFixedHeight(24);
    mouseRow->setReadOnly(true);
    ui->statusbar->addWidget(mouseRow,0);

    ui->statusbar->addWidget(new QLabel("колонка:"));
    mouseCol = new QLineEdit("");
    mouseCol->setFixedHeight(24);
    mouseCol->setReadOnly(true);
    ui->statusbar->addWidget(mouseCol,0);

    ui->statusbar->addWidget(new QLabel("имя: "));

    itemName = new QLineEdit("");
    itemName->setFixedHeight(24);
    itemName->setReadOnly(true);
    ui->statusbar->addWidget(itemName,1);

    ui->statusbar->addWidget(new QLabel("тип: "));
    itemType = new QLineEdit("");
    itemType->setFixedHeight(24);
    itemType->setReadOnly(true);
    ui->statusbar->addWidget(itemType,1);

    ui->statusbar->addWidget(new QLabel("примечание: "));
    itemComment = new QLineEdit("");
    itemComment->setFixedHeight(24);
    itemComment->setReadOnly(true);
    ui->statusbar->addWidget(itemComment,1);

    ui->statusbar->addWidget(new QLabel("привязка: "));
    itemInfo = new QLineEdit("");
    itemInfo->setFixedHeight(24);
    itemInfo->setReadOnly(true);
    ui->statusbar->addWidget(itemInfo,2);

    buttonMin = new QPushButton(QIcon(":/images/minimize.png"),"");
    //buttonMax = new QPushButton(QIcon(":/images/maximize.png"),"");
    ui->statusbar->addWidget(buttonMin);
    //ui->statusbar->addWidget(buttonMax);
    connect(buttonMin,&QPushButton::clicked,[this](){
        if(ui->textBrowser->isVisible()) {ui->textBrowser->setVisible(false);}
        else {ui->textBrowser->setVisible(true);}
    });
    //connect(buttonMax,&QPushButton::clicked,[this](){ui->textBrowser->setVisible(true);});
    ui->textBrowser->setVisible(false);

    for(std::pair<LDScene*,CmdStack*> p:prPages) { connectScene(p.first); }




    fileMenu = new QMenu("Файл");
    auto saveAct = fileMenu->addAction(QIcon(":/images/save.png"),"Сохранить",this,&MainWindow::saveProject);
    saveAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    addAction(saveAct);
    fileMenu->addAction(QIcon(":/images/save.png"),"Сохранить как",this,&MainWindow::saveAsProject);
    auto openAct = fileMenu->addAction(QIcon(":/images/open.png"),"Открыть",this,&MainWindow::openProject);
    openAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    addAction(openAct);
    ui->menubar->addMenu(fileMenu);

    QMenu *viewMenu = new QMenu("Вид");
    QMenu *background = new QMenu("Фон");
    viewMenu->addMenu(background);
    ui->menubar->addMenu(viewMenu);

    background->addAction(QIcon(":/images/white.png"),"",[this](){for(auto &page:prPages){page.first->setBackgroundBrush(QColor(0xFF,0xFF,0xFF));}});
    background->addAction(QIcon(":/images/orange.png"),"",[this](){for(auto &page:prPages){page.first->setBackgroundBrush(QColor(0xFA,0xFA,0xCC));}});
    background->addAction(QIcon(":/images/grey.png"),"",[this](){for(auto &page:prPages){page.first->setBackgroundBrush(QColor(0xEE,0xEE,0xEE));}});
    background->addAction(QIcon(":/images/green.png"),"",[this](){for(auto &page:prPages){page.first->setBackgroundBrush(QColor(0xE5,0xFA,0xD9));}});
    background->addAction(QIcon(":/images/blue.png"),"",[this](){for(auto &page:prPages){page.first->setBackgroundBrush(QColor(0xE0,0xF5,0xFD));}});
    background->addAction(QIcon(":/images/red.png"),"",[this](){for(auto &page:prPages){page.first->setBackgroundBrush(QColor(0xFA,0xDF,0xCC));}});

    nameAction = new QAction("показать имена");
    nameAction->setCheckable(true);
    connect(nameAction,&QAction::triggered,[this](){for(auto &page:prPages) {page.first->setNamesFlag(nameAction->isChecked());}});
    viewMenu->addAction(nameAction);

    QAction *libAction = new QAction("показать библиотеку");
    libAction->setCheckable(true);
    connect(libAction,&QAction::triggered,[this,libAction](){ui->treeWidgetLibrary->setVisible(libAction->isChecked());});
    libAction->setChecked(true);
    viewMenu->addAction(libAction);

    QMenu *confMenu = new QMenu("Настройки");
    confMenu->addAction(QIcon(":/images/plc_config.ico"),"Линия связи",[this](){
        auto *dialog = new DialogProjectConfig(plcType->currentText(),this);
        dialog->setBaudrate(baudrate);
        dialog->setNetAddress(netAddr);
        dialog->setParity(parity);
        dialog->setStopBits(stopBits);
        dialog->setIP(progIP);
        dialog->setIPasDefault(ethAsDefault);
        if(dialog->exec()==QDialog::Accepted) {
            if(progIP != dialog->getIP()) prChanged = true;
            baudrate = dialog->getBaudrate();
            netAddr = dialog->getNetAddress();
            parity = dialog->getParity();
            stopBits = dialog->getStopBits();
            progIP = dialog->getIP();
            ethAsDefault = dialog->useIPasDefault();
        }
    });
    confMenu->addAction(QIcon(":/images/clock.png"),"Системный цикл",[this](){
        auto *dialog = new DialogCycleConfig(prDelay,this);
        if(dialog->exec()==QDialog::Accepted) {
            prDelay = dialog->getDelay();
        }
    });

    confMenu->addAction(configAction);

    applConfigAction = confMenu->addAction(QIcon(":/images/appl_config.png"),"Настройки приложения",[this](){
        auto *dialog = new DialogApplicationConfig(plcConfig, this);
        if(dialog->exec()==QDialog::Accepted) {
            plcConfig.setAppName(dialog->getApplicationName());
            plcConfig.setApplicationCN(static_cast<quint16>(dialog->getAppCN()));
            plcConfig.setAppVersion(dialog->getApplicationVersion());
            plcConfig.setNodeNum(dialog->getNodeNum());
            plcConfig.setClusterNum(dialog->getClusterNum());
        }
    });

    configADCAction = confMenu->addAction(QIcon(":/images/analogue.png"),"Аналоговые входы",[this](){
        auto *dialog = new DialogADCconfig(plcConfig, this);
        if(dialog->exec()==QDialog::Accepted) {
            std::vector<int> result = dialog->getSensorTypes();
            for(int i=0;i<result.size();i++) {
                plcConfig.setSensorType(i,result.at(i));
            }
            std::vector<AINSensor> sensors = dialog->getAINSensors();
            plcConfig.clearADCSensors();
            for(AINSensor sensor:sensors) plcConfig.addADCSensor(sensor);
            prChanged = true;
        }
    });

    configDIAction = confMenu->addAction(QIcon(":/images/open_contact.png"),"Дискретные входы",[this](){
        auto *dialog = new DialogDIConfig(this);
        std::vector<QString> inputs;
        auto vars = PLCVarContainer::getInstance().getVarsByGroup("состояние","Дискретные входы");
        for(auto var:vars) inputs.push_back(var.getComment());
        dialog->setInputs(inputs);
        if(dialog->exec()==QDialog::Accepted) {
            inputs = dialog->getInputs();
            if(inputs.size()>=vars.size()) {
                for(int i=0;i<vars.size();i++) {
                    PLCUtils::updateSystemVarComment(vars.at(i).getName(),"состояние",inputs.at(i),"Дискретные входы");
                }
            }
            prChanged = true;
        }
    });

    configDOAction = confMenu->addAction(QIcon(":/images/rele.svg"),"Дискретные выходы",[this](){
        auto *dialog = new DialogDOConfig(this);
        std::vector<QString> outs;
        auto vars = PLCVarContainer::getInstance().getVarsByGroup("Дискретные выходы");
        for(auto var:vars) outs.push_back(var.getComment());
        dialog->setOuts(outs);
        if(dialog->exec()==QDialog::Accepted) {
            outs = dialog->getOuts();
            if(outs.size()>=vars.size()) {
                for(int i=0;i<vars.size();i++) {
                    PLCVarContainer::getInstance().updateComment("Дискретные выходы",vars.at(i).getName(),outs.at(i));
                }
            }
            prChanged = true;
        }
    });

    confMenu->addAction(modbusAction);

    ui->menubar->addMenu(confMenu);


    QMenu *cmdMenu = new QMenu("Операции");
    cmdMenu->addAction(QIcon(":/images/update_names.png"),"Обновить имена",[this](){
        std::vector<LDScene*> scenes;
        for(auto &prPage:prPages) scenes.push_back(prPage.first);
        for(LDScene *scene:scenes) {
            std::vector<LDElement*> elements = scene->getAllelements();
            for(LDElement *el:elements) {
                QString varName = el->connectedVar.name;
                if(!varName.isEmpty()) {
                    std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(el->connectedVar.group, varName,el->connectedVar.parentGroup);
                    if(v && (!v->getComment().isEmpty())) {
                        el->setName(v->getComment());
                    }
                }
            }
            scene->update();
        }
    });

    createMapAction = cmdMenu->addAction(QIcon(":/images/map.png"),"Создать карту памяти",[this](){

        QByteArray scadaMap = plcConfig.getScadaMap();
        std::vector<DialogConfigScadaMapVars::VarDescription> vars = DialogConfigScadaMapVars::scadaMapFromBytes(scadaMap);

        std::vector<QString> prVars;

        std::vector<LDScene*> scenes;
        for(auto &prPage:prPages) scenes.push_back(prPage.first);
        for(LDScene *scene:scenes) {
            std::vector<LDElement*> elements = scene->getAllelements();
            for(LDElement *el:elements) {
                if(el->isNeedConnect()) {
                    auto var = el->connectedVar;
                    QString varName = var.name;
                    QString varGroup = var.group;
                    QString varParentGroup = var.parentGroup;
                    auto plcVar = PLCVarContainer::getInstance().getVarByGroupAndName(varGroup,varName,varParentGroup);
                    if(plcVar && plcVar->isSystem()) {
                        prVars.push_back(varName.trimmed());
                    }
                }
            }
        }

        auto *dialog = new DialogConfigScadaMapVars(plcConfig, vars, prVars, this);
        MapContent content;
        content.setAppName(plcConfig.getAppName());
        content.setClusterNum(plcConfig.getClusterNum());
        content.setNodeNum(plcConfig.getNodeNum());
        content.setAppVersion(plcConfig.getAppVersion());
        content.setAppCN(plcConfig.getApplicationCN());
        content.setIp(plcConfig.getIP());
        content.setMask(plcConfig.getIPMask());
        content.setGateway(plcConfig.getIPGate());
        content.setAppTime(plcConfig.getAppTime());

        MapContent::ClusterGlobalBit bit;
        bit.name = "Node" + QString::number(plcConfig.getNodeNum());
        bit.nodeNum = 8;
        bit.trueName = "Online";
        bit.falseName = "Offline";
        bit.channelNum = plcConfig.getNodeNum() + 1;
        bit.bitNum = 0; // PC21-1
        content.addClusterGlobalBit(bit);
        bit.name = "Node 7";
        bit.nodeNum = 8;
        bit.trueName = "Online";
        bit.falseName = "Offline";
        bit.channelNum = 8;
        bit.bitNum = 7; // PC21-FE
        content.addClusterGlobalBit(bit);

        if(dialog->exec()==QDialog::Accepted) {
            auto checkVars = dialog->getCheckedVars();
            if(checkVars.size() != vars.size()) prChanged = true;

            plcConfig.setScadaMap(DialogConfigScadaMapVars::scadaMapToBytes(checkVars));

            qDebug() << "SIZE" << checkVars.size();
            for(const auto &var:checkVars) {
                QString vName = var.varName.trimmed();
                qDebug() << "NAME" << vName;
                auto v = PLCVarContainer::getInstance().getVarByName(vName);
                if(v && v->isSystem()) {
                    qDebug() << "ADDED" << v->getName();
                    QRegExp aiExp("^AI(\\d+)$");
                    if(aiExp.indexIn(vName)!=-1) {
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
                            MapContent::AnalogInput inp;
                            inp.tdu = tdu;
                            inp.name = v->getComment();
                            inp.nodeNum = plcConfig.getNodeNum();
                            inp.inputNum = num;
                            inp.channelNum = num;
                            content.addAnalogInput(inp);
                        }
                    }else content.addVar(v.value(),var.properties);
                }
            }
            MapCreator creator(content);
            bool res = creator.createFile(prDir + "/" + plcConfig.getAppName());

            bool isSysInfoVisible = ui->textBrowser->isVisible();
            if(!isSysInfoVisible) ui->textBrowser->setVisible(true);
            if(res) {
                QTimer::singleShot(2000, this, [this](){buttonMin->click();});
                ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Файл " + plcConfig.getAppName() + ".xslx успешно создан");
            }else ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Ошибка записи файла карты памяти. Возможно файл открыт или нет прав на запись.");
        }

//        for(int i=0;i<8;i++) {
//            MapContent::NetGlobalBit bit;
//            bit.bitNum = i;
//            bit.nodeNum = 0;
//            content.addNetGlobalBit(bit);
//        }

//        std::vector<LDScene*> scenes;
//        for(auto &prPage:prPages) scenes.push_back(prPage.first);
//        for(LDScene *scene:scenes) {
//            std::vector<LDElement*> elements = scene->getAllelements();
//            for(LDElement *el:elements) {
//                if(el->isNeedConnect()) {
//                    auto var = el->connectedVar;
//                    QString varName = var.name;
//                    QString varGroup = var.group;
//                    QString varParentGroup = var.parentGroup;
//                    auto plcVar = PLCVarContainer::getInstance().getVarByGroupAndName(varGroup,varName,varParentGroup);
//                    if(plcVar && plcVar->isSystem()) {
//                        QRegExp aiExp("^AI(\\d+)$");
//                        if(aiExp.indexIn(varName)!=-1) {

//                            int num = aiExp.cap(1).toInt();
//                            int sensCount = plcConfig.getADCSensorsCount();
//                            int sensNum = plcConfig.getSensorType(num-1);
//                            if(sensNum && sensNum<=sensCount) {

//                                auto sens = plcConfig.getADCSensor(sensNum-1);
//                                QString tdu = sens.getName() + "\t" + sens.getLowLimit() + "-" + sens.getHighLimit();
//                                QString measUnit = sens.getUnit();
//                                if(!measUnit.isEmpty()) tdu += " " + measUnit;
//                                int sensType = plcConfig.getSensorTypeCode(num-1);
//                                switch(sensType) {
//                                    case 0: tdu+=" (0.4-2В)";break;
//                                    case 1: tdu+=" (0-20мА)";break;
//                                    case 2: tdu+=" (4-20мА)";break;
//                                    case 3: tdu+=" (2-10мА)";break;
//                                    case 4:tdu+=" (0-2.5В)";break;
//                                }
//                                MapContent::AnalogInput inp;
//                                inp.tdu = tdu;
//                                inp.name = plcVar->getComment();
//                                inp.nodeNum = plcConfig.getNodeNum();
//                                inp.inputNum = num;
//                                inp.channelNum = num;
//                                content.addAnalogInput(inp);
//                            }
//                        }else {
//                            content.addVar(plcVar.value());
//                        }

//                    }
//                }
//            }
//        }

    });

    cmdMenu->addAction(QIcon(":/images/list.png"),"Иниц. переменные",[this](){
        auto *dialog = new DialogVarInitList(this);
        auto parGroups = PLCVarContainer::getInstance().getParentGroups();
        for(const QString &parGroup:parGroups) {
            std::vector<QString> groups = PLCVarContainer::getInstance().getVarGroups(parGroup);
            for(const QString &gr:groups) {
                for(const auto & var:PLCVarContainer::getInstance().getVarsByGroup(gr,parGroup)) {
                    if(var.getValueAsString()!="0") {
                        dialog->addVar(var.getName(), var.getComment(), var.getValueAsString());
                    }
                }
            }
        }
        dialog->exec();

    });

    ui->menubar->addMenu(cmdMenu);


    prDir = QCoreApplication::applicationDirPath() + "/new_project/";
    prFileName = QCoreApplication::applicationDirPath() + "/new_project/default.ldp";
    setWindowTitle(prFileName);

    ui->graphicsView->scale(0.8,0.8);

    QStringList prevPr = getPrevProjects();
    updatePrevProjects(prevPr);

    update();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stack;
}

void MainWindow::openPrevProject()
{
    QAction *a = qobject_cast<QAction *>(sender());
    if(a!=nullptr) {
        if(prChanged) {
            if (QMessageBox::Yes == QMessageBox::question(this,
                                                          tr("LD Редактор"),
                                                          tr("Текущий проект был изменён.\nВы хотите его сохранить?"))) {
                saveProject();
            }
        }
        setCellConfig(page.cell_width,page.cell_height,scene->getColumnCount());
        QString fName = a->text();
        openProjectByName(fName);
    }
}

void MainWindow::searchVar(const QString &grName, const QString &varName)
{
    auto *dialog = new DialogSearchVar(grName, varName);

    connect(dialog, &DialogSearchVar::goToElement,[this](int pageNum, int row, int col){
        if(pageNum<=prPages.size() && pageNum) {
            pageNumWidget->setValue(pageNum);
            LDScene* scene = prPages.at(pageNum-1).first;
            scene->setLastRow(0);
            scene->setLastCol(0);
            std::vector<LDElement*> elements = scene->getAllelements();
            for(LDElement *el:elements) {
                if(el->getRowNum()==row && el->getColNum()==col) {
                    el->getItem()->ensureVisible();
                    scene->update();
                    break;
                }
            }
        }
    });

    std::vector<LDScene*> scenes;
    for(auto &prPage:prPages) scenes.push_back(prPage.first);
    int pageNum=1;
    for(LDScene *scene:scenes) {
        scene->setLastRow(0);
        scene->setLastCol(0);
        std::vector<LDElement*> elements = scene->getAllelements();
        for(LDElement *el:elements) {
            el->setSelected(false);
            if(el->connectedVar.group==grName && el->connectedVar.name==varName) {
                el->setSelected(true);
                LDInfo ldInfo(*el);
                dialog->addElement(pageNum, el->getRowNum(),el->getColNum(),ldInfo.type);
            }
        }
        scene->update();pageNum++;
    }

    dialog->exec();
    delete dialog;
}

QStringList MainWindow::getPrevProjects()
{
    QStringList res;
    QSettings settings;
    res = settings.value("/Settings/PrevProjects",QStringList()).toStringList();
    for(int i=0;i<res.count();i++) {
        QString s = res.at(i);
        s.replace('\\','/');
        res[i] = s;
    }
    res.removeDuplicates();
    if(res.count()>maxAmountOfPrevProjects) res = res.mid(0,maxAmountOfPrevProjects);
    settings.setValue("/Settings/PrevProjects",res);
    return res;
}

void MainWindow::updatePrevProjects(const QStringList &prNames)
{
    QSettings settings;
    settings.setValue("/Settings/PrevProjects",prNames);
    QStringList resList = getPrevProjects();
    fileMenu->clear();
    fileMenu->addAction(QIcon(":/images/save.png"),"Сохранить",this,&MainWindow::saveProject);
    fileMenu->addAction(QIcon(":/images/save.png"),"Сохранить как",this,&MainWindow::saveAsProject);
    fileMenu->addAction(QIcon(":/images/open.png"),"Открыть",this,&MainWindow::openProject);

    QMenu* recPr = new QMenu("Недавние проекты");
    fileMenu->addMenu(recPr);
    foreach(QString name, resList) {
        //ui->menu->addAction(name,this,SLOT(openPrevProject()));
        recPr->addAction(name,this,SLOT(openPrevProject()));
    }
}

void MainWindow::createPageBefore()
{
    auto iterator = prPages.begin()+static_cast<int>(page_num);

    auto *sc = new LDScene(page);
    auto *st = new CmdStack(sc);
    sc->setStack(st);
    sc->setCopyBuf(new CopyBufItem(page.cell_width,page.cell_height));
    sc->empty_page();
    sc->setNamesFlag(nameAction->isChecked());

    connectScene(sc);

    prPages.insert(iterator,std::make_pair(sc,st));
    page_count++;
    pageNumWidget->setMaximum(page_count);
    maxPageNumWidget->setText("  из "+QString::number(page_count));

    emit pageNumWidget->valueChanged(static_cast<int>(page_num+1));
    this->update();
    prChanged = true;
}

void MainWindow::createPageAfter()
{
    auto iterator = prPages.begin()+static_cast<int>(page_num);

    auto *sc = new LDScene(page);
    auto *st = new CmdStack(sc);
    sc->setStack(st);
    sc->setCopyBuf(new CopyBufItem(page.cell_width,page.cell_height));
    sc->empty_page();
    sc->setNamesFlag(nameAction->isChecked());

    connectScene(sc);

    iterator++;
    if(iterator!=prPages.end()) prPages.insert(iterator,std::make_pair(sc,st));
    else prPages.emplace_back(std::make_pair(sc,st));
    page_count++;
    page_num++;
    pageNumWidget->setMaximum(page_count);
    maxPageNumWidget->setText("  из "+QString::number(page_count));

    pageNumWidget->setValue(static_cast<int>(page_num+1));
    this->update();
    prChanged = true;
}

void MainWindow::deletePage(bool confirmation)
{
    if(prPages.size()>1) {
        if (!confirmation || (QMessageBox::Yes == QMessageBox::question(this,
                                                      tr("Подтверждение операции"),
                                                      tr("Вы действительно хотите удалить эту страницу?")))) {
            auto iterator = prPages.begin()+static_cast<int>(page_num);
            LDScene *delScene = iterator->first;
            CmdStack *delStack = iterator->second;
            auto nextIterator = iterator+1;
            if(nextIterator!=prPages.end()) {
                scene = nextIterator->first;
                stack = nextIterator->second;
                ui->graphicsView->setScene(scene);
                ui->graphicsView->ensureVisible(0,0,10,10);
                prPages.erase(iterator);
                page_count--;
                pageNumWidget->setMaximum(page_count);
                maxPageNumWidget->setText("  из "+QString::number(page_count));
                delete delScene;
                delete delStack;
                this->update();
            }else {
                page_num--;
                pageNumWidget->setValue(static_cast<int>(page_num+1));
                prPages.erase(iterator);
                page_count--;
                pageNumWidget->setMaximum(page_count);
                maxPageNumWidget->setText("  из "+QString::number(page_count));
                delete delScene;
                delete delStack;
                this->update();
            }
            prChanged = true;
        }
    }else {
        QMessageBox::information(this,tr("Сообщение"),tr("Невозможно удалить единственную страницу"));
    }
}

void MainWindow::updateInfo(const LDInfo &elInfo)
{
    itemName->setText(elInfo.name);
    itemType->setText(elInfo.type);
    itemInfo->setText(elInfo.info);
    itemComment->setText(elInfo.comment);
    mouseCol->setText(QString::number(elInfo.col));
    mouseRow->setText(QString::number(elInfo.row));
}

void MainWindow::saveProject()
{
    if(!prFileName.isEmpty()) {

        QStringList prevPr = getPrevProjects();
        prevPr.insert(0,prFileName);
        updatePrevProjects(prevPr);

        QFile file(prFileName);
        if(file.open(QIODevice::WriteOnly) ) {
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_13);
            out << QString("LD Project");
            auto pageCount = static_cast<int>(prPages.size());
            out << pageCount; // число страниц
            for(std::pair<LDScene*,CmdStack*> &page: prPages) {
                out << QString("Page");
                LDScene* sc = page.first;
                auto elCount = static_cast<int>(sc->getElementCount());
                out << elCount; // количество элементов на странице
                for(LDElement *el:sc->getAllelements()) out << *el;
            }
            savePLCVarContainer(out);
            out << plcType->currentText();
            out << prDelay;
            out << stopBits;
            out << baudrate;
            out << parity;
            out << netAddr;
            if(PLCUtils::isPLCSupportEth(plcType->currentText())) {
                out << progIP;
                out << ethAsDefault;
            }
            if(plcType->currentText()!="MKU") {
                plcConfig.setAppTime(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
                QByteArray config = plcConfig.toBytes();
                out << config;
            }
            if(PLCUtils::isPLCSupportModbusMaster(plcType->currentText())) {
                QByteArray config = ModbusVarsStorage::getInstance().toBytes();
                out << config;
            }
            if(PLCUtils::isPLCSupportCAN(plcType->currentText())) {
                int value = 0;
                out << value; // unused
            }
            prChanged = false;
            QFileInfo fInfo(file);
            prDir = fInfo.canonicalPath()+"/";
            file.close();
            bool isSysInfoVisible = ui->textBrowser->isVisible();
            if(!isSysInfoVisible) ui->textBrowser->setVisible(true);
            if(!isSysInfoVisible) QTimer::singleShot(1000, this, [this](){buttonMin->click();});
            ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Проект успешно сохранён");
            ui->textBrowser->repaint();
        }else {
            bool isSysInfoVisible = ui->textBrowser->isVisible();
            if(!isSysInfoVisible) ui->textBrowser->setVisible(true);
            if(!isSysInfoVisible) QTimer::singleShot(1000, this, [this](){buttonMin->click();});
            ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Ошибка открытия файла для записи");
            ui->textBrowser->repaint();
        }
    }
}

void MainWindow::saveAsProject()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить проект"),
                               prFileName,
                               tr("*.ldp"));
    if(!fileName.isEmpty()) {

        QStringList prevPr = getPrevProjects();
        prevPr.insert(0,fileName);
        updatePrevProjects(prevPr);

        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly) ) {
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_13);
            out << QString("LD Project");
            auto pageCount = static_cast<int>(prPages.size());
            out << pageCount; // число страниц
            for(std::pair<LDScene*,CmdStack*> &page: prPages) {
                out << QString("Page");
                LDScene* sc = page.first;
                auto elCount = static_cast<int>(sc->getElementCount());
                out << elCount; // количество элементов на странице
                for(LDElement *el:sc->getAllelements()) out << *el;
            }
            savePLCVarContainer(out);
            out << plcType->currentText();
            out << prDelay;
            out << stopBits;
            out << baudrate;
            out << parity;
            out << netAddr;
            if(PLCUtils::isPLCSupportEth(plcType->currentText())) {
                out << progIP;
                out << ethAsDefault;
            }
            if(plcType->currentText()!="MKU") {
                plcConfig.setAppTime(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
                QByteArray config = plcConfig.toBytes();
                out << config;
            }
            if(PLCUtils::isPLCSupportModbusMaster(plcType->currentText())) {
                QByteArray config = ModbusVarsStorage::getInstance().toBytes();
                out << config;
            }
            if(PLCUtils::isPLCSupportCAN(plcType->currentText())) {
                int value = 0;
                out << value; // unused
            }
            prChanged = false;
            prFileName = fileName;
            setWindowTitle(fileName);
            QFileInfo fInfo(file);
            prDir = fInfo.canonicalPath()+"/";
            file.close();
            bool isSysInfoVisible = ui->textBrowser->isVisible();
            if(!isSysInfoVisible) ui->textBrowser->setVisible(true);
            if(!isSysInfoVisible) QTimer::singleShot(1000, this, [this](){buttonMin->click();});
            ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Проект успешно сохранён");
            ui->textBrowser->repaint();
        }else {
            bool isSysInfoVisible = ui->textBrowser->isVisible();
            if(!isSysInfoVisible) ui->textBrowser->setVisible(true);
            if(!isSysInfoVisible) QTimer::singleShot(1000, this, [this](){buttonMin->click();});
            ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Ошибка открытия файла для записи");
            ui->textBrowser->repaint();
        }
    }
}

void MainWindow::openProjectByName(const QString &fName) {
    QStringList prevPr = getPrevProjects();
    prevPr.insert(0,fName);
    updatePrevProjects(prevPr);
    QFile file(fName);
    if(file.open(QIODevice::ReadOnly) ) {
        PLCVarContainer::getInstance().clearSysVarsComments();
        PLCVarContainer::getInstance().clearInitValues();
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_13);
        QString tmpStr;
        in >> tmpStr;
        if(tmpStr=="LD Project") {
            while(prPages.size()>1) deletePage(false);
            scene->empty_page();
            bool firstPage = true;
            int pageCount = 0;
            in >> pageCount;
            for(int pageNum=0;pageNum<pageCount;pageNum++) {
                in >> tmpStr;
                if(tmpStr=="Page") {
                    if(!firstPage) createPageAfter();
                    else {firstPage=false;}
                    int elCount = 0;
                    in >> elCount;
                    for(int i=0;i<elCount;i++) {
                        auto res = readLDelement(in);
                        LDElement *el = res.first;
                        if(el) scene->addElement(el,el->getColNum(),el->getRowNum());
                        else {
                            ui->textBrowser->append(res.second);
                        }
                    }
                }
            }
            pageNumWidget->setValue(1);
            stack->clear();

            readPLCVarContainer(in);
            QString plcName;
            in >> plcName;
            plcType->setCurrentText(plcName);

            for(auto page:prPages) {
                LDScene *sc = page.first;
                if(sc) {
                    auto elements = sc->getAllelements();
                    for(LDElement *el:elements) {
                        checkLDElement(el);
                    }
                }
            }

            in >> prDelay;
            in >> stopBits;
            in >> baudrate;
            in >> parity;
            in >> netAddr;
            if(PLCUtils::isPLCSupportEth(plcName)) {
                in >> progIP;
                in >> ethAsDefault;
            }
            if(plcType->currentText()!="MKU") {
                QByteArray config;
                in >> config;
                plcConfig.fromBytes(config);
            }else plcConfig = PLCConfig();
            if(PLCUtils::isPLCSupportModbusMaster(plcType->currentText())) {
                QByteArray config;
                in >> config;
                ModbusVarsStorage::getInstance().fromBytes(config);
            }
            if(PLCUtils::isPLCSupportCAN(plcType->currentText())) {
                // support previous versions
                int value = 0;
                in >> value;
                if(value) plcConfig.setApplicationCN(static_cast<quint16>(value));
            }
            plcChanged(plcName);
            setWindowTitle(fName);
            prChanged = false;
            prFileName = fName;

            QFileInfo fInfo(file);
            prDir = fInfo.canonicalPath()+"/";

            bool isSysInfoVisible = ui->textBrowser->isVisible();
            if(!isSysInfoVisible) ui->textBrowser->setVisible(true);
            if(!isSysInfoVisible) QTimer::singleShot(1000, this, [this](){buttonMin->click();});
            ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Проект открыт");
            ui->textBrowser->repaint();
        }
    }else {
        bool isSysInfoVisible = ui->textBrowser->isVisible();
        if(!isSysInfoVisible) ui->textBrowser->setVisible(true);
        if(!isSysInfoVisible) QTimer::singleShot(1000, this, [this](){buttonMin->click();});
        ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Ошибка открытия файла для чтения");
        ui->textBrowser->repaint();
    }
}

void MainWindow::openProject()
{
    if(prChanged) {
        if (QMessageBox::Yes == QMessageBox::question(this,
                                                      tr("LD Редактор"),
                                                      tr("Текущий проект был изменён.\nВы хотите его сохранить?"))) {
            saveProject();
        }
    }
    setCellConfig(page.cell_width,page.cell_height,scene->getColumnCount());
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть проект"),"",tr("*.ldp"));
    if(!fileName.isEmpty()) {
        openProjectByName(fileName);
    }
}

void MainWindow::printPreview(QPrinter *printer)
{

    printer->setPageSize(QPrinter::A4);
    printer->setOrientation(QPrinter::Portrait);
    printer->setFullPage(true);
    QPainter p;
    if( !p.begin(printer ) )
    {
        return;
    }

    int pageNum = 0;
    for(std::pair<LDScene*,CmdStack*> &page:prPages) {
        pageNum++;
        page.first->render(&p);
        if(static_cast<std::size_t>(pageNum)<prPages.size()) printer->newPage();
    }
    p.end();
}

void MainWindow::previewAction()
{
    QPrinter printer(QPrinter::HighResolution );
    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags ( Qt::Window );
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    preview.exec();
}

void MainWindow::build()
{
    ui->textBrowser->clear();
    ui->textBrowser->append("<b><font color=\"black\"></font></b>");
    //bool isSysInfoVisible = ui->textBrowser->isVisible();
    ui->textBrowser->setVisible(true);
    ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Проверка схемы");
    ui->textBrowser->repaint();
    int pageNum = 1;
    bool  checkResult = true;
    //ui->textBrowser->append(QDateTime::currentDateTime().time().toString("TIME hh:mm:ss.zzz"));
    for(auto &page:prPages) {
        std::vector<LDElement*> els = page.first->getAllelements();
        std::vector<QString> res = LDChecker::checkVarConnectedElements(els);
        if(!res.empty()) {
            checkResult = false;
            ui->textBrowser->append("Страница "+QString::number(pageNum));
            for(const QString &s:res) ui->textBrowser->append(s);
        }
        pageNum++;
    }

    std::vector<QString> adcInfo = checkADCconfig();
    if(adcInfo.size()) {
        for(QString info:adcInfo) {
            QString infoStr = QString("<b><font color=\"orange\">") + info + "</font></b>";
            ui->textBrowser->append(infoStr);
        }
        ui->textBrowser->append("<b><font color=\"black\"></font></b>");
    }

    if(checkResult) {
        ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Старт сборки проекта");
        ui->textBrowser->repaint();
        pageNum = 1;
        std::vector<QString> varBody;
        std::vector<QString> progBody;
        std::vector<QString> funcBody;
        std::set<QString> libHeader;
        std::set<QString> libBody;
        for(auto &page:prPages) {
            ConnectionFinder pageScan(page.first,pageNum++);
            auto result = pageScan.scanCircuits();

            std::vector<LDElement*> notUsedElements = result.notUsedElements;
            if(!notUsedElements.empty()) {
                ui->textBrowser->append("Страница " + QString::number(pageNum-1));
                for(LDElement *el:notUsedElements) {
                    QString infoStr = "cтрока:" + QString::number(el->getRowNum());
                    infoStr += " колонка:" + QString::number(el->getColNum()) + " <b><font color=\"orange\">(Элемент не влияет на работу программы)</font></b>";
                    el->setSelected(true);
                    el->getItem()->update();
                    ui->textBrowser->append(infoStr);
                }
                ui->textBrowser->append("<b><font color=\"black\"></font></b>");
            }


            std::vector<LDElement*> shortCircuitElements = result.shortCircuitElements;
            if(!shortCircuitElements.empty()) {
                ui->textBrowser->append("Страница " + QString::number(pageNum-1));
                for(LDElement *el:shortCircuitElements) {
                    QString infoStr = "cтрока:" + QString::number(el->getRowNum());
                    infoStr += " колонка:" + QString::number(el->getColNum()) + " <b><font color=\"red\">(Выход элемента замкнут на входную цепь)</font></b>";
                    el->setSelected(true);
                    el->getItem()->update();
                    ui->textBrowser->append(infoStr);
                }
                ui->textBrowser->append("<b><font color=\"black\"></font></b>");
            }

            std::copy(result.varBody.begin(),result.varBody.end(),std::back_inserter(varBody));
            std::copy(result.progBody.begin(),result.progBody.end(),std::back_inserter(progBody));
            std::copy(result.functionsBody.begin(),result.functionsBody.end(),std::back_inserter(funcBody));
            libHeader.insert(result.libHeader.begin(),result.libHeader.end());
            libBody.insert(result.libBody.begin(),result.libBody.end());
        }

        ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Генерация файлов");
        Compiler::makeProgFile(varBody,progBody,funcBody,plcConfig,prDelay);
        Compiler::makeLibraryfiles(libHeader,libBody);
        ui->textBrowser->repaint();
        ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Компиляция и линковка");
        ui->textBrowser->repaint();

        buildAction->setEnabled(false);
        QFutureWatcher<std::vector<QString>> *watcher = new QFutureWatcher<std::vector<QString>>();
        QFuture<std::vector<QString>> future = QtConcurrent::run(&Compiler::compile,plcType->currentText(),prDir);
        watcher->setFuture(future);

        connect(watcher,&QFutureWatcher<std::vector<QString>>::finished,
                [this,watcher,future](){
            std::vector<QString> compileResult =  future.result();
            if(!compileResult.empty()) {
                //buttonMax->click();
                for(const QString &s:compileResult) {
                    ui->textBrowser->append(s+"\n");
                }
            }else {
                bool isSysInfoVisible = ui->textBrowser->isVisible();
                if(isSysInfoVisible) QTimer::singleShot(500, this, [this](){buttonMin->click();});
                ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Проект успешно собран");
            }
            buildAction->setEnabled(true);
            delete watcher;
        });

        /*std::vector<QString> compileResult = Compiler::compile(plcType->currentText(),prDir);
        if(!compileResult.empty()) {
            buttonMax->click();
            for(const QString &s:compileResult) {
                ui->textBrowser->append(s+"\n");
            }
        }else {
            if(!isSysInfoVisible) QTimer::singleShot(500, this, [this](){buttonMin->click();});
            ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Проект успешно собран");
        }*/
    }else {
        this->update();
    }
    ui->textBrowser->repaint();
}

void MainWindow::search()
{
    std::vector<LDScene*> scenes;
    for(auto &prPage:prPages) scenes.push_back(prPage.first);
    auto pNum = static_cast<int>(page_num);

    int row = scenes.at(pNum)->getLastRow();
    int col = scenes.at(pNum)->getLastCol();
    if(row==0) row=1;
    if(col==0) col=1;
    Finder *finder = new Finder(scenes);
    auto *dialog = new DialogSearch(pNum,col,row);
    connect(dialog,&DialogSearch::startSearch,finder,&Finder::search);
    connect(finder,&Finder::findInfo,this,&MainWindow::searchResults);
    connect(dialog,&DialogSearch::replaceVarName,finder,&Finder::replace);
    dialog->exec();
    delete dialog;
    delete finder;
}

void MainWindow::searchResults(const std::vector<QString> &res)
{
    ui->textBrowser->clear();
    ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Поиск элементов");
    ui->textBrowser->setVisible(true);
    if(res.empty()) {
        ui->textBrowser->append(QDateTime::currentDateTime().time().toString() + ": Элементы не найдены");
    }else {
        for(const QString &info:res) {
            ui->textBrowser->append(info);
        }
    }
    ui->textBrowser->repaint();
}

void MainWindow::plcChanged(const QString &plcName)
{
    std::map<QString,QString> sysVarsComments = PLCVarContainer::getInstance().getSysVarsComments();
    std::map<QString,QString> initValues = PLCVarContainer::getInstance().getInitValues();

    std::vector<QString> parGroups = PLCVarContainer::getInstance().getParentGroups();

    for(const QString &parGr:parGroups) {
        std::vector<QString> sysGroups = PLCVarContainer::getInstance().getSystemVarGroups(parGr);

        for(const QString &gr:sysGroups) {
            PLCVarContainer::getInstance().delGroup(gr,parGr);
        }
    }

    PLCParams::readParamsByPLCName(plcName);
    for(int i=0;i<PLCParams::diCnt;i++) {
        PLCVar divar("DI"+QString::number(i+1),"состояние","Дискретные входы");
        divar.setReadable(true);
        divar.setValue(false);
        divar.setSystem(true);
        if(sysVarsComments.find(divar.getName())!=sysVarsComments.end()) divar.setComment(sysVarsComments[divar.getName()]);
        if(initValues.find(divar.getName())!=initValues.end()) divar.setValueAsString(initValues[divar.getName()]);

        PLCVarContainer::getInstance().addVar(divar);
    }

    for(int i=0;i<PLCParams::diCnt;i++) {
        PLCVar divarFault("DI"+QString::number(i+1)+"_FAULT","ошибка","Дискретные входы");
        divarFault.setReadable(true);
        divarFault.setValue(false);
        divarFault.setSystem(true);
        if(sysVarsComments.find(divarFault.getName())!=sysVarsComments.end()) divarFault.setComment(sysVarsComments[divarFault.getName()]);
        if(initValues.find(divarFault.getName())!=initValues.end()) divarFault.setValueAsString(initValues[divarFault.getName()]);

        PLCVarContainer::getInstance().addVar(divarFault);
    }

    for(int i=0;i<PLCParams::diCnt;i++) {
        PLCVar divarBreak("DI"+QString::number(i+1)+"_BREAK","обрыв","Дискретные входы");
        divarBreak.setReadable(true);
        divarBreak.setValue(false);
        divarBreak.setSystem(true);
        if(sysVarsComments.find(divarBreak.getName())!=sysVarsComments.end()) divarBreak.setComment(sysVarsComments[divarBreak.getName()]);
        if(initValues.find(divarBreak.getName())!=initValues.end()) divarBreak.setValueAsString(initValues[divarBreak.getName()]);

        PLCVarContainer::getInstance().addVar(divarBreak);
    }

    for(int i=0;i<PLCParams::diCnt;i++) {
        PLCVar divarShort("DI"+QString::number(i+1)+"_SHORT","кор. замыкание","Дискретные входы");
        divarShort.setReadable(true);
        divarShort.setValue(false);
        divarShort.setSystem(true);
        if(sysVarsComments.find(divarShort.getName())!=sysVarsComments.end()) divarShort.setComment(sysVarsComments[divarShort.getName()]);
        if(initValues.find(divarShort.getName())!=initValues.end()) divarShort.setValueAsString(initValues[divarShort.getName()]);

        PLCVarContainer::getInstance().addVar(divarShort);
    }

    for(int i=0;i<PLCParams::doCnt;i++) {
        PLCVar dovar("DO"+QString::number(i+1),"Дискретные выходы");
        dovar.setReadable(true);
        dovar.setWriteable(true);
        dovar.setValue(false);
        dovar.setSystem(true);
        if(sysVarsComments.find(dovar.getName())!=sysVarsComments.end()) dovar.setComment(sysVarsComments[dovar.getName()]);
        if(initValues.find(dovar.getName())!=initValues.end()) {
            dovar.setValueAsString(initValues[dovar.getName()]);
        }

        PLCVarContainer::getInstance().addVar(dovar);
    }

    for(int i=0;i<PLCParams::aiCnt;i++) {
        PLCVar aivar("AI"+QString::number(i+1),"состояние","Аналоговые входы");
        aivar.setReadable(true);
        aivar.setValue(static_cast<unsigned short>(0));
        aivar.setSystem(true);
        if(sysVarsComments.find(aivar.getName())!=sysVarsComments.end()) aivar.setComment(sysVarsComments[aivar.getName()]);
        if(initValues.find(aivar.getName())!=initValues.end()) aivar.setValueAsString(initValues[aivar.getName()]);

        PLCVarContainer::getInstance().addVar(aivar);
    }

    if(plcName!="MKU") {
        for(int i=0;i<PLCParams::aiCnt;i++) {
            PLCVar aivarRaw("AI"+QString::number(i+1)+"_RAW","необраб","Аналоговые входы");
            aivarRaw.setReadable(true);
            aivarRaw.setValue(static_cast<unsigned short>(0));
            aivarRaw.setSystem(true);
            if(sysVarsComments.find(aivarRaw.getName())!=sysVarsComments.end()) aivarRaw.setComment(sysVarsComments[aivarRaw.getName()]);
            if(initValues.find(aivarRaw.getName())!=initValues.end()) aivarRaw.setValueAsString(initValues[aivarRaw.getName()]);

            PLCVarContainer::getInstance().addVar(aivarRaw);
        }

        for(int i=0;i<PLCParams::aiCnt;i++) {
            PLCVar aivarUnder("AI"+QString::number(i+1)+"_UNDER","ниже порога","Аналоговые входы");
            aivarUnder.setReadable(true);
            aivarUnder.setValue(static_cast<unsigned short>(0));
            aivarUnder.setSystem(true);
            if(sysVarsComments.find(aivarUnder.getName())!=sysVarsComments.end()) aivarUnder.setComment(sysVarsComments[aivarUnder.getName()]);
            if(initValues.find(aivarUnder.getName())!=initValues.end()) aivarUnder.setValueAsString(initValues[aivarUnder.getName()]);

            PLCVarContainer::getInstance().addVar(aivarUnder);
        }
        for(int i=0;i<PLCParams::aiCnt;i++) {
            PLCVar aivarOver("AI"+QString::number(i+1)+"_OVER","выше порога","Аналоговые входы");
            aivarOver.setReadable(true);
            aivarOver.setValue(static_cast<unsigned short>(0));
            aivarOver.setSystem(true);
            if(sysVarsComments.find(aivarOver.getName())!=sysVarsComments.end()) aivarOver.setComment(sysVarsComments[aivarOver.getName()]);
            if(initValues.find(aivarOver.getName())!=initValues.end()) aivarOver.setValueAsString(initValues[aivarOver.getName()]);

            PLCVarContainer::getInstance().addVar(aivarOver);
        }
        for(int i=0;i<PLCParams::aiCnt;i++) {
            PLCVar aivarAlarm("AI"+QString::number(i+1)+"_ALARM","авария","Аналоговые входы");
            aivarAlarm.setReadable(true);
            aivarAlarm.setValue(static_cast<unsigned short>(0));
            aivarAlarm.setSystem(true);
            if(sysVarsComments.find(aivarAlarm.getName())!=sysVarsComments.end()) aivarAlarm.setComment(sysVarsComments[aivarAlarm.getName()]);
            if(initValues.find(aivarAlarm.getName())!=initValues.end()) aivarAlarm.setValueAsString(initValues[aivarAlarm.getName()]);

            PLCVarContainer::getInstance().addVar(aivarAlarm);
        }
    }

    for(int i=0;i<PLCParams::tmrms_cnt;i++) {
        PLCVar tmrvar("TMRms"+QString::number(i+1),"мс","Таймеры");
        tmrvar.setReadable(true);
        tmrvar.setWriteable(true);
        tmrvar.setValue(static_cast<unsigned short>(0));
        tmrvar.setSystem(true);
        if(sysVarsComments.find(tmrvar.getName())!=sysVarsComments.end()) tmrvar.setComment(sysVarsComments[tmrvar.getName()]);
        if(initValues.find(tmrvar.getName())!=initValues.end()) tmrvar.setValueAsString(initValues[tmrvar.getName()]);

        PLCVarContainer::getInstance().addVar(tmrvar);
    }
    for(int i=0;i<PLCParams::tmrs_cnt;i++) {
        PLCVar tmrvar("TMRs"+QString::number(i+1),"сек","Таймеры");
        tmrvar.setReadable(true);
        tmrvar.setWriteable(true);
        tmrvar.setValue(static_cast<unsigned short>(0));
        tmrvar.setSystem(true);
        if(sysVarsComments.find(tmrvar.getName())!=sysVarsComments.end()) tmrvar.setComment(sysVarsComments[tmrvar.getName()]);
        if(initValues.find(tmrvar.getName())!=initValues.end()) tmrvar.setValueAsString(initValues[tmrvar.getName()]);

        PLCVarContainer::getInstance().addVar(tmrvar);
    }

    for(int i=0;i<PLCParams::ibit_cnt;i++) {
        PLCVar bitVar("IB" + QString::number(i+1), "Биты");
        bitVar.setReadable(true);
        bitVar.setWriteable(true);
        bitVar.setValue(false);
        bitVar.setSystem(true);
        if(sysVarsComments.find(bitVar.getName())!=sysVarsComments.end()) bitVar.setComment(sysVarsComments[bitVar.getName()]);
        if(initValues.find(bitVar.getName())!=initValues.end()) bitVar.setValueAsString(initValues[bitVar.getName()]);

        PLCVarContainer::getInstance().addVar(bitVar);
    }

    for(int i=0;i<PLCParams::ireg_cnt;i++) {
        PLCVar regVar("IR" + QString::number(i+1), "Регистры");
        regVar.setReadable(true);
        regVar.setWriteable(true);
        regVar.setValue(static_cast<unsigned short>(0));
        regVar.setSystem(true);
        if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
        if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

        PLCVarContainer::getInstance().addVar(regVar);
    }

    if(PLCUtils::isPLCSupportCAN(plcName)) {
        for(int node=0;node<8;node++) {
            for(int i=0;i<28;i++) {
                PLCVar bitVar("CLBIT" + QString::number(node*28+i+17), "NODE"+QString::number(node),"Биты кластера");
                bitVar.setReadable(true);
                bitVar.setWriteable(true);
                bitVar.setValue(false);
                bitVar.setSystem(true);
                if(sysVarsComments.find(bitVar.getName())!=sysVarsComments.end()) bitVar.setComment(sysVarsComments[bitVar.getName()]);
                if(initValues.find(bitVar.getName())!=initValues.end()) bitVar.setValueAsString(initValues[bitVar.getName()]);

                PLCVarContainer::getInstance().addVar(bitVar);
            }
        }

        for(int i=0;i<64;i++) {
            PLCVar regVar("CLREG" + QString::number(i+17), "Регистры кластера");
            regVar.setReadable(true);
            regVar.setWriteable(true);
            regVar.setValue(static_cast<unsigned short>(0));
            regVar.setSystem(true);
            if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
            if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

            PLCVarContainer::getInstance().addVar(regVar);
        }

        for(int i=0;i<16;i++) {
            PLCVar bitVar("NBIT" + QString::number(i+241), "TX","Сетевые биты");
            bitVar.setReadable(true);
            bitVar.setWriteable(true);
            bitVar.setValue(false);
            bitVar.setSystem(true);
            if(sysVarsComments.find(bitVar.getName())!=sysVarsComments.end()) bitVar.setComment(sysVarsComments[bitVar.getName()]);
            if(initValues.find(bitVar.getName())!=initValues.end()) bitVar.setValueAsString(initValues[bitVar.getName()]);

            PLCVarContainer::getInstance().addVar(bitVar);
        }

        for(int clust=0;clust<8;clust++) {
            for(int i=0;i<16;i++) {
                PLCVar bitVar("NBIT" + QString::number(clust*16+i+257), "CLUSTER"+QString::number(clust),"Сетевые биты");
                bitVar.setReadable(true);
                bitVar.setWriteable(false);
                bitVar.setValue(false);
                bitVar.setSystem(true);
                if(sysVarsComments.find(bitVar.getName())!=sysVarsComments.end()) bitVar.setComment(sysVarsComments[bitVar.getName()]);
                if(initValues.find(bitVar.getName())!=initValues.end()) bitVar.setValueAsString(initValues[bitVar.getName()]);

                PLCVarContainer::getInstance().addVar(bitVar);
            }
        }

        for(int i=0;i<16;i++) {
            PLCVar regVar("NREG" + QString::number(i+81), "TX","Сетевые регистры");
            regVar.setReadable(true);
            regVar.setWriteable(true);
            regVar.setValue(static_cast<unsigned short>(0));
            regVar.setSystem(true);
            if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
            if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

            PLCVarContainer::getInstance().addVar(regVar);
        }

        for(int clust=0;clust<8;clust++) {
            for(int i=0;i<16;i++) {
                PLCVar regVar("NREG" + QString::number(clust*16+i+97), "CLUSTER"+QString::number(clust),"Сетевые регистры");
                regVar.setReadable(true);
                regVar.setWriteable(false);
                regVar.setValue(static_cast<unsigned short>(0));
                regVar.setSystem(true);
                if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
                if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

                PLCVarContainer::getInstance().addVar(regVar);
            }
        }

        std::vector<QString> comments = {"Node 0 online","Node 1 online","Node 2 online","Node 3 online",
                                         "Node 4 online","Node 5 online","Node 6 online","Node 7 online",
                                        "CAN Network", "Start up", "Seconds", "Minutes", "not used", "Telemetry state","Cluster num",
                                        "Cluster 0 online","Cluster 1 online","Cluster 2 online","Cluster 3 online",
                                        "Cluster 4 online","Cluster 5 online","Cluster 6 online","Cluster 7 online"};
        for(std::size_t i=0;i<23;i++) {
            PLCVar regVar("SS" + QString::number(i+1), "Системные переменные");
            regVar.setReadable(true);
            regVar.setWriteable(false);
            if(i==10 || i==11) {
                regVar.setValue(static_cast<unsigned long>(0));
            }else {
                regVar.setValue(static_cast<unsigned short>(0));
            }
            regVar.setSystem(true);
            if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
            else {
                if(i<comments.size()) regVar.setComment(comments.at(i));
            }
            if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

            PLCVarContainer::getInstance().addVar(regVar);
        }
    }

    if(plcName!="MKU") {
        for(int i=0;i<16;i++) {
            PLCVar bitVar("SC_BIT" + QString::number(i+1), "Биты","Скада");
            bitVar.setReadable(true);
            bitVar.setWriteable(true);
            bitVar.setValue(false);
            bitVar.setSystem(true);
            if(sysVarsComments.find(bitVar.getName())!=sysVarsComments.end()) bitVar.setComment(sysVarsComments[bitVar.getName()]);
            if(initValues.find(bitVar.getName())!=initValues.end()) bitVar.setValueAsString(initValues[bitVar.getName()]);

            PLCVarContainer::getInstance().addVar(bitVar);
        }

        for(int i=0;i<16;i++) {
            PLCVar regVar("SC_REG" + QString::number(i+1), "Регистры","Скада");
            regVar.setReadable(true);
            regVar.setWriteable(true);
            regVar.setValue(static_cast<unsigned short>(0));
            regVar.setSystem(true);
            if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
            if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

            PLCVarContainer::getInstance().addVar(regVar);
        }
    }

    if(PLCUtils::isPLCSupportModbusMaster(plcName)) {
        for(int i=0;i<64;i++) {
            PLCVar regVar("MODB" + QString::number(i+1), "регистры", "Modbus master");
            regVar.setReadable(true);
            regVar.setWriteable(true);
            regVar.setValue(static_cast<unsigned short>(0));
            regVar.setSystem(true);
            if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
            if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

            PLCVarContainer::getInstance().addVar(regVar);
        }

        for(int i=1;i<=254;i++) {
            PLCVar regVar("MODB_ERR" + QString::number(i), "ошибки", "Modbus master");
            regVar.setReadable(true);
            regVar.setWriteable(false);
            regVar.setValue(static_cast<unsigned short>(0));
            regVar.setSystem(true);
            if(sysVarsComments.find(regVar.getName())!=sysVarsComments.end()) regVar.setComment(sysVarsComments[regVar.getName()]);
            if(initValues.find(regVar.getName())!=initValues.end()) regVar.setValueAsString(initValues[regVar.getName()]);

            PLCVarContainer::getInstance().addVar(regVar);
        }
    }

    PLCVar workTimeVar("work_time", "Системное время","Таймеры");
    workTimeVar.setReadable(true);
    workTimeVar.setWriteable(true);
    workTimeVar.setValue(static_cast<unsigned short>(0));
    workTimeVar.setSystem(true);
    if(sysVarsComments.find(workTimeVar.getName())!=sysVarsComments.end()) workTimeVar.setComment(sysVarsComments[workTimeVar.getName()]);
    if(initValues.find(workTimeVar.getName())!=initValues.end()) workTimeVar.setValueAsString(initValues[workTimeVar.getName()]);

    PLCVarContainer::getInstance().addVar(workTimeVar);

    if(PLCUtils::isPLCSupportEth(plcName)) {
        if(progIP.isEmpty()) {
            progIP="192.168.1.2";
            ethAsDefault = true;
        }
    }
    if(configAction) {
        if(PLCUtils::isPLCSupportEth(plcName)) {configAction->setVisible(true);configAction->setEnabled(true);}
        else {configAction->setVisible(false);configAction->setEnabled(false);}
    }
    if(configADCAction) {
        if(PLCUtils::isPLCSupportADC(plcName)) {configADCAction->setVisible(true);configADCAction->setEnabled(true);}
        else {configADCAction->setVisible(false);configADCAction->setEnabled(false);}
    }

    if(configDIAction) {
        if(PLCUtils::isPLCSupportDI(plcName)) {configDIAction->setVisible(true);configDIAction->setEnabled(true);}
        else {configDIAction->setVisible(false);configDIAction->setEnabled(false);}
    }

    if(configDOAction) {
        if(PLCUtils::isPLCSupportDO(plcName)) {configDOAction->setVisible(true);configDOAction->setEnabled(true);}
        else {configDOAction->setVisible(false);configDOAction->setEnabled(false);}
    }

    if(modbusAction) {
        if(PLCUtils::isPLCSupportModbusMaster(plcName)) {modbusAction->setVisible(true);modbusAction->setEnabled(true);}
        else {modbusAction->setVisible(false);modbusAction->setEnabled(false);}
    }
    if(createMapAction) {
        if(PLCUtils::isPLCSupportEth(plcName)) {createMapAction->setVisible(true);createMapAction->setEnabled(true);}
        else {createMapAction->setVisible(false);createMapAction->setEnabled(false);}
    }
    if(applConfigAction) {
        if(PLCUtils::isPLCSupportEth(plcName)) {applConfigAction->setVisible(true);applConfigAction->setEnabled(true);}
        else {applConfigAction->setVisible(false);applConfigAction->setEnabled(false);}
    }
    plcConfig.setName(plcName);
}

void MainWindow::readWriteConfig()
{
    if(PLCUtils::isPLCSupportEth(plcType->currentText())) {
        DialogPLCConfig *dialog = new DialogPLCConfig(plcConfig);

        connect(dialog,&DialogPLCConfig::saveConf,[this](const QByteArray &conf){plcConfig.setSettings(conf);prChanged=true;});
        dialog->setCurrentIP(progIP);
        dialog->exec();
        delete dialog;
    }
}

void MainWindow::connectScene(LDScene *sc)
{
    connect(sc,&LDScene::updateName,this,&MainWindow::updateInfo);
    connect(sc,&LDScene::createPageBefore,this,&MainWindow::createPageBefore);
    connect(sc,&LDScene::createPageAfter,this,&MainWindow::createPageAfter);
    connect(sc,&LDScene::deletePage,this,&MainWindow::deletePage);
    connect(sc,&LDScene::sceneChanged,[this](){prChanged=true;});
    connect(sc,&LDScene::searchElement,this,&MainWindow::searchVar);
}

std::vector<QString> MainWindow::checkADCconfig()
{
    std::vector<QString> result;
    if(PLCUtils::isPLCSupportADC(plcType->currentText())) {
        int cnt = 14;
        if(plcConfig.getSettings().size()>=cnt)
        for(int i=0;i<cnt;i++) {
            bool inpType = static_cast<bool>(plcConfig.getSettings().at(i));
            int sensType = plcConfig.getSensorTypeCode(i);
            if(sensType>=0) {
                if(inpType) {
                    if(sensType==0 || sensType==4) result.push_back("Аналоговый вход " + QString::number(i+1) + ": Тип датчика не соответствует типу входа");
                }else {
                    if(sensType==1 || sensType==2 || sensType==3) result.push_back("Аналоговый вход " + QString::number(i+1) + ": Тип датчика не соответствует типу входа");
                }
            }
        }
    }
    return result;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(prChanged) {
        if (QMessageBox::Yes == QMessageBox::question(this,
            tr("LD Редактор"),
            tr("Текущий проект был изменён.\nВы хотите его сохранить?"))) {
            saveProject();
        }
    }
    event->accept();
}
