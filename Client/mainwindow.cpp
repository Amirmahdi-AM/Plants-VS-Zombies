#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->GameControl->setFixedSize(800,800);

    //////////////////////////////////////////////
    QPixmap Loadingpic(":/Images/LoadingBG.png");

    QPalette palette1;
    palette1.setBrush(QPalette::Window, QBrush(Loadingpic));
    ui->FLoading->setPalette(palette1);
    ui->FLoading->setAutoFillBackground(true);
    //////////////////////////////////////////////

    ui->GameControl->setCurrentIndex(0);
    changetab = new QTimer();
    changetab->start(2000);
    connect(changetab,&QTimer::timeout,this,&MainWindow::ChangetabHandle);

}

void MainWindow::ChangetabHandle(){
    QPixmap Loginpic(":/Images/LoginBG.png");

    QPalette palette2;
    palette2.setBrush(QPalette::Window, QBrush(Loginpic));
    ui->LoginPage->setPalette(palette2);
    ui->LoginPage->setAutoFillBackground(true);
    ui->GameControl->setCurrentIndex(1);
    changetab->stop();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SignUp_clicked()
{
    QPixmap SignUppic(":/Images/SignUpBG.png");

    QPalette palette3;
    palette3.setBrush(QPalette::Window, QBrush(SignUppic));
    ui->Signup->setPalette(palette3);
    ui->Signup->setAutoFillBackground(true);
    ui->GameControl->setCurrentIndex(2);
}

