#ifndef DIALOGVARINITLIST_H
#define DIALOGVARINITLIST_H

#include <QDialog>

namespace Ui {
class DialogVarInitList;
}

class DialogVarInitList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVarInitList(QWidget *parent = nullptr);
    ~DialogVarInitList();
    void addVar(const QString &vName, const QString &userVarName, const QString &initValue);

private:
    Ui::DialogVarInitList *ui;
};

#endif // DIALOGVARINITLIST_H
