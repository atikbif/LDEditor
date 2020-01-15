#ifndef DIALOGEDITSYSTEMVAR_H
#define DIALOGEDITSYSTEMVAR_H

#include <QDialog>

namespace Ui {
class DialogEditSystemVar;
}

class DialogEditSystemVar : public QDialog
{
    Q_OBJECT

    QString varComment;

public:
    explicit DialogEditSystemVar(const QString &varComment, QWidget *parent = nullptr);
    ~DialogEditSystemVar();
    QString getComment() const {return varComment;}

private slots:
    void on_lineEditComment_textChanged(const QString &arg1);

private:
    Ui::DialogEditSystemVar *ui;
};

#endif // DIALOGEDITSYSTEMVAR_H
