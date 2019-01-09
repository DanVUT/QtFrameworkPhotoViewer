#include "fullscreen.h"
#include "ui_fullscreen.h"
#include <QImage>
#include <QPixmap>


fullscreen::fullscreen(QString image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fullscreen)
{
    ui->setupUi(this);
    QImage img(image);
    ui->imageLabel->setPixmap(QPixmap::fromImage(img));

}

fullscreen::~fullscreen()
{
    delete ui;


}


void fullscreen::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    }
}
