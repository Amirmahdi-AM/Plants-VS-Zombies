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
    ////////////////////////////////////////////////////////////////////////
    QPixmap server_page(":/Images/server_connection.png");

    QPalette palette100;
    palette100.setBrush(QPalette::Window, QBrush(server_page));
    ui->server->setPalette(palette100);
    ui->server->setAutoFillBackground(true);
    connect(this,&MainWindow::brainClicked, this, &MainWindow::on_Spawned_Item_Lable_clicked);
    connect(this,&MainWindow::sunClicked, this, &MainWindow::on_Spawned_Item_Lable_clicked);
//    ui->Spawned_brain->setContentsMargins(0, 0, 0, 0);
//    ui->Spawned_brain->setStyleSheet("margin: 0px;");
//    ui->Spawned_brain->setAutoFillBackground(true);
//    ui->Spawned_brain->setAlignment(Qt::AlignCenter);

    /////////////////////////////////////////////////////////////////////////
    ///threads connection
    Rotate = new QTimer();
    connect(Rotate,&QTimer::timeout,this,&MainWindow::Loading_rotation);
    Rotate2 = new QTimer();
    connect(Rotate2,&QTimer::timeout,this,&MainWindow::waiting_rotation);

    Sun_Rotate = new QTimer();
    connect(Sun_Rotate,&QTimer::timeout,this,&MainWindow::sun_rotation);
    Sun_spawn = new QTimer();
    connect(Sun_spawn,&QTimer::timeout,this,&MainWindow::Spawnning_Item);
    fade = new QTimer(this);
    connect(fade,&QTimer::timeout,this,&MainWindow::Fade_Item);

    Labeldrag_drop = new QTimer(this);
    connect(Labeldrag_drop,&QTimer::timeout,this,&MainWindow::Drag_Lable);

    brain_Rotate = new QTimer();
    connect(brain_Rotate,&QTimer::timeout,this,&MainWindow::brain_rotation);
    brain_spawn = new QTimer();
    connect(brain_spawn,&QTimer::timeout,this,&MainWindow::Spawnning_Item);
    Brainfade = new QTimer(this);
    connect(Brainfade,&QTimer::timeout,this,&MainWindow::Fade_Item);
    checkCollision = new QTimer(this);
    connect(checkCollision,&QTimer::timeout,this,&MainWindow::onCheckcollision);

    Bulletcollision = new QTimer(this);
    connect(Bulletcollision,&QTimer::timeout,this,&MainWindow::onBulletcollision);

    ////////////////////////////////////////////////////////////////////////
    /// socket connection
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, QOverload<QTcpSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &MainWindow::onError);
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
                z->setGeometry(z->x()-100,z->y(),100,100);
            }
        }
    }
    if(type=="Jalopino"){
        for(auto z: zombies){
            if(z->y()==_y){
                z->decreaseHP(300);
                z->setGeometry(z->x()-100,z->y(),100,100);
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
    //output+=ui->lineEdit_3->text()+","+ui->lineEdit_4->text()+",";
    QRegularExpression phonenumRegex(R"(\b09\d{9}\b)");
    if (!phonenumRegex.match(ui->lineEdit_5->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid phonenumber format\n");
        return;
    }
    //output+=ui->lineEdit_5->text()+",";
    QRegularExpression emailRegex(R"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");
    if (!emailRegex.match(ui->lineEdit_6->text()).hasMatch()) {
        QMessageBox::warning(this,"Error", "Invalid email format\n");
        return;
    }
    //output+=ui->lineEdit_6->text()+",";
    if(ui->lineEdit_7->text().length()<8){
        QMessageBox::warning(this,"Error", "Password is weak!!\n");
        return;
    }
    if(ui->lineEdit_7->text()!=ui->lineEdit_8->text()){
        QMessageBox::warning(this,"Error", "Password confirmation failed\n");
        return;
    }
    //output+=ui->lineEdit_7->text()+"\n";
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
    for(auto temp : PSPvec){
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
    //cheking in server if was true.go to set new password page

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
    /////////////////////////////////////////
    QPixmap GroundPic(":/Images/field.png");
    ui->Ground->setFixedSize(1500,700);
    ui->Ground->setPixmap(GroundPic);
    /////////////////////////////////////////
    QPixmap Sun(":/Images/sun.png");
    ui->Moving_sun->setFixedSize(90,74);
    ui->Moving_sun->setPixmap(Sun);
    Sun_Rotate->start(10);
    Sun_spawn->start(6000);
    /////////////////////////////////////////
    ui->Plants_point_Label->setText(QString("%1").arg(Player.Point));
}

void MainWindow::Zombies_set(){
    ui->GameControl->setCurrentIndex(9);
    this->setFixedSize(1500,800);
    this->move(15,0);
    ui->GameControl->setFixedSize(1500,800);
    /////////////////////////////////////////
    QPixmap GroundPic(":/Images/field.png");
    ui->Ground_2->setFixedSize(1500,700);
    ui->Ground_2->setPixmap(GroundPic);
    /////////////////////////////////////////
    QPixmap brain(":/Images/Brain.png");
    ui->Moving_brain->setFixedSize(90,74);
    ui->Moving_brain->setPixmap(brain);
    brain_Rotate->start(10);
    brain_spawn->start(6000);
    /////////////////////////////////////////
    ui->Spawned_brain->setGeometry(-10,-10,80,74);
    ui->Zombies_point_Lable->setText(QString("%1").arg(Player.Point));
}

void MainWindow::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString response(data);

    QStringList fields = response.split(",");

    if (fields[0] == "111"){
        QMessageBox::critical(this, "Signup", "This username already token!");
    }
    if (fields[0] == "0"){
        P_or_Z = 1;
        checkCollision->start(200);
        Bulletcollision->start(50);
        currentMap = ui->Plants_map;
        Plants_set();
    }
    if (fields[0] == "1"){
        P_or_Z = -1;
        checkCollision->start(200);
        Bulletcollision->start(50);
        currentMap = ui->Zombies_map;
        Zombies_set();
    }
    if (fields[0] == "113"){
        Player.set_name(fields[1]);
        Player.set_username(fields[3]);
        Player.set_password(fields[2]);
        Player.set_phoneNumber(fields[4]);
        Player.set_email(fields[5]);
        Player.winRound = 0;
        Player.Point = 20000;
        ui->GameControl->setCurrentIndex(6);
        ////////////////////////////////////////
//        ui->GameControl->setCurrentIndex(9);
//        P_or_Z = -1;
//        Zombies_set();
//        currentMap = ui->Zombies_map;
        ////////////////////////////////////////////
        //ui->GameControl->setCurrentIndex(7);
        //P_or_Z = 1;
        //Plants_set();
        QPixmap MenuBackground(":/Images/MenuBG.png");

        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(MenuBackground));
        ui->Menu->setPalette(palette);
        ui->Menu->setAutoFillBackground(true);
    }

    if (fields[0] == "114"){
        QMessageBox::critical(this, "Signin", "Incorrect username/password!");
    }
    if (fields[0] == "1151"){
        ui->GameControl->setCurrentIndex(4);
        QPixmap ResetPass(":/Images/PasswordReset.png");

        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(ResetPass));
        ui->SetnewPass->setPalette(palette);
        ui->SetnewPass->setAutoFillBackground(true);
    }
    if (fields[0] == "1152"){
        QMessageBox::critical(this, "Reset password", "Incorrect Emial/Phonenumber!");
    }
    if (fields[0] == "116"){
        QMessageBox::information(this, "Pass changed", "Password successfully changed");
        ui->GameControl->setCurrentIndex(1);
    }
    if (fields[0] == "card"){

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
            /*
//            for(auto temp : RZvec){
//                if(temp->y()==fields[3].toInt()&&temp->x()>=fields[2].toInt()){
//                    Targetcounts++;
//                }
//            }
//            for(auto temp : BHZvec){
//                if(temp->y()==fields[3].toInt()&&temp->x()>=fields[2].toInt()){
//                    Targetcounts++;
//                }
//            }
//            for(auto temp : LHZvec){
//                if(temp->y()==fields[3].toInt()&&temp->x()>=fields[2].toInt()){
//                    Targetcounts++;
//                }
//            }
//            for(auto temp : PHZvec){
//                if(temp->y()==fields[3].toInt()&&temp->x()>=fields[2].toInt()){
//                    Targetcounts++;
//                }
//            }
//            for(auto temp : TZvec){
//                if(temp->y()==fields[3].toInt()&&temp->x()>=fields[2].toInt()){
//                    Targetcounts++;
//                }
//            }
//            for(auto temp : AZvec){
//                if(temp->y()==fields[3].toInt()&&temp->x()>=fields[2].toInt()){
//                    Targetcounts++;
//                }
//            }
*/
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
            /*
//            for(auto temp : PSPvec){
//                if(temp->y()==fields[3].toInt()){
//                    temp->target++;
//                }
//            }
//            for(auto temp : BPvec){
//                if(temp->y()==fields[3].toInt()){
//                    temp->target++;
//                }
//            }
//            for(auto temp : TPSPvec){
//                if(temp->y()==fields[3].toInt()){
//                    temp->target++;
//                }
//            }
*/
        }




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
        Brainfade->start(3000);
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
            if (fade->isActive()){
                fade->stop();
            }
    }
    if(P_or_Z==-1){
        ui->Spawned_brain->setStyleSheet("background-image: url(:/Images/FadeBrain.png);");
            if (Brainfade->isActive()){
                Brainfade->stop();
            }
    }
}

void MainWindow::on_Start_Game_Botton_clicked()
{
    socket->write("13");
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
    if (Brainfade->isActive()){
        Brainfade->stop();
    }

    spawnedItemp_Label->setWindowFlags(Qt::FramelessWindowHint);
    QPropertyAnimation *animation = new QPropertyAnimation(spawnedItemp_Label, "geometry");
    animation->setDuration(1000);
    QPoint Spawned_ItemPos = spawnedItemp_Label->pos();
    int x = Spawned_ItemPos.x();
    int y = Spawned_ItemPos.y();
    animation->setStartValue(QRect(x, y, 100,100));
    //QRect(13, 10, 80,74
    animation->setEndValue(QRect(13, 10, 100,100));
    QObject::connect(animation, &QPropertyAnimation::finished, spawnedItemp_Label,[&](){
        Player.Point += 25;
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

        //&QLabel::hide;
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
            else if (x >= 806 && x< 913){
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
            for (auto temp : fullLocations) {
                if (temp.first == x && temp.second == y)
                    validate = false;
            }
            draging_Label->setGeometry(740, 10, 100, 100);
            if (validate) {
                if(selection!=5&&selection!=5){
                 fullLocations.push_back(make_pair(x,y));
                }
                QString output = "card,";
                if (selection == 1){
                    output += "PSP,";
                    output += QString::number(x)+","+QString::number(y);
                    //PeaShooter* ps = new PeaShooter(x, y, ui->Plants_map);
                    Player.Point -= 50;
                }

                if (selection == 2){
                    output += "TPSP,";
                    output += QString::number(x)+","+QString::number(y);
                    //TwoPeaShooter* tps = new TwoPeaShooter(x, y, ui->Plants_map);
                    Player.Point -= 100;
                }

                if (selection == 3){
                    output += "WP,";
                    output += QString::number(x)+","+QString::number(y);
                    //Walnut *w = new Walnut(x, y, ui->Plants_map);
                    Player.Point -= 100;
                }

                if (selection == 4){
                    output += "PMP,";
                    output += QString::number(x)+","+QString::number(y);
                    //PlumMine *pm = new PlumMine(x, y, ui->Plants_map);
                    Player.Point -= 175;
                }

                if (selection == 5){
                    output += "JP,";
                    output += QString::number(x)+","+QString::number(y);
                    //Jalapeno *j = new Jalapeno(x, y, ui->Plants_map);
                    Player.Point -= 150;
                }

                if (selection == 6){
                    output += "BP,";
                    output += QString::number(x)+","+QString::number(y);
                    //Boomerang *b = new Boomerang(x, y, ui->Plants_map);
                    Player.Point -= 125;
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
                    //RegularZombie* rz = new RegularZombie(x, y, ui->Zombies_map);
                    Player.Point -= 100;
                }

                if (selection == 4){
                    output += "BHZ,";
                    output += QString::number(x)+","+QString::number(y);
                    //BucketHeadZombie* bhz = new BucketHeadZombie(x, y, ui->Zombies_map);
                    Player.Point -= 200;
                }

                if (selection == 5){
                    output += "LHZ,";
                    output += QString::number(x)+","+QString::number(y);
                    //LeafHeadZombie *lhz = new LeafHeadZombie(x, y, ui->Zombies_map);
                    Player.Point -= 150;
                }

                if (selection == 2){
                    output += "TZ,";
                    output += QString::number(x)+","+QString::number(y);
                    //TallZombie *tz = new TallZombie(x, y, ui->Zombies_map);
                    Player.Point -= 150;
                }

                if (selection == 3){
                    output += "AZ,";
                    output += QString::number(x)+","+QString::number(y);
                    //AstronautZombie *az = new AstronautZombie(x, y, ui->Zombies_map);
                    Player.Point -= 200;
                }

                if (selection == 1){
                    output += "PHZ,";
                    output += QString::number(x)+","+QString::number(y);
                    //PurpleHairZombie *phz = new PurpleHairZombie(x, y, ui->Zombies_map);
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
           Walnut* W = dynamic_cast<Walnut*>(p);
           TallZombie* tz = dynamic_cast<TallZombie*>(z);
           if (W && tz) {
               tz->setGeometry(W->x() - 100, tz->y(), 100, 100);
           }
           else if (p->geometry().intersects(z->geometry())) {
               z->offMovement();
               z->target = p;
           }
       }
   }
/*
    for(auto p : PSPvec){
            for (auto z : RZvec) {
                if(z->target!= NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : BHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : LHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : PHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : AZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : TZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
        }
        for(auto p : TPSPvec){
            for (auto z : RZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : BHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : LHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : PHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : AZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : TZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
        }
        for(auto p : WPvec){
            for (auto z : RZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : BHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : LHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : PHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : AZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
        }

        for(auto p : BPvec){
            for (auto z : RZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : BHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : LHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : PHZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : AZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
            for (auto z : TZvec) {
                if(z->target!=NULL){
                    continue;
                }
                if (p->geometry().intersects(z->geometry())) {
                    z->offMovement();
                    z->target = p;
                }
            }
        }
        */
}

void MainWindow::onCleanLocation(int x, int y)
{
    auto it = std::find(fullLocations.begin(), fullLocations.end(), make_pair(x,y));
    if (it != fullLocations.end()) {
        Locationmute.lock();
        fullLocations.erase(it);
        Locationmute.unlock();
    }


}

void MainWindow::onCreateBBullets(int x, int y, int _power)
{
    BoomerangPea *bullet = new BoomerangPea(x, y, _power,currentMap);
    PeaBulletmute.lock();
    BPeavec.push_back(bullet);
    BPeaBulletmute.unlock();
}

void MainWindow::onBulletcollision()
{
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
            }
        }
    }
    /*
    for(auto PeaBullet:Peavec){
        for(auto Z:RZvec){
            if(PeaBullet && PeaBullet->geometry().intersects(Z->geometry())){
                Z->decreaseHP(PeaBullet->getPower());
                if(Z->getHp()<=0){
                    decreasePlantsTargets(Z->y());
                    RZvec.erase(std::find(RZvec.begin(),RZvec.end(),Z));
                    delete Z;
                }
                Peavec.erase(std::find(Peavec.begin(),Peavec.end(),PeaBullet));
                delete PeaBullet;
            }
        }
        for(auto Z:BHZvec){
            if(PeaBullet && PeaBullet->geometry().intersects(Z->geometry())){
                Z->decreaseHP(PeaBullet->getPower());
                if(Z->getHp()<=0){
                    decreasePlantsTargets(Z->y());
                    BHZvec.erase(std::find(BHZvec.begin(),BHZvec.end(),Z));
                    delete Z;
                }
                Peavec.erase(std::find(Peavec.begin(),Peavec.end(),PeaBullet));
                delete PeaBullet;
            }
        }
        for(auto Z:LHZvec){
            if(PeaBullet && PeaBullet->geometry().intersects(Z->geometry())){
                Z->decreaseHP(PeaBullet->getPower());
                if(Z->getHp()<=0){
                    decreasePlantsTargets(Z->y());
                    LHZvec.erase(std::find(LHZvec.begin(),LHZvec.end(),Z));
                    delete Z;
                }
                Peavec.erase(std::find(Peavec.begin(),Peavec.end(),PeaBullet));
                delete PeaBullet;
            }
        }
        for(auto Z:PHZvec){
            if(PeaBullet && PeaBullet->geometry().intersects(Z->geometry())){
                Z->decreaseHP(PeaBullet->getPower());
                if(Z->getHp()<=0){
                    decreasePlantsTargets(Z->y());
                    PHZvec.erase(std::find(PHZvec.begin(),PHZvec.end(),Z));
                    delete Z;
                }
                Peavec.erase(std::find(Peavec.begin(),Peavec.end(),PeaBullet));
                delete PeaBullet;
            }
        }
        for(auto Z:TZvec){
            if(PeaBullet && PeaBullet->geometry().intersects(Z->geometry())){
                Z->decreaseHP(PeaBullet->getPower());
                if(Z->getHp()<=0){
                    decreasePlantsTargets(Z->y());
                    TZvec.erase(std::find(TZvec.begin(),TZvec.end(),Z));
                    delete Z;
                }
                Peavec.erase(std::find(Peavec.begin(),Peavec.end(),PeaBullet));
                delete PeaBullet;
            }
        }
        for(auto Z:AZvec){
            if(PeaBullet && PeaBullet->geometry().intersects(Z->geometry())){
                Z->decreaseHP(PeaBullet->getPower());
                if(Z->getHp()<=0){
                    decreasePlantsTargets(Z->y());
                    AZvec.erase(std::find(AZvec.begin(),AZvec.end(),Z));
                    delete Z;
                }
                Peavec.erase(std::find(Peavec.begin(),Peavec.end(),PeaBullet));
                delete PeaBullet;
            }
        }
    }
    */
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
                    BPeavec.erase(std::find(BPeavec.begin(),BPeavec.end(),BPeaBullet));
                    delete BPeaBullet;
                }
            }
        }
        /*
        for(auto BPeaBullet: BPeavec){
            for(auto Z:RZvec){
                if(BPeaBullet && BPeaBullet->geometry().intersects(Z->geometry())){
                    Z->decreaseHP(BPeaBullet->getPower());
                    if(Z->getHp()<=0){
                        decreasePlantsTargets(Z->y());
                        RZvec.erase(std::find(RZvec.begin(),RZvec.end(),Z));
                        delete Z;
                    }
                }
            }
            for(auto Z:BHZvec){
                if(BPeaBullet && BPeaBullet->geometry().intersects(Z->geometry())){
                    Z->decreaseHP(BPeaBullet->getPower());
                    if(Z->getHp()<=0){
                        decreasePlantsTargets(Z->y());
                        BHZvec.erase(std::find(BHZvec.begin(),BHZvec.end(),Z));
                        delete Z;
                    }
                }
            }
            for(auto Z:LHZvec){
                if(BPeaBullet && BPeaBullet->geometry().intersects(Z->geometry())){
                    Z->decreaseHP(BPeaBullet->getPower());
                    if(Z->getHp()<=0){
                        decreasePlantsTargets(Z->y());
                        LHZvec.erase(std::find(LHZvec.begin(),LHZvec.end(),Z));
                        delete Z;
                    }
                }
            }
            for(auto Z:PHZvec){
                if(BPeaBullet && BPeaBullet->geometry().intersects(Z->geometry())){
                    Z->decreaseHP(BPeaBullet->getPower());
                    if(Z->getHp()<=0){
                        decreasePlantsTargets(Z->y());
                        PHZvec.erase(std::find(PHZvec.begin(),PHZvec.end(),Z));
                        delete Z;
                    }
                }
            }
            for(auto Z:TZvec){
                if(BPeaBullet && BPeaBullet->geometry().intersects(Z->geometry())){
                    Z->decreaseHP(BPeaBullet->getPower());
                    if(Z->getHp()<=0){
                        decreasePlantsTargets(Z->y());
                        TZvec.erase(std::find(TZvec.begin(),TZvec.end(),Z));
                        delete Z;
                    }
                }
            }
            for(auto Z:AZvec){
                if(BPeaBullet && BPeaBullet->geometry().intersects(Z->geometry())){
                    Z->decreaseHP(BPeaBullet->getPower());
                    if(Z->getHp()<=0){
                        decreasePlantsTargets(Z->y());
                        AZvec.erase(std::find(AZvec.begin(),AZvec.end(),Z));
                        delete Z;
                    }
                }
            }

        }*/
}






