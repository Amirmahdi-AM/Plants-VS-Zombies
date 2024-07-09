#include "bucketheadzombie.h"

BucketHeadZombie::BucketHeadZombie(int x, int y, QWidget *parent)  : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&BucketHeadZombie::move);
    attackPower = 50;
    HP = 1950;
    moveMentDelay = 200;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/Bucket_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &BucketHeadZombie::onAttack);
}
void BucketHeadZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}
