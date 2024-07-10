#include "purplehairzombie.h"

PurpleHairZombie::PurpleHairZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&PurpleHairZombie::move);
    attackPower = 75;
    HP = 4000;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/PurpleHead_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    this->target=NULL;
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &PurpleHairZombie::onAttack);
    TBA = 500;
}
void PurpleHairZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}
