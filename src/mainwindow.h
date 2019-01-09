#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_horizontalSlider_sliderMoved(int position);

    void buttonClicked();

    void on_deleteGroupButton_clicked();

    void on_pushButton_clicked();

    void groupButtonClicked();

    void buttonDoubleClicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    void loadGroups();
    void setGroups();
    void clearPhotoArea();
    void fillPhotoArea();

};

#endif // MAINWINDOW_H
