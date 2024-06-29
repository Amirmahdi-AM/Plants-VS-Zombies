#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QTimer>
#include <QRegularExpression>
#include <QMessageBox>
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


void MainWindow::on_SignupCheck_clicked()
{
    if(ui->lineEdit_3->text().isEmpty()||ui->lineEdit_4->text().isEmpty()||ui->lineEdit_6->text().isEmpty()
            ||ui->lineEdit_7->text().isEmpty()||ui->lineEdit_8->text().isEmpty()){
        QMessageBox::warning(this,"Error", "pleas fill in all the blanks\n");
        return;
    }
    QString output="";
    output+=ui->lineEdit_3->text()+","+ui->lineEdit_4->text()+",";
    QRegularExpression emailRegex(R"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");
    if (!emailRegex.match(ui->lineEdit_6->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid email format\n");
        return;
    }
    output+=ui->lineEdit_6->text()+",";
    if(ui->lineEdit_7->text().length()<8){
        QMessageBox::warning(this,"Error", "Password is weak!!\n");
        return;
    }
    if(ui->lineEdit_7->text()!=ui->lineEdit_8->text()){
        QMessageBox::warning(this,"Error", "Password confirmation failed\n");
        return;
    }
    output+=ui->lineEdit_7->text()+"\n";

}


void MainWindow::on_Back_clicked()
{
    ui->GameControl->setCurrentIndex(1);
}

