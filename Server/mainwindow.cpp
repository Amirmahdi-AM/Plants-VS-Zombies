#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    ui->connectButton->setText("Connecting...");

    QString _server = ui->serverEdit->text();
    QString _port = ui->portEdit->text();

    Server server;
    if (!server.listen(QHostAddress(_server), _port.toInt())) {
        qDebug() << "Failed to connect to server";
    }
    else {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

