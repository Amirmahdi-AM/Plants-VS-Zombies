#include "tallzombie.h"

TallZombie::TallZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&TallZombie::move);
    attackPower = 30;
    HP = 500;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
    //picture.load(":/Images/Tall_Z.png");
    //setPixmap(picture);
    WalkingAnimation = new QMovie(":/Images/TallZombie_Eating_Walking.gif");
    EatingAnimation = new QMovie(":/Images/TallZombie_Eating.gif");
    this->setMovie(WalkingAnimation);
    WalkingAnimation->start();
    setGeometry(x, y, 100, 100);
    show();
    this->target=NULL;
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &TallZombie::onAttack);
    TBA = 1000;
}
void TallZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}

