#include "leafheadzombie.h"

LeafHeadZombie::LeafHeadZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&LeafHeadZombie::move);
    attackPower = 25;
    HP = 800;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/leaf_head_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &LeafHeadZombie::onAttack);
}
void LeafHeadZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}
