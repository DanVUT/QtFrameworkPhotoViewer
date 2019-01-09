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
#include "dialog.h"
#include "imageadder.h"
#include "fullscreen.h"


QDir currentDirectory;
QString pathString;
QFileSystemModel * dirmodel;
QStringList groups;
QString currentImage;

void MainWindow::loadGroups(){
    QFile inputFile("Groups.cfg");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          groups << line;
       }
       inputFile.close();
    }

}

void MainWindow::setGroups(){
    QGridLayout * gridLayoutGroups = new QGridLayout();

    QImage img;
    for(int i=0; i<groups.count(); i++){
        MyPushButton *button = new MyPushButton;
        button->setFixedSize(60,60);
        if(img.load("directory.png")){
            button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            button->setIcon(QIcon(QPixmap::fromImage(img.scaled(button->width(),button->height(), Qt::KeepAspectRatio))));
            button->setIconSize(QSize(60,40));
        }
        button->group = groups[i];
        button->setText(groups[i]);
        QFile input(groups[i]);
        input.open(QIODevice::ReadOnly);
        QTextStream inS(&input);
        while (!inS.atEnd())
        {
           QString line = inS.readLine();
           button->images << line;
        }
        connect(button, SIGNAL(clicked()), this, SLOT(groupButtonClicked()));
        gridLayoutGroups->addWidget(button, i/4, i%4);
    }
    ui->groupContents->setLayout(gridLayoutGroups);
}

void MainWindow::clearPhotoArea(){
    QLayoutItem *child;
    while ((child = ui->scrollAreaWidgetContents->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    delete ui->scrollAreaWidgetContents->layout();
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    loadGroups();
    setGroups();


    QGridLayout * gridLayout = new QGridLayout();
    ui->scrollAreaWidgetContents->setLayout(gridLayout);






    QList<int> sizes;
    sizes << 200 << 500;
    ui->splitter->setSizes(sizes);



    QString rPath="/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(rPath);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->treeView->setModel(dirmodel);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{



    clearPhotoArea();

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.tiff" << "*.bmp";
    QDir directory(dirmodel->fileInfo(index).absoluteFilePath());
    directory.setNameFilters(filters);
    currentDirectory = directory;
    pathString = dirmodel->fileInfo(index).absoluteFilePath();
    QImage img;

    QGridLayout * gridLayout = new QGridLayout();
    ui->scrollAreaWidgetContents->setLayout(gridLayout);

    for (int i = 0; i<directory.count(); i++) {
        if (img.load(dirmodel->fileInfo(index).absoluteFilePath() + "/" + directory[i])){
            MyPushButton *button = new MyPushButton;
            button->imgPath = dirmodel->fileInfo(index).absoluteFilePath() + "/" + directory[i];
            button->setFixedWidth(ui->horizontalSlider->value());
            button->setText(directory[i]);
            button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            button->setFixedHeight((ui->horizontalSlider->value()/16)*9);
            button->setIcon(QIcon(QPixmap::fromImage(img.scaled(button->width(),button->height(), Qt::KeepAspectRatio))));
            button->setIconSize(QSize(button->width(),button->height() - 25));
            button->setStyleSheet("QToolButton{border: 0;} QToolButton:hover{background-color:#A9A9A9;}");
            connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
            gridLayout->addWidget(button, i/4, i%4);
        }
    }

    ui->scrollAreaWidgetContents->setLayout(gridLayout);


}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    QImage img;
    for(int i = 0; i<ui->scrollAreaWidgetContents->layout()->count(); i++){
         QWidget *widget = ui->scrollAreaWidgetContents->layout()->itemAt(i)->widget();
         img.load(((MyPushButton *)widget)->imgPath);

         widget->setFixedWidth(position);
         widget->setFixedHeight((position/16)*9);
         ((MyPushButton *)widget)->setIcon(QIcon(QPixmap::fromImage(img.scaled(widget->width(),widget->height(), Qt::KeepAspectRatio))));
         ((MyPushButton *)widget)->setIconSize(QSize(widget->width(),widget->height() - 25));
    }
}

void MainWindow::buttonClicked(){
    QObject * senderObj = sender();
    MyPushButton * button = qobject_cast<MyPushButton*>(senderObj);
    QImage image(button->imgPath);
    QFileInfo file(button->imgPath);
    ui->typeLabel->setText("Type: " + file.completeSuffix());
    ui->heightLabel->setText("Height: "+ QString::number(image.height()));
    ui->widthLabel->setText("Width: "+ QString::number(image.width()));
    ui->sizeLabel->setText("Size: "+ QString::number(file.size() / 1000) + " kB");
    ui->createdLabel->setText("Created: "+ file.created().toString(Qt::DefaultLocaleShortDate));
    ui->imageLabel->setFixedHeight(250);
    ui->imageLabel->setFixedWidth(500);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->imageLabel->width(), ui->imageLabel->height(), Qt::KeepAspectRatio)));
    currentImage = button->imgPath;

}




void MainWindow::on_deleteGroupButton_clicked()
{
    Dialog * dia = new Dialog(&groups);
    QLayoutItem *child;
    QImage img;
    if(dia->exec() == QDialog::Accepted){
        while ((child = ui->groupContents->layout()->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
        delete ui->groupContents->layout();
        QGridLayout * gridLayout = new QGridLayout();

        for(int i=0; i<groups.count(); i++){
            MyPushButton *button = new MyPushButton;

            if(img.load("directory.png")){
                button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                button->setIcon(QIcon(QPixmap::fromImage(img.scaled(button->width(),button->height(), Qt::KeepAspectRatio))));
                button->setIconSize(QSize(60,40));
            }
            button->group = groups[i];
            button->setText(groups[i]);
            connect(button, SIGNAL(clicked()), this, SLOT(groupButtonClicked()));
            QFile input(groups[i]);
            input.open(QIODevice::ReadOnly);
            QTextStream inS(&input);
            while (!inS.atEnd())
            {
               QString line = inS.readLine();
               button->images << line;
            }
            gridLayout->addWidget(button, i/4, i%4);
        }
        ui->groupContents->setLayout(gridLayout);
    }
    QFile outputfile("Groups.cfg");
    outputfile.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!outputfile.isOpen()){
        qDebug() << "- Error, unable to open" << "outputFilename" << "for output";
    }
    QTextStream outStream(&outputfile);
    for(int i = 0; i<groups.count(); i++){
        outStream << groups[i] << "\n";
    }
    outputfile.close();
}

void MainWindow::on_pushButton_clicked()
{
    imageAdder * adder = new imageAdder(&groups);
    QLayoutItem *child;
    QImage img;
    if(adder->exec() == QDialog::Accepted){
        while ((child = ui->groupContents->layout()->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
        delete ui->groupContents->layout();
        QGridLayout * gridLayout = new QGridLayout();

        for(int i=0; i<groups.count(); i++){
            MyPushButton *button = new MyPushButton;
            if(img.load("directory.png")){
                button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                button->setIcon(QIcon(QPixmap::fromImage(img.scaled(button->width(),button->height(), Qt::KeepAspectRatio))));
                button->setIconSize(QSize(60,40));
            }
            button->group = groups[i];
            button->setText(groups[i]);
            QFile input(groups[i]);
            input.open(QIODevice::ReadOnly);
            QTextStream inS(&input);
            connect(button, SIGNAL(clicked()), this, SLOT(groupButtonClicked()));
            while (!inS.atEnd())
            {
               QString line = inS.readLine();
               button->images << line;
            }

            gridLayout->addWidget(button, i/4, i%4);
        }
        ui->groupContents->setLayout(gridLayout);
    }


}

void MainWindow::groupButtonClicked(){

    QObject * senderObj = sender();
    MyPushButton * button = qobject_cast<MyPushButton*>(senderObj);

    QLayoutItem *child;
    QImage img;

    while ((child = ui->scrollAreaWidgetContents->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    delete ui->scrollAreaWidgetContents->layout();
    QGridLayout * gridLayout = new QGridLayout();

    ui->scrollAreaWidgetContents->setLayout(gridLayout);

    for (int i = 0; i<button->images.count(); i++) {

        if (img.load(button->images.at(i))){
            MyPushButton * tmp = new MyPushButton;
            tmp->imgPath = button->images.at(i);

            tmp->setFixedWidth(ui->horizontalSlider->value());
            tmp->setText(button->images.at(i));
            tmp->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            tmp->setFixedHeight((ui->horizontalSlider->value()/16)*9);
            tmp->setIcon(QIcon(QPixmap::fromImage(img.scaled(button->width(),button->height(), Qt::KeepAspectRatio))));
            tmp->setIconSize(QSize(button->width(),button->height() - 25));
            tmp->setStyleSheet("QToolButton{border: 0;} QToolButton:hover{background-color:#A9A9A9;}");
            connect(tmp, SIGNAL(clicked()), this, SLOT(buttonClicked()));
            gridLayout->addWidget(tmp, i/4, i%4);
        }

    }

    ui->scrollAreaWidgetContents->setLayout(gridLayout);
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::buttonDoubleClicked(){
    QMessageBox * box = new QMessageBox();
    box->setText("TEST");
    box->exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(currentImage != ""){
        fullscreen * fscr = new fullscreen(currentImage);
        fscr->setWindowState(fscr->windowState() ^ Qt::WindowFullScreen);
        fscr->show();
    }
}
