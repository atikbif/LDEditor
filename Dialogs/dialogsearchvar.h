#ifndef DIALOGSEARCHVAR_H
#define DIALOGSEARCHVAR_H

#include <QDialog>

namespace Ui {
class DialogSearchVar;
}

class DialogSearchVar : public QDialog
{
    Q_OBJECT

    QString grName;
    QString varName;

public:
    explicit DialogSearchVar(const QString &grName, const QString &varName, QWidget *parent = nullptr);
    void addElement(int pageNum, int row, int column, const QString &elName);
    ~DialogSearchVar();

signals:
    void goToElement(int pageNum, int row, int col);

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::DialogSearchVar *ui;
};

#endif // DIALOGSEARCHVAR_H
