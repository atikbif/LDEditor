#ifndef DIALOGEDITNOTSYSTEMVAR_H
#define DIALOGEDITNOTSYSTEMVAR_H

#include <QDialog>

namespace Ui {
class DialogEditNotSystemVar;
}

class DialogEditNotSystemVar : public QDialog
{
    Q_OBJECT
    QString varName;
    QString varType;
    QString varComment;
public:
    explicit DialogEditNotSystemVar(const QString &varName, const QString &varType, const QString &varComment, QWidget *parent = nullptr);
    ~DialogEditNotSystemVar();
    QString getVarName() const {return varName;}
    QString getVarType() const {return varType;}
    QString getVarComment() const {return varComment;}

private slots:
    void on_lineEditVarName_textChanged(const QString &arg1);

    void on_comboBoxVarType_currentTextChanged(const QString &arg1);

    void on_lineEditComment_textChanged(const QString &arg1);

private:
    Ui::DialogEditNotSystemVar *ui;
};

#endif // DIALOGEDITNOTSYSTEMVAR_H
