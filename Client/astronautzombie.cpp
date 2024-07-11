#include "astronautzombie.h"


AstronautZombie::AstronautZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,&AstronautZombie::move);
    attackPower = 20;
    HP = 500;
    moveMentDelay = 100;
    moveTimer->start(moveMentDelay);
    picture.load(":/Images/Austronut_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    attackTimer = new QTimer(this);
    this->target=NULL;
    connect(attackTimer, &QTimer::timeout, this, &AstronautZombie::onAttack);
    TBA = 1000;
}

void AstronautZombie::decreaseHP(int _power)
{
    HP -= _power;
    if (HP <= 100) {
        TBA = 500;
        moveMentDelay = 50;
        if(moveTimer->isActive()) {
            moveTimer->stop();
            moveTimer->start(moveMentDelay);
        }
        if(attackTimer->isActive()) {
            attackTimer->stop();
            attackTimer->start(TBA);
        }
    }
}
void AstronautZombie::move(){
    setGeometry(this->x()-moveX, this->y(), 100, 100);
}

void AstronautZombie::onAttack()
{
    target->decreaseHP(attackPower);
    if (target->getHP() <= 0) {
        this->setMovie(WalkingAnimation);
        emit cleanLocation(target->x(), target->y());
        target->setGeometry(-200, -200, 100, 100);
        onMovemevt();
        target = NULL;
    }
}
void AstronautZombie::offMovement()
{
    moveTimer->stop();
    attackTimer->start(TBA);
}
