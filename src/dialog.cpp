#include "dialog.h"
#include "ui_dialog.h"
#include <QStringList>
#include "mainwindow.h"

QStringList * groupsGlobal;
Dialog::Dialog(QStringList * groups, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    groupsGlobal = groups;
    ui->setupUi(this);
    for(int i=0; i<groups->count(); i++){
        ui->comboBox->addItem(groups->at(i));
    }

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    groupsGlobal->removeAt(ui->comboBox->currentIndex());
}
