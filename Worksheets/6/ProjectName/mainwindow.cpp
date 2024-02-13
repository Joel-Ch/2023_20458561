#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::Button2);
    connect(ui->pushButton_2 , &QPushButton::released, this, &MainWindow::Button1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Button1() {
   // This causes MainWindow to emit the signal that will then be received by the statusbar’s slot
   emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
}

void MainWindow::Button2() {
    // This causes MainWindow to emit the signal that will then be received by the statusbar’s slot
    emit statusUpdateMessage(QString("Button 2 was clicked"), 0);
}