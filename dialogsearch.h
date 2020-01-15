#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include <QDialog>

namespace Ui {
class DialogSearch;
}

struct SearchConfig {
    bool searchNames = true;
    bool searchComments = true;
    bool searchConnections = true;
    bool fullDocument = false;
    bool registerFlag = false;
    bool fullWord = false;
    QString searchWord;
    QString connectionVarName;
    QString replaceVarName;
};

class DialogSearch : public QDialog
{
    Q_OBJECT
    int &pageNum;
    int &col;
    int &row;

public:
    SearchConfig config;
    explicit DialogSearch(int &pageNum, int &col, int &row, QWidget *parent = nullptr);
    ~DialogSearch();

signals:
    void startSearch(const SearchConfig &conf, int &pageNum, int &col, int &row);
    void replaceVarName(const QString &oldVarName, const QString &newVarName);

private slots:
    void on_radioButtonDocument_toggled(bool checked);

    void on_pushButtonSearch_clicked();

    void on_pushButtonReplaceAll_clicked();

private:
    Ui::DialogSearch *ui;
};

#endif // DIALOGSEARCH_H
