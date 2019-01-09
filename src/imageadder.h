#ifndef IMAGEADDER_H
#define IMAGEADDER_H

#include <QDialog>

namespace Ui {
class imageAdder;
}

class imageAdder : public QDialog
{
    Q_OBJECT

public:
    explicit imageAdder(QStringList * groups,QWidget *parent = 0);
    ~imageAdder();

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void buttonClicked();




    void on_buttonBox_accepted();

private:
    Ui::imageAdder *ui;
};

#endif // IMAGEADDER_H
