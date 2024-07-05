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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , connectionTimer(new QTimer(this))
{
    ui->setupUi(this);
    this->setFixedSize(800,800);
    this->move(400,0);
    ui->GameControl->setFixedSize(800,800);
    ui->GameControl->setCurrentIndex(5);
    ////////////////////////////////////////////////////////////////////////
    QPixmap server_page(":/Images/server_connection.png");

    QPalette palette100;
    palette100.setBrush(QPalette::Window, QBrush(server_page));
    ui->server->setPalette(palette100);
    ui->server->setAutoFillBackground(true);

    /////////////////////////////////////////////////////////////////////////
    ///threads connection
    Rotate = new QTimer();
    connect(Rotate,&QTimer::timeout,this,&MainWindow::Loading_rotation);
    Rotate2 = new QTimer();
    connect(Rotate2,&QTimer::timeout,this,&MainWindow::waiting_rotation);

    Sun_Rotate = new QTimer();
    connect(Sun_Rotate,&QTimer::timeout,this,&MainWindow::sun_rotation);
    Sun_spawn = new QTimer();
    connect(Sun_spawn,&QTimer::timeout,this,&MainWindow::Spawnning_sun);
    fade = new QTimer(this);
    connect(fade,&QTimer::timeout,this,&MainWindow::Fade_sun);

    ////////////////////////////////////////////////////////////////////////
    /// socket connection
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, QOverload<QTcpSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &MainWindow::onError);
    connect(connectionTimer, &QTimer::timeout, this, &MainWindow::onConnectionTimeout);

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
    QString output="";
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
    output+=ui->lineEdit_10->text()+"\n";
    //cheking in server if was true.go to set new password page
    QPixmap ResetPass(":/Images/PasswordReset.png");

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(ResetPass));
    ui->SetnewPass->setPalette(palette);
    ui->SetnewPass->setAutoFillBackground(true);
    ui->GameControl->setCurrentIndex(4);
}

void MainWindow::on_Ok_newpass_2_clicked()
{
    ui->GameControl->setCurrentIndex(3);
}

void MainWindow::on_Ok_newpass_3_clicked()
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
    QString output="";
    if(ui->lineEdit_11->text().length()<8){
        QMessageBox::warning(this,"Error", "Password is weak!!\n");
        return;
    }
    if(ui->lineEdit_12->text()!=ui->lineEdit_11->text()){
        QMessageBox::warning(this,"Error", "Password confirmation failed\n");
        return;
    }
    output+=ui->lineEdit_11->text()+"\n";
}

void MainWindow::on_connectButton_clicked()
{
    socket->abort();
    socket->connectToHost(ui->IPEdit->text(), ui->portEdit->text().toUShort());
    connectionTimer->start(2000);
}

void MainWindow::Plants_set(){
    ui->GameControl->setCurrentIndex(7);
    this->setFixedSize(1600,900);
    this->move(15,0);
    ui->GameControl->setFixedSize(1600,1000);
    /////////////////////////////////////////
    QPixmap GroundPic(":/Images/field.png");
    ui->Ground->setFixedSize(2000,743);
    ui->Ground->setPixmap(GroundPic);
    /////////////////////////////////////////
    QPixmap Sun(":/Images/sun.png");
    ui->Moving_sun->setFixedSize(105,99);
    ui->Moving_sun->setPixmap(Sun);
    Sun_Rotate->start(10);
    Sun_spawn->start(6000);
    /////////////////////////////////////////
}

void MainWindow::Zombies_set(){
    ui->GameControl->setCurrentIndex(9);
    this->setFixedSize(1600,900);
    this->move(15,0);
    ui->GameControl->setFixedSize(1600,1000);
    /////////////////////////////////////////
    QPixmap GroundPic(":/Images/field.png");
    ui->Ground->setFixedSize(2000,743);
    ui->Ground->setPixmap(GroundPic);
    /////////////////////////////////////////
    QPixmap Sun(":/Images/brain.png");
    ui->Moving_sun->setFixedSize(105,99);
    ui->Moving_sun->setPixmap(Sun);
    Sun_Rotate->start(10);
    Sun_spawn->start(6000);
    /////////////////////////////////////////
}

void MainWindow::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString response(data);

    QStringList fields = response.split(",");
    if (fields[0] == "112") {
        QMessageBox::information(this, "Signup", "Signup successfully!");
    }

    if (fields[0] == "111"){
        QMessageBox::critical(this, "Signup", "This username already token!");
    }
    if (fields[0] == "0"){
        P_or_Z = 1;
        Plants_set();
    }
    if (fields[0] == "1"){
        P_or_Z = -1;
        Zombies_set();
    }
    if (fields[0] == "113"){
        //QMessageBox::information(this, "Signin", "Welcome!");
        ui->GameControl->setCurrentIndex(6);
        QPixmap MenuBackground(":/Images/MenuBG.png");

        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(MenuBackground));
        ui->Menu->setPalette(palette);
        ui->Menu->setAutoFillBackground(true);


//        if(true){
//            Plants_set();
//        }
//        else{
//            Zombies_set();
//        }
    }

    if (fields[0] == "114"){
        QMessageBox::critical(this, "Signin", "Incorrect username/password!");
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

void MainWindow::Spawnning_sun(){
    QPropertyAnimation *animation;
    if(P_or_Z==1){
        ui->Spawned_sun->setFixedSize(105,99);
        ui->Spawned_sun->setStyleSheet("background-image: url(:/Images/sun.png);");
        ui->Spawned_sun->setGeometry(-1, -1, 120, 99);
       animation = new QPropertyAnimation(ui->Spawned_sun, "geometry");
    }
    if(P_or_Z==-1){
        ui->Spawned_brain->setFixedSize(105,99);
        ui->Spawned_brain->setStyleSheet("background-image: url(:/Images/brain.png);");
        ui->Spawned_brain->setGeometry(-1, -1, 120, 99);
        animation = new QPropertyAnimation(ui->Spawned_brain, "geometry");
    }

    fade->start(4000);

    animation->setDuration(2000);
    int x = rand() % 1200 + 200;
    int y = rand() % 550 + 200;
    animation->setStartValue(QRect(x, -1, 150, 30));
    animation->setEndValue(QRect(x, y, 150, 30));
    animation->start();
}

void MainWindow::Fade_sun(){
    ui->Spawned_sun->setStyleSheet("background-image: url(:/Images/FadedSun.png);");
            if (!fade->isActive()){
                fade->stop();
            }
}

void MainWindow::on_Spawned_sun_clicked()
{
    if (!fade->isActive()){
        fade->stop();
    }
    QPropertyAnimation *animation = new QPropertyAnimation(ui->Spawned_sun, "geometry");
    animation->setDuration(1000);
    QPoint Spawned_sunPos = ui->Spawned_sun->pos();
    int x = Spawned_sunPos.x();
    int y = Spawned_sunPos.y();
    animation->setStartValue(QRect(x, y, 150, 30));
    animation->setEndValue(QRect(13, 10, 150, 30));
    animation->start();
}


void MainWindow::on_Start_Game_Botton_clicked()
{
    ui->GameControl->setCurrentIndex(8);
    this->setFixedSize(1600,900);
    this->move(15,0);
    ui->GameControl->setFixedSize(1600,900);

    QPixmap WaitingPic(":/Images/WaitingBG.jpg");
    ui->Waiting_Lable->setFixedSize(1600,1000);
    ui->Waiting_Lable->move(0,-50);
    ui->Waiting_Lable->setPixmap(WaitingPic);
    Rotate2->start(10);
//    Plants_set();
}

