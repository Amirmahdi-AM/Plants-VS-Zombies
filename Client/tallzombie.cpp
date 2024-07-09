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
    this->target=NULL;
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &TallZombie::onAttack);
}
void TallZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 200);
}
