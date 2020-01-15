#ifndef DIALOGVARCONFIG_H
#define DIALOGVARCONFIG_H

#include <QDialog>
#include "plcvarcontainer.h"
#include <QTreeWidgetItem>

namespace Ui {
class DialogVarConfig;
}

class DialogVarConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVarConfig(QWidget *parent = nullptr);
    ~DialogVarConfig();

private slots:
    void on_pushButtonAddVar_clicked();

    void on_DialogVarConfig_rejected();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButtonEditVar_clicked();

    void on_pushButtonDelVar_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButtonRenameGroup_clicked();

    void on_pushButtonDeleteGroup_clicked();

private:
    Ui::DialogVarConfig *ui;
    void drawVarTree(const QString &grName="", const QString &varName="");
};

#endif // DIALOGVARCONFIG_H
