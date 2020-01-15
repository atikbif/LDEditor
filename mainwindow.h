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

    bool prChanged = false;
    QString prFileName;
    QString prDir;
    int prDelay = 10;
    int stopBits = 1;
    QString baudrate = "АВТО";
    QString parity = "чётный (even)";
    int netAddr = 0x00;


    std::size_t page_num = 0;
    std::vector<std::pair<LDScene*,CmdStack*>> prPages;
    int otherPage = 12;

    int page_count = 1;

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
    void printPreview(QPrinter *printer);
    void previewAction();
    void build();
    void search();
    void searchResults(const std::vector<QString> &res);
    void plcChanged(const QString &plcName);

private:
    void connectScene(LDScene *sc);
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
