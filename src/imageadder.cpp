#include "imageadder.h"
#include "ui_imageadder.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QtGui>
#include <QScrollArea>
#include <QFileInfo>
#include <iostream>
#include <string>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QLayout>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include "mypushbutton.h"
#include <QStringList>
#include "mainwindow.h"


QDir currentDirectoryIA;
QString pathStringIA;
QFileSystemModel * dirmodelIA;
QStringList groupImages;
QStringList * groupsIA;

imageAdder::imageAdder(QStringList * groups, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::imageAdder)
{
    ui->setupUi(this);
    groupsIA = groups;
    QString rPath="/";
    dirmodelIA = new QFileSystemModel(this);
    dirmodelIA->setRootPath(rPath);
    dirmodelIA->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->treeView->setModel(dirmodelIA);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);


    QGridLayout * grid = new QGridLayout();
    ui->scrollAreaWidgetContents_2->setLayout(grid);
}

imageAdder::~imageAdder()
{
    delete ui;
}

void imageAdder::on_treeView_clicked(const QModelIndex &index)
{
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.tiff" << "*.bmp";
    QDir directory(dirmodelIA->fileInfo(index).absoluteFilePath());
    directory.setNameFilters(filters);
    currentDirectoryIA = directory;
    pathStringIA = dirmodelIA->fileInfo(index).absoluteFilePath();

    QImage img;
    QLayoutItem *child;

    while ((child = ui->scrollAreaWidgetContents_2->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    delete ui->scrollAreaWidgetContents_2->layout();
    QGridLayout * gridLayout = new QGridLayout();

    ui->scrollAreaWidgetContents_2->setLayout(gridLayout);

    for (int i = 0; i<directory.count(); i++) {
        if (img.load(dirmodelIA->fileInfo(index).absoluteFilePath() + "/" + directory[i])){
            MyPushButton *button = new MyPushButton;
            button->imgPath = dirmodelIA->fileInfo(index).absoluteFilePath() + "/" + directory[i];
            if(groupImages.contains(button->imgPath)){
                button->isSelected = true;
                button->setStyleSheet("QToolButton{background-color: green}");
            }
            button->setFixedWidth(ui->horizontalSlider_2->value());
            button->setText(directory[i]);
            button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            button->setFixedHeight((ui->horizontalSlider_2->value()/16)*9);
            button->setIcon(QIcon(QPixmap::fromImage(img.scaled(button->width(),button->height(), Qt::KeepAspectRatio))));
            button->setIconSize(QSize(button->width(),button->height() - 25));
            connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
            gridLayout->addWidget(button, i/4, i%4);
        }
    }

    ui->scrollAreaWidgetContents_2->setLayout(gridLayout);
}


void imageAdder::buttonClicked(){
    QObject * senderObj = sender();
    MyPushButton * button = qobject_cast<MyPushButton*>(senderObj);

    if(!button->isSelected){
        if(!groupImages.contains(button->imgPath)){
            groupImages << button->imgPath;
        }
        button->setStyleSheet("QToolButton{background-color: green}");
        button->isSelected = true;
    }
    else{
        if(groupImages.contains(button->imgPath)){
            groupImages.removeAt(groupImages.indexOf(button->imgPath));
        }
        button->setStyleSheet("QToolButton{}");
        button->isSelected = false;
    }
}



void imageAdder::on_buttonBox_accepted()
{
    QMessageBox * box = new QMessageBox();
    if(ui->lineEdit->text()==""){
        groupImages.clear();
        box->setText("Forgot to fill the Group Name field, no changes were made");
        box->exec();
        return;
    }
    if(groupImages.count()==0){
        groupImages.clear();
        box->setText("No images were selected, no changes were made");
        box->exec();
        return;
    }
    groupsIA->append(ui->lineEdit->text());

    QFile outputfile("Groups.cfg");
    outputfile.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!outputfile.isOpen()){
        qDebug() << "- Error, unable to open" << "outputFilename" << "for output";
    }
    QTextStream outStream(&outputfile);
    for(int i = 0; i<groupsIA->count(); i++){
        outStream << groupsIA->at(i) << "\n";
    }
    outputfile.close();

    QFile output(ui->lineEdit->text());
    output.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!output.isOpen()){
        qDebug() << "- Error, unable to open" << "outputFilename" << "for output";
    }
    QTextStream out(&output);
    for(int i = 0; i<groupImages.count(); i++){
        out << groupImages.at(i) << "\n";
    }
    output.close();
    groupImages.clear();
}
