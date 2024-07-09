#include "regularzombie.h"

RegularZombie::RegularZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&RegularZombie::move);
    attackPower = 25;
    HP = 500;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/Regular_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
void RegularZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}
