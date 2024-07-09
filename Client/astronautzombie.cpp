#include "astronautzombie.h"


AstronautZombie::AstronautZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&AstronautZombie::move);
    attackPower = 20;
    HP = 500;
    moveMentDelay = 50;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/Austronut_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
void AstronautZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}
