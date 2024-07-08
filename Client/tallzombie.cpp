#include "tallzombie.h"

TallZombie::TallZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&TallZombie::move);
    attackPower = 30;
    HP = 500;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/Tall_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 200);
    show();
}
void TallZombie::move(){
    setGeometry(this->x()-5.3, this->y(), 100, 200);
}
