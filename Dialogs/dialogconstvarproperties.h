#ifndef DIALOGCONSTVARPROPERTIES_H
#define DIALOGCONSTVARPROPERTIES_H

#include <QDialog>
#include "plcvar.h"

namespace Ui {
class DialogConstVarProperties;
}

class DialogConstVarProperties : public QDialog
{
    Q_OBJECT
    PLCVar::varType value;
    QString type;
    QString stringValue;
public:
    explicit DialogConstVarProperties(PLCVar::varType value, const QString &type, QWidget *parent = nullptr);
    ~DialogConstVarProperties();
    QString getType() const {return type;}
    PLCVar::varType getValue() const {return value;}

private slots:
    void on_DialogConstVarProperties_accepted();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::DialogConstVarProperties *ui;
};

#endif // DIALOGCONSTVARPROPERTIES_H
