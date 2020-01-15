#ifndef DIALOGADDEDITCOMMENT_H
#define DIALOGADDEDITCOMMENT_H

#include <QDialog>

namespace Ui {
class DialogAddEditComment;
}

class DialogAddEditComment : public QDialog
{
    Q_OBJECT
    QString comment;
public:
    explicit DialogAddEditComment(const QString &comment, QWidget *parent = nullptr);
    ~DialogAddEditComment();
    QString getComment() const {return comment;}

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::DialogAddEditComment *ui;
};

#endif // DIALOGADDEDITCOMMENT_H
