#ifndef DIALOGADDVAR_H
#define DIALOGADDVAR_H

#include <QDialog>

namespace Ui {
class DialogAddVar;
}

class DialogAddVar : public QDialog
{
    Q_OBJECT
    static QString lastGroup;
    QString varName;
    QString grName;
public:
    explicit DialogAddVar(QWidget *parent = nullptr);
    ~DialogAddVar();
    QString getVarName() const {return varName;}
    QString getGroupName() const {return grName;}

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_DialogAddVar_accepted();

    void on_lineEditVarName_textChanged(const QString &arg1);

private:
    Ui::DialogAddVar *ui;
};

#endif // DIALOGADDVAR_H
