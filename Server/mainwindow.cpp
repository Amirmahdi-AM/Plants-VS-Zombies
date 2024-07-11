#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include "QDebug"
#include <QPixmap>
#include <QPalette>
#include <QHostAddress>
#include <QHostInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->setFixedSize(800, 600);

    ///////////////////////////////
    QPixmap server_page(":/images/serverBG.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(server_page));
    ui->page->setPalette(palette);
    ui->page->setAutoFillBackground(true);
    //////////////////////////////
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    ui->connectButton->setText("Connecting...");

    QString _port = ui->portEdit->text();

    Server *server = new Server;
    if (!server->listen(QHostAddress::Any, _port.toInt())) {
        qDebug() << "Failed to connect to server";
    }
    else {
        ui->stackedWidget->setCurrentIndex(1);
        QPixmap connected_page(":/images/connectedBG.png");
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(connected_page));
        ui->page_2->setPalette(palette);
        ui->page_2->setAutoFillBackground(true);

        QString localhostname = QHostInfo::localHostName();

        QString localhostIP;

        QList<QHostAddress> hostList = QHostInfo::fromName(localhostname).addresses();

        foreach (const QHostAddress& address, hostList) {

            if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) {

                localhostIP = address.toString();

            }

        }

        ui->connectedLabel_3->setText(localhostIP);
    }
}

