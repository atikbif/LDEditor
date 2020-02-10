#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ldscene.h"
#include "cmdstack.h"
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include "copybufitem.h"
#include <QPrinter>
#include <QPushButton>
#include <QComboBox>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    LDScene *scene;
    QLineEdit *itemName;
    QLineEdit *itemType;
    QLineEdit *itemInfo;
    QLineEdit *itemComment;
    QLineEdit *mouseCol;
    QLineEdit *mouseRow;

    QPushButton *buttonMin;
    QPushButton *buttonMax;

    QComboBox *plcType;

    LDPage page;
    CmdStack *stack;
    QSpinBox *pageNumWidget;
    QLabel *maxPageNumWidget;
    int zoomLevel = 0;

    QMenu *fileMenu;

    const int maxAmountOfPrevProjects = 10;

    bool prChanged = false;
    QString prFileName;
    QString prDir;
    int prDelay = 10;
    int stopBits = 1;
    QString baudrate = "АВТО";
    QString parity = "чётный (even)";
    int netAddr = 0x00;
    QString progIP;
    bool ethAsDefault = false;


    std::size_t page_num = 0;
    std::vector<std::pair<LDScene*,CmdStack*>> prPages;
    int otherPage = 12;

    int page_count = 1;

    QAction *configAction = nullptr;

    QStringList getPrevProjects(void);
    void updatePrevProjects(const QStringList &prNames);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createPageBefore();
    void createPageAfter();
    void deletePage(bool confirmation = true);
    void updateInfo(const LDInfo &elInfo);
    void saveProject();
    void saveAsProject();
    void openProject();
    void openProjectByName(const QString &fName);
    void printPreview(QPrinter *printer);
    void previewAction();
    void build();
    void search();
    void searchResults(const std::vector<QString> &res);
    void plcChanged(const QString &plcName);
    void readWriteConfig();
    void openPrevProject();

private:
    void connectScene(LDScene *sc);
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
