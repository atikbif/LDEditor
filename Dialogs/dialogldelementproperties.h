#ifndef DIALOGLDELEMENTPROPERTIES_H
#define DIALOGLDELEMENTPROPERTIES_H

#include <QDialog>
#include "ldelement.h"
#include <QTreeWidgetItem>


namespace Ui {
class DialogLDElementProperties;
}

class DialogLDElementProperties : public QDialog
{
    Q_OBJECT

    LDElement *el = nullptr;
    QString grName;
    QString varName;
    QString comment;

public:
    explicit DialogLDElementProperties(LDElement *el, QWidget *parent = nullptr);
    ~DialogLDElementProperties();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_buttonBox_accepted();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::DialogLDElementProperties *ui;
};

#endif // DIALOGLDELEMENTPROPERTIES_H
