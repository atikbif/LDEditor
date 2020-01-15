#ifndef DIALOGRENAMEGROUP_H
#define DIALOGRENAMEGROUP_H

#include <QDialog>

namespace Ui {
class DialogRenameGroup;
}

class DialogRenameGroup : public QDialog
{
    Q_OBJECT
    QString groupName;
public:
    explicit DialogRenameGroup(QWidget *parent = nullptr);
    ~DialogRenameGroup();
    QString getGroupName() const {return groupName;}

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::DialogRenameGroup *ui;
};

#endif // DIALOGRENAMEGROUP_H
