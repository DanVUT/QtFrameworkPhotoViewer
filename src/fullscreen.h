#ifndef FULLSCREEN_H
#define FULLSCREEN_H

#include <QWidget>
#include <QKeyEvent>


namespace Ui {
class fullscreen;
}

class fullscreen : public QWidget
{
    Q_OBJECT

public:
    explicit fullscreen(QString image,QWidget *parent = 0);
    ~fullscreen();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::fullscreen *ui;
};

#endif // FULLSCREEN_H
