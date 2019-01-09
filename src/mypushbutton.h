#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QString>
#include <QObject>
#include <QStringList>

class MyPushButton: public QToolButton
{
    Q_OBJECT

public:
    MyPushButton();
    QString imgPath;
    QString group;
    bool isSelected;
    QStringList images;
    //virtual ~MyPushButton();
signals:
    void doubleClick();
};



#endif // MYPUSHBUTTON_H
