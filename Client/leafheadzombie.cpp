#include "leafheadzombie.h"
#include "qmovie.h"

LeafHeadZombie::LeafHeadZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&LeafHeadZombie::move);
    attackPower = 25;
    HP = 800;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
//    picture.load(":/Images/leaf_head_Z.png");
//    setPixmap(picture);
    WalkingAnimation = new QMovie(":/Images/LeafHeadZombie_Walking.gif");
    EatingAnimation = new QMovie(":/Images/RegularZombie_Eat.gif");
    this->setMovie(WalkingAnimation);
    WalkingAnimation->start();
    setGeometry(x, y, 100, 100);
    show();
    this->target=NULL;
    attackTimer = new QTimer(this);
    connect(attackTimer, &QTimer::timeout, this, &LeafHeadZombie::onAttack);
    TBA = 1000;
}
void LeafHeadZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}

