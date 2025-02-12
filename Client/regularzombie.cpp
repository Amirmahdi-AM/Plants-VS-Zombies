#include "regularzombie.h"
#include "qmovie.h"

RegularZombie::RegularZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&RegularZombie::move);
    attackPower = 25;
    HP = 500;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
//    picture.load(":/Images/Regular_Z.png");
//    setPixmap(picture);
    WalkingAnimation = new QMovie(":/Images/RegularZombie_Walking.gif");
    EatingAnimation = new QMovie(":/Images/RegularZombie_Eat.gif");
    this->setMovie(WalkingAnimation);
    WalkingAnimation->start();
    setGeometry(x, y, 100, 100);
    show();
    this->target=NULL;
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &RegularZombie::onAttack);
    TBA = 1000;
}
void RegularZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}
