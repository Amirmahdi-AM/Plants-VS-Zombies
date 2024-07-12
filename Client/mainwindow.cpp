#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QTimer>
#include <QRegularExpression>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QTransform>
#include <QPropertyAnimation>
#include <stdlib.h>
#include <QPainter>
#include <QCursor>
#include "person.h"
#include <QDebug>
#include <QtAlgorithms>
#include <algorithm>
#include "peashooter.h"
#include "twopeashooter.h"
#include "walnut.h"
#include "boomerang.h"
#include "regularzombie.h"
#include "bucketheadzombie.h"
#include "leafheadzombie.h"
#include "tallzombie.h"
#include "astronautzombie.h"
#include "purplehairzombie.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , connectionTimer(new QTimer(this))
{
    ui->setupUi(this);
    ////////////////////////////////////////////////////////////////////////
    this->setFixedSize(800,800);
    this->move(400,0);
    remainingSeconds=210;
    ui->GameControl->setFixedSize(800,800);
    ui->GameControl->setCurrentIndex(5);
    ui->label_35->hide();
    ui->label_36->hide();
    ui->label_37->hide();
    ui->label_38->hide();
    ui->label_39->hide();
    ui->label_40->hide();
    ui->label_41->hide();
    ui->label_42->hide();
    ui->label_43->hide();
    ui->label_44->hide();
    ui->label_45->hide();
    ui->label_46->hide();
    Massege = new QLabel(this);
    Massege->setGeometry(800,400,54,70);
    Massege->hide();
    ui->ChatRoom->hide();
    ui->Chat_Button->hide();
    QIcon C1Icon(":/Images/C1.png");
    ui->S1->setIcon(C1Icon);
    ui->S1->setIconSize(QSize(54,70));
    QIcon C2Icon(":/Images/C2.png");
    ui->S2->setIcon(C2Icon);
    ui->S2->setIconSize(QSize(54,70));
    QIcon C3Icon(":/Images/C3.png");
    ui->S3->setIcon(C3Icon);
    ui->S3->setIconSize(QSize(54,70));
    QIcon C4Icon(":/Images/C4.png");
    ui->S4->setIcon(C4Icon);
    ui->S4->setIconSize(QSize(54,70));
    QIcon C5Icon(":/Images/C5.png");
    ui->S5->setIcon(C5Icon);
    ui->S5->setIconSize(QSize(54,70));
    QIcon C6Icon(":/Images/C6.png");
    ui->S6->setIcon(C6Icon);
    ui->S6->setIconSize(QSize(54,70));
    QIcon C7Icon(":/Images/ChatPic.png");
    ui->Chat_Button->setIcon(C7Icon);
    ui->Chat_Button->setIconSize(QSize(51,51));
    ////////////////////////////////////////////////////////////////////////
    QPixmap server_page(":/Images/server_connection.png");

    QPalette palette100;
    palette100.setBrush(QPalette::Window, QBrush(server_page));
    ui->server->setPalette(palette100);
    ui->server->setAutoFillBackground(true);
    connect(this,&MainWindow::brainClicked, this, &MainWindow::on_Spawned_Item_Lable_clicked);
    connect(this,&MainWindow::sunClicked, this, &MainWindow::on_Spawned_Item_Lable_clicked);

    /////////////////////////////////////////////////////////////////////////
    ///threads connection
    Rotate = new QTimer();
    connect(Rotate,&QTimer::timeout,this,&MainWindow::Loading_rotation);
    Rotate2 = new QTimer();
    connect(Rotate2,&QTimer::timeout,this,&MainWindow::waiting_rotation);

    Sun_Rotate = new QTimer();
    connect(Sun_Rotate,&QTimer::timeout,this,&MainWindow::sun_rotation);
    fade = new QTimer(this);
    connect(fade,&QTimer::timeout,this,&MainWindow::Fade_Item);

    Labeldrag_drop = new QTimer(this);
    connect(Labeldrag_drop,&QTimer::timeout,this,&MainWindow::Drag_Lable);

    brain_Rotate = new QTimer();
    connect(brain_Rotate,&QTimer::timeout,this,&MainWindow::brain_rotation);
    Item_spawn = new QTimer();
    connect(Item_spawn,&QTimer::timeout,this,&MainWindow::Spawnning_Item);
    checkCollision = new QTimer(this);
    connect(checkCollision,&QTimer::timeout,this,&MainWindow::onCheckcollision);

    Bulletcollision = new QTimer(this);
    connect(Bulletcollision,&QTimer::timeout,this,&MainWindow::onBulletcollision);
    gameTimer = new QTimer(this);
    QObject::connect(gameTimer, &QTimer::timeout, [&]() {
        remainingSeconds--;
        int minutes = remainingSeconds / 60;
        int seconds = remainingSeconds % 60;
        TimerLable->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));

        // Stop the timer when the countdown reaches 0
        if (remainingSeconds == 0) {
            gameTimer->stop();
            QString outpute = "Round"+Player.CurrentRound+","+Player.get_username()+",Plants";
            socket->write(outpute.toUtf8());
        }
    });
    counter = 3;
    ReadyCounter = new QTimer(this);
    QObject::connect(ReadyCounter, &QTimer::timeout, [&]() {
        if(counter==3){
            ui->Counter_Lable->setText("3");
            counter--;
        }
        else if(counter==2){
            ui->Counter_Lable->setText("2");
            counter--;
        }
        else if(counter==1){
            ui->Counter_Lable->setText("1");
            counter--;
        }
        else if(counter==0){
            ui->Counter_Lable->setText("Go");
            counter--;
        }
        else if(counter==-1){
            counter=3;
            ReadyCounter->stop();
            if(P_or_Z==1){
                Plants_set();
            }
            if(P_or_Z== -1){
                Zombies_set();
            }
        }
    });
    ////////////////////////////////////////////////////////////////////////
    /// socket connection
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, QOverload<QTcpSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &MainWindow::onError);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(connectionTimer, &QTimer::timeout, this, &MainWindow::onConnectionTimeout);
    ui->Spawned_brain->setStyleSheet("QPushButton { border: none; }");
    ///////////////////////////////
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BOOM(int _x, int _y,QString type)
{
    if(type=="Mine"){
        for(auto z: zombies){
            if(z->y()>=_y-220&&z->y()<=_y+330&&z->x()>=_x-214&&z->x()<=_x+321){
                z->decreaseHP(500);
                AstronautZombie* AS = dynamic_cast <AstronautZombie*> (z);
                if(!AS){
                    z->WalkingAnimation->stop();
                }
                z->picture.load(":/Images/Charred_Zombie.png");
                z->setPixmap(z->picture);
            }
        }
    }
    if(type=="Jalopino"){
        for(auto z: zombies){
            if(z->y()==_y){
                z->decreaseHP(300);
                AstronautZombie* AS = dynamic_cast <AstronautZombie*> (z);
                if(AS==NULL){
                    z->WalkingAnimation->stop();
                }
                z->picture.load(":/Images/Charred_Zombie.png");
                z->setPixmap(z->picture);
            }
        }
    }
}

void MainWindow::on_SignUp_clicked()
{
    QPixmap SignUppic(":/Images/SignUpBG.png");

    QPalette palette3;
    palette3.setBrush(QPalette::Window, QBrush(SignUppic));
    ui->Signup->setPalette(palette3);
    ui->Signup->setAutoFillBackground(true);
    ui->GameControl->setCurrentIndex(2);

    QPixmap OEyepic2(":/Images/open-eye.png");
    ui->eye_signup->setIcon(QIcon(OEyepic2));
    ui->lineEdit_7->setEchoMode(QLineEdit::Password);
    ui->lineEdit_8->setEchoMode(QLineEdit::Password);
}

void MainWindow::on_SignupCheck_clicked()
{
    if(ui->lineEdit_3->text().isEmpty()||ui->lineEdit_4->text().isEmpty()||ui->lineEdit_6->text().isEmpty()
            ||ui->lineEdit_7->text().isEmpty()||ui->lineEdit_8->text().isEmpty()){
        QMessageBox::warning(this,"Error", "pleas fill in all the blanks\n");
        return;
    }
    QString output="11,";
    QRegularExpression phonenumRegex(R"(\b09\d{9}\b)");
    if (!phonenumRegex.match(ui->lineEdit_5->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid phonenumber format\n");
        return;
    }
    QRegularExpression emailRegex(R"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");
    if (!emailRegex.match(ui->lineEdit_6->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid email format\n");
        return;
    }
    if(ui->lineEdit_7->text().length()<8){
        QMessageBox::warning(this,"Error", "Password is weak!!\n");
        return;
    }
    if(ui->lineEdit_7->text()!=ui->lineEdit_8->text()){
        QMessageBox::warning(this,"Error", "Password confirmation failed\n");
        return;
    }
    output += ui->lineEdit_3->text() + ",";
    output += ui->lineEdit_4->text() + ",";
    output += ui->lineEdit_7->text() + ",";
    output += ui->lineEdit_5->text() + ",";
    output += ui->lineEdit_6->text();

    socket->write(output.toUtf8());

}

int circle=0;
int rotationAngle = 1;
void MainWindow::Loading_rotation(){

    if(circle==2){
        Rotate->stop();
        QPixmap Loginpic(":/Images/LoginBG.png");

        QPalette palette2;
        palette2.setBrush(QPalette::Window, QBrush(Loginpic));
        ui->LoginPage->setPalette(palette2);
        ui->LoginPage->setAutoFillBackground(true);
        ui->GameControl->setCurrentIndex(1);

        QPixmap OEyepic(":/Images/open-eye.png");
        ui->eye_login->setIcon(QIcon(OEyepic));
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    }
    if(rotationAngle==365){
        rotationAngle=0;
        circle++;
    }
    if(rotationAngle%160==0){

        ui->points->setText("");
    }
    if(rotationAngle%160==40){

        ui->points->setText(".");
    }
    if(rotationAngle%160==80){

        ui->points->setText(". .");
    }
    if(rotationAngle%160==120){

        ui->points->setText(". . .");
    }
    QPixmap Sunflower(":/Images/Sunflower.png");
    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedSunflower = Sunflower.transformed(transform);
    ui->SunFlow->setPixmap(rotatedSunflower);
    rotationAngle++;
}

void MainWindow::waiting_rotation(){

    if(rotationAngle==365){
        rotationAngle=0;
    }
    if(rotationAngle%160==0){

        ui->points_2->setText("");
    }
    if(rotationAngle%160==40){

        ui->points_2->setText(".");
    }
    if(rotationAngle%160==80){

        ui->points_2->setText(". .");
    }
    if(rotationAngle%160==120){

        ui->points_2->setText(". . .");
    }
    QPixmap Sunflower(":/Images/Sunflower.png");
    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedSunflower = Sunflower.transformed(transform);
    ui->SunFlow_2->setPixmap(rotatedSunflower);
    rotationAngle++;
}

void MainWindow::decreasePlantsTargets(int _y)
{
    for(auto temp : plants){
        PeaShooter* PSP= dynamic_cast< PeaShooter*> (temp);
        TwoPeaShooter* TPSP= dynamic_cast< TwoPeaShooter*> (temp);
        Boomerang* BP= dynamic_cast< Boomerang*> (temp);
        if(PSP){
            PSP->target--;
        }
        if(TPSP){
            TPSP->target--;
        }
        if(BP){
            BP->target--;
        }
    }

}

void MainWindow::sun_rotation(){

    if(rotationAngle==365){
        rotationAngle=0;
    }
    QPixmap Sun(":/Images/sun.png");
    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedSun = Sun.transformed(transform);
    ui->Moving_sun->setPixmap(rotatedSun);
    rotationAngle++;
}

void MainWindow::brain_rotation(){

    if(rotationAngle==365){
        rotationAngle=0;
    }
    QPixmap brain(":/Images/Brain.png");
    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedbrain = brain.transformed(transform);
    ui->Moving_brain->setPixmap(rotatedbrain);
    rotationAngle++;
}

int clickcount=0;
void MainWindow::on_eye_login_clicked()
{
    if(clickcount==0){
        QPixmap CEyepic(":/Images/close-eye.png");
        ui->eye_login->setIcon(QIcon(CEyepic));
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
        clickcount=1;
        return;
    }
    if(clickcount==1){
        QPixmap OEyepic(":/Images/open-eye.png");
        ui->eye_login->setIcon(QIcon(OEyepic));
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
        clickcount=0;
        return;
    }
}

int clickcount2=0;
void MainWindow::on_eye_signup_clicked()
{
    if(clickcount2==0){
        QPixmap CEyepic(":/Images/close-eye.png");
        ui->eye_signup->setIcon(QIcon(CEyepic));
        ui->lineEdit_7->setEchoMode(QLineEdit::Normal);
        clickcount2=1;
        return;
    }
    if(clickcount2==1){
        QPixmap OEyepic(":/Images/open-eye.png");
        ui->eye_signup->setIcon(QIcon(OEyepic));
        ui->lineEdit_7->setEchoMode(QLineEdit::Password);
        clickcount2=0;
        return;
    }
}

void MainWindow::on_forgot_pass_clicked()
{
    QPixmap ResetPass(":/Images/PasswordReset.png");

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(ResetPass));
    ui->RestorePass->setPalette(palette);
    ui->RestorePass->setAutoFillBackground(true);
    ui->GameControl->setCurrentIndex(3);
}

void MainWindow::on_Back_clicked()
{
    ui->GameControl->setCurrentIndex(1);
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
}

void MainWindow::on_Ok_RPass_clicked()
{
    if(ui->lineEdit_9->text().isEmpty()||ui->lineEdit_10->text().isEmpty()){
        QMessageBox::warning(this,"Error", "pleas fill in all the blanks\n");
        return;
    }
    QString output="15,";
    QRegularExpression phonenumRegex(R"(\b09\d{9}\b)");
    if (!phonenumRegex.match(ui->lineEdit_9->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid phonenumber format\n");
        return;
    }
    output+=ui->lineEdit_9->text()+",";
    QRegularExpression emailRegex(R"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");
    if (!emailRegex.match(ui->lineEdit_10->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid email format\n");
        return;
    }

    output+=ui->lineEdit_10->text();
    socket->write(output.toUtf8());
    //cheking in server if was true.goes to set new password page

}

void MainWindow::on_Back_newpass_2_clicked()
{
    ui->GameControl->setCurrentIndex(3);
    ui->lineEdit_11->setText("");
    ui->lineEdit_12->setText("");
}

void MainWindow::on_Back_newpass_clicked()
{
    ui->GameControl->setCurrentIndex(1);
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
}

void MainWindow::on_Ok_newpass_clicked()
{
    if(ui->lineEdit_11->text().isEmpty()||ui->lineEdit_12->text().isEmpty()){
        QMessageBox::warning(this,"Error", "pleas fill in all the blanks\n");
        return;
    }
    QString output="16,"+ui->lineEdit_9->text()+","+ui->lineEdit_10->text()+",";
    if(ui->lineEdit_11->text().length()<8){
        QMessageBox::warning(this,"Error", "Password is weak!!\n");
        return;
    }
    if(ui->lineEdit_12->text()!=ui->lineEdit_11->text()){
        QMessageBox::warning(this,"Error", "Password confirmation failed\n");
        return;
    }

    output+=ui->lineEdit_11->text();
    socket->write(output.toUtf8());
}

void MainWindow::on_connectButton_clicked()
{
    socket->abort();
    socket->connectToHost(ui->IPEdit->text(), ui->portEdit->text().toUShort());
    connectionTimer->start(2000);
}

void MainWindow::Plants_set(){
    ui->GameControl->setCurrentIndex(7);
    this->setFixedSize(1500,800);
    this->move(15,0);
    ui->GameControl->setFixedSize(1500,800);
    remainingSeconds=210;
    /////////////////////////////////////////
    QPixmap GroundPic(":/Images/PlantsField.png");
    ui->Ground->setFixedSize(1500,700);
    ui->Ground->setPixmap(GroundPic);
    ui->label_24->setPixmap(QPixmap(":/Images/Peashooter_P_Card_Faded.png"));
    ui->label_30->setPixmap(QPixmap(":/Images/two_peashooter_P_card_Faded.png"));
    ui->label_31->setPixmap(QPixmap(":/Images/Walnut_P_Card_Faded.png"));
    ui->label_34->setPixmap(QPixmap(":/Images/Boomerang_P_Card_Faded.png"));
    ui->label_33->setPixmap(QPixmap(":/Images/Jalapeno_P_Card_Faded.png"));
    ui->label_32->setPixmap(QPixmap(":/Images/Plum_Mine_P_Card_Faded.png"));
    /////////////////////////////////////////
    QPixmap Sun(":/Images/sun.png");
    ui->Moving_sun->setFixedSize(90,74);
    ui->Moving_sun->setPixmap(Sun);
    Sun_Rotate->start(10);
    Item_spawn->start(6000);
    gameTimer->start(1000);
    /////////////////////////////////////////
    ui->Plants_point_Label->setText(QString("%1").arg(Player.Point));
    TimerLable = ui->Plant_Timer;
    ui->Chat_Button->show();
}

void MainWindow::Zombies_set(){
    ui->GameControl->setCurrentIndex(9);
    this->setFixedSize(1500,800);
    this->move(15,0);
    ui->GameControl->setFixedSize(1500,800);
    remainingSeconds=210;

    /////////////////////////////////////////
    QPixmap GroundPic(":/Images/ZombiesField.png");
    ui->Ground_2->setFixedSize(1500,700);
    ui->Ground_2->setPixmap(GroundPic);
    ui->label_23->setPixmap(QPixmap(":/Images/Regular_Z_Card_Faded.png"));
    ui->label_25->setPixmap(QPixmap(":/Images/LeafHead_Z_Card_Faded.png"));
    ui->label_28->setPixmap(QPixmap(":/Images/Tall_Z_Card_Faded.png"));
    ui->label_27->setPixmap(QPixmap(":/Images/Austronut_Z_Card_Faded.png"));
    ui->label_26->setPixmap(QPixmap(":/Images/Bucket_Z_Card_Faded.png"));
    ui->label_29->setPixmap(QPixmap(":/Images/PurpleHead_Z_Card_Faded.png"));
    /////////////////////////////////////////
    QPixmap brain(":/Images/Brain.png");
    ui->Moving_brain->setFixedSize(90,74);
    ui->Moving_brain->setPixmap(brain);
    brain_Rotate->start(10);
    Item_spawn->start(6000);
    gameTimer->start(1000);
    /////////////////////////////////////////
    ui->Spawned_brain->setGeometry(-10,-10,80,74);
    ui->Zombies_point_Lable->setText(QString("%1").arg(Player.Point));
    TimerLable = ui->Zombies_Timer;
    ui->Chat_Button->show();
}

void MainWindow::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString response(data);

    QStringList fields = response.split(",");

    if (fields[0] == "111"){
        QMessageBox::critical(this, "Signup", "This username already token!");
    }
    else if (fields[0] == "0"){
        QString output="PlayerUsername,"+Player.get_username();
        socket->write(output.toUtf8());
        P_or_Z = 1;
        checkCollision->start(200);
        Bulletcollision->start(50);
        currentMap = ui->Plants_map;
        //ui->
        QPixmap Ready_page(":/Images/PlantsGetReady.jpg");
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(Ready_page));
        ui->getReady->setPalette(palette);
        ui->getReady->setAutoFillBackground(true);
        ui->GameControl->setCurrentIndex(12);
        ReadyCounter->start(1000);
    }
    else if (fields[0] == "1"){
        QString output="PlayerUsername,"+Player.get_username();
        socket->write(output.toUtf8());
        P_or_Z = -1;
        checkCollision->start(200);
        Bulletcollision->start(50);
        currentMap = ui->Zombies_map;
        QPixmap Ready_page(":/Images/ZombieGetReady.jpg");
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(Ready_page));
        ui->getReady->setPalette(palette);
        ui->getReady->setAutoFillBackground(true);
        ui->GameControl->setCurrentIndex(12);
        ReadyCounter->start(1000);
    }
    else if(fields[0] == "NextRound"){
        /*if(fields[2]==Player.get_username()){
            //you won
        }
        else{
            //fields[2] won
        }*/
        Player.Point =0;
        Player.CurrentRound ="2";
        Peavec.clear();
        BPeavec.clear();
        plants.clear();
        zombies.clear();
        fullLocations.clear();
        spawnedItemp_Label->show();
        if(P_or_Z==1){
            P_or_Z = -1;
            checkCollision->start(200);
            Bulletcollision->start(50);
            currentMap = ui->Zombies_map;
            QPixmap Ready_page(":/Images/ZombieGetReady.jpg");
            QPalette palette;
            palette.setBrush(QPalette::Window, QBrush(Ready_page));
            ui->getReady->setPalette(palette);
            ui->getReady->setAutoFillBackground(true);
            ui->GameControl->setCurrentIndex(12);
            ReadyCounter->start(1000);
        }
        else if(P_or_Z==-1){
            P_or_Z = 1;
            checkCollision->start(200);
            Bulletcollision->start(50);
            currentMap = ui->Plants_map;
            QPixmap Ready_page(":/Images/PlantsGetReady.jpg");
            QPalette palette;
            palette.setBrush(QPalette::Window, QBrush(Ready_page));
            ui->getReady->setPalette(palette);
            ui->getReady->setAutoFillBackground(true);
            ui->GameControl->setCurrentIndex(12);
            ReadyCounter->start(1000);
        }
 }

    else if (fields[0] == "113"){
        Player.set_name(fields[3]);
        Player.set_username(fields[1]);
        Player.set_password(fields[2]);
        Player.set_phoneNumber(fields[4]);
        Player.set_email(fields[5]);
        Player.winRound = 0;
        Player.CurrentRound= "1";
        Player.Point = 0;
        ui->GameControl->setCurrentIndex(6);
        ////////////////////////////////////////
        QPixmap MenuBackground(":/Images/MenuBG.jpg");

        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(MenuBackground));
        ui->Menu->setPalette(palette);
        ui->Menu->setAutoFillBackground(true);
    }

    else if (fields[0] == "114"){
        QMessageBox::critical(this, "Signin", "Incorrect username/password!");
    }
    else if (fields[0] == "1151"){
        ui->GameControl->setCurrentIndex(4);
        QPixmap ResetPass(":/Images/PasswordReset.png");

        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(ResetPass));
        ui->SetnewPass->setPalette(palette);
        ui->SetnewPass->setAutoFillBackground(true);
    }
    else if (fields[0] == "1152"){
        QMessageBox::critical(this, "Reset password", "Incorrect Emial/Phonenumber!");
    }
    else if (fields[0] == "116"){
        QMessageBox::information(this, "Pass changed", "Password successfully changed");
        ui->GameControl->setCurrentIndex(1);
    }
    else if (fields[0] == "card"){

        if(fields[1][fields[1].size() - 1] == 'P') {
            int Targetcounts = 0;
            for(auto temp : zombies){

                if(temp->y()==fields[3].toInt()&&temp->x()>=fields[2].toInt()){
                    Targetcounts++;
                }
            }
            if (fields[1] == "PSP"){
                PeaShooter* ps = new PeaShooter(fields[2].toInt(),fields[3].toInt(), currentMap);
                plants.push_back(ps);
                ps->target += Targetcounts;
                connect(ps, &PeaShooter::createPea, this, &MainWindow::onCreateBullets);
            }
            if (fields[1] == "TPSP"){
                TwoPeaShooter* tps = new TwoPeaShooter(fields[2].toInt(),fields[3].toInt(), currentMap);
                plants.push_back(tps);
                tps->target += Targetcounts;
                connect(tps, &TwoPeaShooter::createPea, this, &MainWindow::onCreateBullets);
            }
            if (fields[1] == "WP"){
                Walnut* w = new Walnut(fields[2].toInt(),fields[3].toInt(), currentMap);
                plants.push_back(w);
            }
            if (fields[1] == "PMP"){

                BOOM(fields[2].toInt(),fields[3].toInt(),"Mine");
            }
            if (fields[1] == "JP"){

                BOOM(fields[2].toInt(),fields[3].toInt(),"Jalopino");
            }
            if (fields[1] == "BP"){
                Boomerang* bm = new Boomerang(fields[2].toInt(),fields[3].toInt(), currentMap);
                plants.push_back(bm);
                bm->target += Targetcounts;
                connect(bm, &Boomerang::createBBullet, this, &MainWindow::onCreateBBullets);
            }
        }

        if(fields[1][fields[1].size() - 1] == 'Z') {
            for(auto temp : plants){
                if(temp->y()==fields[3].toInt()){
                    PeaShooter* PSP= dynamic_cast< PeaShooter*> (temp);
                    TwoPeaShooter* TPSP= dynamic_cast< TwoPeaShooter*> (temp);
                    Boomerang* BP= dynamic_cast< Boomerang*> (temp);
                    if(PSP){
                        PSP->target++;
                    }
                    if(TPSP){
                        TPSP->target++;
                    }
                    if(BP){
                        BP->target++;
                    }
                }
            }
            if (fields[1] == "RZ"){
                RegularZombie* rz = new RegularZombie(fields[2].toInt(),fields[3].toInt(), currentMap);
                zombies.push_back(rz);
                connect(rz, &Zombies::cleanLocation, this, &MainWindow::onCleanLocation);
            }
            if (fields[1] == "BHZ"){
                BucketHeadZombie* bhz = new BucketHeadZombie(fields[2].toInt(),fields[3].toInt(), currentMap);
                zombies.push_back(bhz);
                connect(bhz, &Zombies::cleanLocation, this, &MainWindow::onCleanLocation);
            }
            if (fields[1] == "LHZ"){
                LeafHeadZombie* lhz = new LeafHeadZombie(fields[2].toInt(),fields[3].toInt(), currentMap);
                zombies.push_back(lhz);
                connect(lhz, &Zombies::cleanLocation, this, &MainWindow::onCleanLocation);
            }
            if (fields[1] == "TZ"){
                TallZombie* tz = new TallZombie(fields[2].toInt(),fields[3].toInt(), currentMap);
                zombies.push_back(tz);
                connect(tz, &Zombies::cleanLocation, this, &MainWindow::onCleanLocation);
            }
            if (fields[1] == "AZ"){
                AstronautZombie* az = new AstronautZombie(fields[2].toInt(),fields[3].toInt(), currentMap);
                zombies.push_back(az);
                connect(az, &Zombies::cleanLocation, this, &MainWindow::onCleanLocation);
            }
            if (fields[1] == "PHZ"){
                PurpleHairZombie* phz = new PurpleHairZombie(fields[2].toInt(),fields[3].toInt(), currentMap);
                zombies.push_back(phz);
                connect(phz, &Zombies::cleanLocation, this, &MainWindow::onCleanLocation);
            }
        }

        if(fields[1] == "Showel"){
            for(auto temp : plants){
                if(temp->y()==fields[3].toInt()&&temp->x()==fields[2].toInt()){
                    temp->setGeometry(-200, -200, 100, 100);
                    onCleanLocation(fields[2].toInt(),fields[3].toInt());
                }
            }

        }
    }
    else if (fields[0] == "His") {
        ui->GameControl->setCurrentIndex(11);
        QPixmap historyPagepic(":/Images/LoginBG.png");
        QPalette palette2;
        palette2.setBrush(QPalette::Window, QBrush(historyPagepic));
        ui->historyPage->setPalette(palette2);
        ui->historyPage->setAutoFillBackground(true);
        ui->listWidget->clear();
        ui->listWidget->addItem(fields[1]);
    }
    else if(fields[0] == "Chat"){
        Massege->setPixmap(QPixmap(fields[1]));
        if(Massege->isHidden()){
            Massege->show();
        }
        QPropertyAnimation *animation = new QPropertyAnimation(Massege, "geometry");
        animation->setDuration(2000);
        QPoint MassegePos = Massege->pos();
        int x = MassegePos.x();
        int y = MassegePos.y();
        animation->setStartValue(QRect(x, y, 54,70));
        animation->setEndValue(QRect(x, -50, 54,70));
        QObject::connect(animation, &QPropertyAnimation::finished, Massege,[&](){
            Massege->hide();
            Massege->setGeometry(800,400,54,70);
        });
        animation->start();
    }
    else if(fields[0] == "EOG"){
        this->setFixedSize(800, 800);
        ui->GameControl->setFixedSize(800, 800);
        this->move(400,0);
        Player.Point =0;
        Player.CurrentRound ="1";
        Peavec.clear();
        BPeavec.clear();
        plants.clear();
        zombies.clear();
        fullLocations.clear();
        ui->GameControl->setCurrentIndex(6);
    }

}


void MainWindow::onConnected() {
    connectionTimer->stop();
    QPixmap Loadingpic(":/Images/LoadingBG.png");

    QPalette palette1;
    palette1.setBrush(QPalette::Window, QBrush(Loadingpic));
    ui->FLoading->setPalette(palette1);
    ui->FLoading->setAutoFillBackground(true);

    ui->forgot_pass->setAutoFillBackground(false);
    QPalette pal = ui->forgot_pass->palette();
    pal.setColor(QPalette::Button, Qt::transparent);
    ui->forgot_pass->setPalette(pal);
    ui->GameControl->setCurrentIndex(0);

    Rotate->start(10);
}

void MainWindow::onError(QTcpSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    if (connectionTimer->isActive()) {
        connectionTimer->stop();
        QMessageBox::warning(this, "Error", "Failed to connect");
    }
}

void MainWindow::onConnectionTimeout() {
    if (socket->state() != QTcpSocket::ConnectedState) {
        socket->abort();
        connectionTimer->stop();
        QMessageBox::warning(this, "Error", "Failed to connect: Timeout");
    }
}

void MainWindow::on_Ok_clicked()
{
    QString output = "12,";
    output += ui->lineEdit->text() + ",";
    output += ui->lineEdit_2->text();
    socket->write(output.toUtf8());
}

void MainWindow::Spawnning_Item(){
    QPropertyAnimation *animation;
    if(P_or_Z==1){
        ui->Spawned_sun->setFixedSize(80,74);
        ui->Spawned_sun->setStyleSheet("background-image: url(:/Images/sun.png);");
        ui->Spawned_sun->setGeometry(-1, -1, 80,74);
       animation = new QPropertyAnimation(ui->Spawned_sun, "geometry");
       if(!ui->Spawned_sun->isVisible()){
           ui->Spawned_sun->show();
       }
       fade->start(3000);
    }
    if(P_or_Z==-1){
        ui->Spawned_brain->setFixedSize(75,74);
        ui->Spawned_brain->setStyleSheet("background-image: url(:/Images/Brain.png);");
        ui->Spawned_brain->setGeometry(-1, -1, 75,74);
        animation = new QPropertyAnimation(ui->Spawned_brain, "geometry");
        if(!ui->Spawned_brain->isVisible()){
            ui->Spawned_brain->show();
        }
        fade->start(3000);
    }
    animation->setDuration(2000);
    int x = rand() % 1200 + 200;
    int y = rand() % 550 + 200;
    animation->setStartValue(QRect(x, -1, 75,74));
    animation->setEndValue(QRect(x, y, 75,74));
    animation->start();
}

void MainWindow::Fade_Item(){
    if(P_or_Z==1){
        ui->Spawned_sun->setStyleSheet("background-image: url(:/Images/FadedSun.png);");
    }
    if(P_or_Z==-1){
        ui->Spawned_brain->setStyleSheet("background-image: url(:/Images/FadeBrain.png);");     
    }
    if (fade->isActive()){
        fade->stop();
    }
}

void MainWindow::on_Start_Game_Botton_clicked()
{
    QString output = "13," + Player.get_username();
    socket->write(output.toUtf8());
    ui->GameControl->setCurrentIndex(8);
    this->setFixedSize(1500,800);
    this->move(15,0);
    ui->GameControl->setFixedSize(1500,800);

    QPixmap WaitingPic(":/Images/WaitingBG.jpg");
    ui->Waiting_Lable->setFixedSize(1500,800);
    ui->Waiting_Lable->move(0,0);
    ui->Waiting_Lable->setPixmap(WaitingPic);
    Rotate2->start(10);

}

void MainWindow::on_Spawned_Item_Lable_clicked()
{
    if (fade->isActive()){
        fade->stop();
    }

    spawnedItemp_Label->setWindowFlags(Qt::FramelessWindowHint);
    QPropertyAnimation *animation = new QPropertyAnimation(spawnedItemp_Label, "geometry");
    animation->setDuration(1000);
    QPoint Spawned_ItemPos = spawnedItemp_Label->pos();
    int x = Spawned_ItemPos.x();
    int y = Spawned_ItemPos.y();
    animation->setStartValue(QRect(x, y, 100,100));
    animation->setEndValue(QRect(13, 10, 100,100));
    QObject::connect(animation, &QPropertyAnimation::finished, spawnedItemp_Label,[&](){
        if(P_or_Z==-1){
            ui->Zombies_point_Lable->setText(QString("%1").arg(Player.Point));
            if(Player.Point>=100){
                ui->label_23->setPixmap(QPixmap(":/Images/Regular_Z_Card.png"));
            }
            if(Player.Point>=150){
                ui->label_25->setPixmap(QPixmap(":/Images/LeafHead_Z_Card.png"));
                ui->label_28->setPixmap(QPixmap(":/Images/Tall_Z_Card.png"));
            }
            if(Player.Point>=200){
                ui->label_26->setPixmap(QPixmap(":/Images/Bucket_Z_Card.png"));
                ui->label_27->setPixmap(QPixmap(":/Images/Austronut_Z_Card.png"));
            }
            if(Player.Point>=800){
                ui->label_29->setPixmap(QPixmap(":/Images/PurpleHead_Z_Card.png"));
            }
        }
        if(P_or_Z == 1){
            ui->Plants_point_Label->setText(QString("%1").arg(Player.Point));
            if(Player.Point>=50){
                ui->label_24->setPixmap(QPixmap(":/Images/Peashooter_P_Card.png"));
            }
            if(Player.Point>=100){
                ui->label_30->setPixmap(QPixmap(":/Images/two_peashooter_P_card.png"));
                ui->label_31->setPixmap(QPixmap(":/Images/Walnut_P_Card.png"));
            }
            if(Player.Point>=125){
                ui->label_34->setPixmap(QPixmap(":/Images/Boomerang_P_Card.png"));
            }
            if(Player.Point>=150){
                ui->label_33->setPixmap(QPixmap(":/Images/Jalapeno_P_Card.png"));
            }
            if(Player.Point>=175){
                ui->label_32->setPixmap(QPixmap(":/Images/Plum_Mine_P_Card.png"));
            }
        }
        spawnedItemp_Label->hide();
    });
    animation->start();


}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (P_or_Z == -1){
            if (event->x() >= ui->Spawned_brain->x() &&  event->x() <= ui->Spawned_brain->x() + 80 && event->y() >= ui->Spawned_brain->y() && event->y() <= ui->Spawned_brain->y() + 74){
                spawnedItemp_Label = ui->Spawned_brain;
                emit brainClicked();
                Player.Point += 30;
            }

            if (event->x() >= ui->label_29->x() && event->x() <= ui->label_29->x() + ui->label_29->width() && event->y() >= ui->label_29->y() && event->y() <= ui->label_29->y() + ui->label_29->height()&& Player.Point>=800){
                draging_Label = ui->label_36;
                draging_Label->show();
                selection = 1;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_28->x() && event->x() <= ui->label_28->x() + ui->label_28->width() && event->y() >= ui->label_28->y() && event->y() <= ui->label_28->y() + ui->label_28->height()&& Player.Point>=150){
                draging_Label = ui->label_39;
                draging_Label->show();
                selection = 2;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_27->x() && event->x() <= ui->label_27->x() + ui->label_27->width() && event->y() >= ui->label_27->y() && event->y() <= ui->label_27->y() + ui->label_27->height()&& Player.Point>=200){
                draging_Label = ui->label_40;
                draging_Label->show();
                selection = 3;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_26->x() && event->x() <= ui->label_26->x() + ui->label_26->width() && event->y() >= ui->label_26->y() && event->y() <= ui->label_26->y() + ui->label_26->height()&& Player.Point>=200){
                draging_Label = ui->label_37;
                draging_Label->show();
                selection = 4;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_25->x() && event->x() <= ui->label_25->x() + ui->label_25->width() && event->y() >= ui->label_25->y() && event->y() <= ui->label_25->y() + ui->label_25->height()&& Player.Point>=150){
                draging_Label = ui->label_38;
                draging_Label->show();
                selection = 5;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_23->x() && event->x() <= ui->label_23->x() + ui->label_23->width() && event->y() >= ui->label_23->y() && event->y() <= ui->label_23->y() + ui->label_23->height()&& Player.Point>=100){
                draging_Label = ui->label_35;
                draging_Label->show();
                selection = 6;
                Labeldrag_drop->start(1);
            }

        }
        if (P_or_Z == 1){
            if (event->x() >= ui->Spawned_sun->x() && event->x() <= ui->Spawned_sun->x() + 80 && event->y() >= ui->Spawned_sun->y() && event->y() <= ui->Spawned_sun->y() + 74){
                spawnedItemp_Label = ui->Spawned_sun;
                emit sunClicked();
                Player.Point += 30;
            }

            if (event->x() >= ui->label_24->x() && event->x() <= ui->label_24->x() + ui->label_24->width() && event->y() >= ui->label_24->y() && event->y() <= ui->label_24->y() + ui->label_24->height()&& Player.Point>=50){
                draging_Label = ui->label_41;
                draging_Label->show();
                selection = 1;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_30->x() && event->x() <= ui->label_30->x() + ui->label_30->width() && event->y() >= ui->label_30->y() && event->y() <= ui->label_30->y() + ui->label_30->height() && Player.Point>=100){
                draging_Label = ui->label_42;
                draging_Label->show();
                selection = 2;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_31->x() && event->x() <= ui->label_31->x() + ui->label_31->width() && event->y() >= ui->label_31->y() && event->y() <= ui->label_31->y() + ui->label_31->height() && Player.Point>=100){
                draging_Label = ui->label_43;
                draging_Label->show();
                selection = 3;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_32->x() && event->x() <= ui->label_32->x() + ui->label_32->width() && event->y() >= ui->label_32->y() && event->y() <= ui->label_32->y() + ui->label_32->height() && Player.Point>=175){
                draging_Label = ui->label_44;
                draging_Label->show();
                selection = 4;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_33->x() && event->x() <= ui->label_33->x() + ui->label_33->width() && event->y() >= ui->label_33->y() && event->y() <= ui->label_33->y() + ui->label_33->height() && Player.Point>=150){
                draging_Label = ui->label_45;
                draging_Label->show();
                selection = 5;
                Labeldrag_drop->start(1);
            }
            if (event->x() >= ui->label_34->x() && event->x() <= ui->label_34->x() + ui->label_34->width() && event->y() >= ui->label_34->y() && event->y() <= ui->label_34->y() + ui->label_34->height() && Player.Point>=125){
                draging_Label = ui->label_46;
                draging_Label->show();
                selection = 6;
                Labeldrag_drop->start(1);
            }
            ///showel
            if (event->x() >= ui->label_56->x() && event->x() <= ui->label_56->x() + ui->label_56->width() && event->y() >= ui->label_56->y() && event->y() <= ui->label_56->y() + ui->label_56->height()){
                draging_Label = ui->label_55;
                draging_Label->show();
                selection = 7;
                Labeldrag_drop->start(1);
            }
        }
    }
    QWidget::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Labeldrag_drop->stop();
    if (P_or_Z == 1) {
        if (selection) {
            int x ,y;
            bool validate = true;
            x = event->x();
            y = event->y();

            if (x >= 165 && x < 271) {
                x= 165;
            }
            else if (x >= 271 && x < 378){
                x = 271;
            }
            else if (x >= 378 && x< 485){
                x = 378;
            }
            else if (x >= 485 && x< 592){
                x = 485;
            }
            else if (x >= 592 && x< 699){
                x = 592;
            }
            else if (x >= 699 && x< 806){
                x = 699;
            }
            else if(selection == 5 || selection == 4){
                if (x >= 806 && x< 913){
                    x = 806;
                }
                else if (x >= 913 && x< 1020){
                    x = 913;
                }
                else if (x >= 1020 && x< 1127){
                    x = 1020;
                }
                else if (x >= 1127 && x< 1234){
                    x = 1127;
                }
                else if (x >= 1234 && x< 1341){
                    x = 1234;
                }
            }
            else {
                validate = false;
            }
            if (y >= 135 && y < 225)
                y = 120;
            else if (y >= 225 && y < 325)
                y = 230;
            else if (y >= 325 && y < 425)
                y = 330;
            else if (y >= 425 && y < 525)
                y = 430;
            else if (y >= 525 && y < 625)
                y = 530;
            else if (y >= 625 && y < 725)
                y = 640;
            else {
                validate = false;
            }
            if(selection!=7){
                for (auto temp : fullLocations) {
                    if (temp.first == x && temp.second == y)
                        validate = false;
                }
            }
            draging_Label->setGeometry(740, 10, 100, 100);
            if (validate) {
                if(selection != 5 && selection != 4 && selection != 7){
                 fullLocations.push_back(make_pair(x,y));
                }
                QString output = "card,";
                if (selection == 1){
                    output += "PSP,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 50;
                }

                if (selection == 2){
                    output += "TPSP,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 100;
                }

                if (selection == 3){
                    output += "WP,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 100;
                }

                if (selection == 4){
                    output += "PMP,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 175;
                }

                if (selection == 5){
                    output += "JP,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 150;
                }

                if (selection == 6){
                    output += "BP,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 125;
                }
                if (selection == 7){
                    output += "Showel,";
                    output += QString::number(x)+","+QString::number(y);
                }
                socket->write(output.toUtf8());

            }
            ui->Plants_point_Label->setText(QString("%1").arg(Player.Point));
            if(Player.Point<50){
                ui->label_24->setPixmap(QPixmap(":/Images/Peashooter_P_Card_Faded.png"));
            }
            if(Player.Point<100){
                ui->label_30->setPixmap(QPixmap(":/Images/two_peashooter_P_card_Faded.png"));
                ui->label_31->setPixmap(QPixmap(":/Images/Walnut_P_Card_Faded.png"));
            }
            if(Player.Point<125){
                ui->label_34->setPixmap(QPixmap(":/Images/Boomerang_P_Card_Faded.png"));
            }
            if(Player.Point<150){
                ui->label_33->setPixmap(QPixmap(":/Images/Jalapeno_P_Card_Faded.png"));
            }
            if(Player.Point<175){
                ui->label_32->setPixmap(QPixmap(":/Images/Plum_Mine_P_Card_Faded.png"));
            }
            draging_Label->hide();
            selection = 0;
        }
    }
    if (P_or_Z == -1) {
        if (selection) {
            int x ,y;
            bool validate = true;
            x = event->x();
            y = event->y();
            if (x >= 1347 && x<= 1447){
                x = 1347;
            }
            else {
                validate = false;
            }
            if (y >= 135 && y < 225)
                y = 120;
            else if (y >= 225 && y < 325)
                y = 230;
            else if (y >= 325 && y < 425)
                y = 330;
            else if (y >= 425 && y < 525)
                y = 430;
            else if (y >= 525 && y < 625)
                y = 530;
            else if (y >= 625 && y < 725)
                y = 640;
            else {
                validate = false;
            }
            if(validate){
                    QString output = "card,";
                if (selection == 6){
                    output += "RZ,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 100;
                }

                if (selection == 4){
                    output += "BHZ,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 200;
                }

                if (selection == 5){
                    output += "LHZ,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 150;
                }

                if (selection == 2){
                    output += "TZ,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 150;
                }

                if (selection == 3){
                    output += "AZ,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 200;
                }

                if (selection == 1){
                    output += "PHZ,";
                    output += QString::number(x)+","+QString::number(y);
                    Player.Point -= 800;
                }
                socket->write(output.toUtf8());

            }
            ui->Zombies_point_Lable->setText(QString("%1").arg(Player.Point));
            if(Player.Point<100){
                ui->label_23->setPixmap(QPixmap(":/Images/Regular_Z_Card_Faded.png"));
            }
            if(Player.Point<150){
                ui->label_25->setPixmap(QPixmap(":/Images/LeafHead_Z_Card_Faded.png"));
                ui->label_28->setPixmap(QPixmap(":/Images/Tall_Z_Card_Faded.png"));
            }
            if(Player.Point<200){
                ui->label_27->setPixmap(QPixmap(":/Images/Austronut_Z_Card_Faded.png"));
                ui->label_26->setPixmap(QPixmap(":/Images/Bucket_Z_Card_Faded.png"));
            }
            if(Player.Point<800){
                ui->label_29->setPixmap(QPixmap(":/Images/PurpleHead_Z_Card_Faded.png"));
            }
            draging_Label->hide();
            selection = 0;
        }
    }
}

void MainWindow::Drag_Lable(){
    QPoint pos = QCursor::pos();
    draging_Label->setGeometry(pos.x()-60,pos.y()-60,draging_Label->width(),draging_Label->height());
}

void MainWindow::onCreateBullets(int x, int y, int _power)
{
    Pea *bullet = new Pea(x, y, _power,currentMap);
    Peavec.push_back(bullet);
}

void MainWindow::onCheckcollision()
{
   for( auto z:zombies){
       if(z->target!= NULL){
           continue;
       }
       for(auto p:plants){
           if (p->geometry().intersects(z->geometry())) {
               Walnut* W = dynamic_cast<Walnut*>(p);
               TallZombie* tz = dynamic_cast<TallZombie*>(z);
               if (W && tz) {
                   tz->setGeometry(tz->x() - 210, tz->y(), 100, 100);
               }
               else {
               z->offMovement();
               z->target = p;
               }
           }
       }
   }
}

void MainWindow::onCleanLocation(int x, int y)
{
    for(auto temp : plants){
        if(temp->y()==y&&temp->x()==x){
            plants.erase(std::find(plants.begin(),plants.end(), temp));
            //delete temp;
            break;
        }
    }
    auto it = std::find(fullLocations.begin(), fullLocations.end(), make_pair(x,y));
    if (it != fullLocations.end()) {
        fullLocations.erase(it);
    }
}

void MainWindow::onCreateBBullets(int x, int y, int _power)
{
    BoomerangPea *bullet = new BoomerangPea(x, y, _power,currentMap);
    BPeavec.push_back(bullet);
}

void MainWindow::onBulletcollision()
{
    for(auto temp:zombies){
       if(temp->x()<=100) {
           checkCollision->stop();
           Bulletcollision->stop();
           Item_spawn->stop();
           if(spawnedItemp_Label->isVisible()){
               spawnedItemp_Label->hide();
           }

           for(auto DZ:zombies){
              delete DZ;
           }
           for(auto DP:plants){
              delete DP;
           }
           for(auto DPea:Peavec){
              delete DPea;
           }
           for(auto DBP:BPeavec){
              delete DBP;
           }
           if(P_or_Z==-1){
               QString output = "Round"+Player.CurrentRound+",";
               output+=Player.get_username();
               output+=",Zombies";
               socket->write(output.toUtf8());

           }
           return;
       }
    }
    for(auto p : Peavec) {
        if (p->x() > 1500) {
            Peavec.erase(std::find(Peavec.begin(),Peavec.end(), p));
            delete p;
        }
    }
    for(auto PeaBullet : Peavec) {
        for(auto Z : zombies) {
            if(PeaBullet && PeaBullet->geometry().intersects(Z->geometry())){
                Z->decreaseHP(PeaBullet->getPower());
                if(Z->getHp()<=0){
                    decreasePlantsTargets(Z->y());
                    zombies.erase(std::find(zombies.begin(),zombies.end(),Z));
                    delete Z;
                }
                Peavec.erase(std::find(Peavec.begin(),Peavec.end(),PeaBullet));
                delete PeaBullet;
                break;
            }
        }
    }
    ////////////////////////////////////////////////////////////////////
        for(auto bp : BPeavec) {
            if (bp->x() > 1500) {
                BPeavec.erase(std::find(BPeavec.begin(),BPeavec.end(), bp));
                delete bp;
            }
        }
        for(auto BPeaBullet : BPeavec) {
            for(auto Z : zombies) {
                if(BPeaBullet && BPeaBullet->geometry().intersects(Z->geometry())){
                    Z->decreaseHP(BPeaBullet->getPower());
                    if(Z->getHp()<=0){
                        decreasePlantsTargets(Z->y());
                        zombies.erase(std::find(zombies.begin(),zombies.end(),Z));
                        delete Z;
                    }
                }
            }
        }
}


void MainWindow::on_Edit_information_botton_clicked()
{
    ui->GameControl->setCurrentIndex(10);
    QPixmap NewInform(":/Images/PasswordReset.png");

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(NewInform));
    ui->Edit_Information_page->setPalette(palette);
    ui->Edit_Information_page->setAutoFillBackground(true);
    ui->lineEdit_13->setText(Player.get_name());
    ui->lineEdit_14->setText(Player.get_phoneNumber());
    ui->lineEdit_15->setText(Player.get_email());
}


void MainWindow::on_NewInform_Back_clicked()
{


    ui->GameControl->setCurrentIndex(6);
}


void MainWindow::on_NewInform_Ok_clicked()
{
    if(ui->lineEdit_13->text().isEmpty()||ui->lineEdit_14->text().isEmpty()||ui->lineEdit_15->text().isEmpty()
            ||ui->lineEdit_16->text().isEmpty()){
        QMessageBox::warning(this,"Error", "pleas fill in all the blanks\n");
        return;
    }
    QString output="14,";
    QRegularExpression phonenumRegex(R"(\b09\d{9}\b)");
    if (!phonenumRegex.match(ui->lineEdit_14->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid phonenumber format\n");
        return;
    }
    QRegularExpression emailRegex(R"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");
    if (!emailRegex.match(ui->lineEdit_15->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid email format\n");
        return;
    }
    if(ui->lineEdit_16->text().length()<8){
        QMessageBox::warning(this,"Error", "Password is weak!!\n");
        return;
    }
    output += ui->lineEdit_13->text() + ",";
    output += Player.get_username() + ",";
    output += ui->lineEdit_16->text()+ ",";;
    output += ui->lineEdit_14->text() + ",";
    output += ui->lineEdit_15->text() + ",";


    socket->write(output.toUtf8());
}



void MainWindow::on_historyButton_clicked()
{
    QString output = "His," + Player.get_username();
    socket->write(output.toUtf8());
}


void MainWindow::on_history_Back_clicked()
{
    ui->GameControl->setCurrentIndex(6);
}

int C_O = 0;
void MainWindow::on_Chat_Button_clicked()
{
    if(C_O==0){
        ui->ChatRoom->show();
        C_O=1;
    }
    else{
        ui->ChatRoom->hide();
        C_O=0;
    }

}

void MainWindow::onDisconnected()
{
    QMessageBox::critical(this, "Connection", "Connection lost!");
    this->setFixedSize(800, 800);
    ui->GameControl->setFixedSize(800, 800);
    this->move(400,0);
    Player.Point =0;
    Player.CurrentRound ="1";
    Peavec.clear();
    BPeavec.clear();
    plants.clear();
    zombies.clear();
    fullLocations.clear();
    ui->GameControl->setCurrentIndex(5);
}


void MainWindow::on_S1_clicked()
{
    QString Output = "Chat,";
    Output += ":/Images/C1.png";
    socket->write(Output.toUtf8());
}


void MainWindow::on_S2_clicked()
{
    QString Output = "Chat,";
    Output += ":/Images/C2.png";
    socket->write(Output.toUtf8());
}


void MainWindow::on_S3_clicked()
{
    QString Output = "Chat,";
    Output += ":/Images/C3.png";
    socket->write(Output.toUtf8());
}


void MainWindow::on_S4_clicked()
{
    QString Output = "Chat,";
    Output += ":/Images/C4.png";
    socket->write(Output.toUtf8());
}

void MainWindow::on_S5_clicked()
{
    QString Output = "Chat,";
    Output += ":/Images/C5.png";
    socket->write(Output.toUtf8());
}

void MainWindow::on_S6_clicked()
{
    QString Output = "Chat,";
    Output += ":/Images/C1.png";
    socket->write(Output.toUtf8());
}

