#include "bucketheadzombie.h"
#include "qmovie.h"

BucketHeadZombie::BucketHeadZombie(int x, int y, QWidget *parent)  : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&BucketHeadZombie::move);
    attackPower = 50;
    HP = 1950;
    moveMentDelay = 200;
    moveTimer->start(moveMentDelay);
    WalkingAnimation = new QMovie(":/Images/BucketHead_Walking.gif");
    EatingAnimation = new QMovie(":/Images/BucketHead_Eating.gif");
    this->setMovie(WalkingAnimation);
    WalkingAnimation->start();
    //setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    this->target=NULL;
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &BucketHeadZombie::onAttack);
    TBA = 1000;
}
void BucketHeadZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}
