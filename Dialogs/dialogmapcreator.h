#ifndef DIALOGMAPCREATOR_H
#define DIALOGMAPCREATOR_H

#include <QDialog>
#include <QString>
#include <QStatusBar>

namespace Ui {
class DialogMapCreator;
}

class DialogMapCreator : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMapCreator(QWidget *parent = nullptr);
    ~DialogMapCreator();
    void setApplicationName(const QString &value);
    QString getApplicationName() const;
    void setApplicationVersion(const QString &value);
    QString getApplicationVersion() const;
    void setClusterNum(int value);
    int getClusterNum() const;
    void setNodeNum(int value);
    int getNodeNum() const;

private:
    Ui::DialogMapCreator *ui;
};

#endif // DIALOGMAPCREATOR_H
