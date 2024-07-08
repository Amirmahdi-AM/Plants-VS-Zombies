#include "purplehairzombie.h"

PurpleHairZombie::PurpleHairZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&PurpleHairZombie::move);
    attackPower = 75;
    HP = 4000;
    moveMentDelay = 50;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/PurpleHead_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
void PurpleHairZombie::move(){
    setGeometry(this->x()-5.3, this->y(), 100, 100);
}
