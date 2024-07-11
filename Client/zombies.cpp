#include "zombies.h"
#include "qmovie.h"
#include <QMessageBox>

int Zombies::getHp()
{
    return HP;
}

void Zombies::onAttack()
{
    target->decreaseHP(attackPower);
    if (target->getHP() <= 0) {
        EatingAnimation->stop();
        this->setMovie(WalkingAnimation);
        WalkingAnimation->start();
        target->setGeometry(-200, -200, 100, 100);
        emit cleanLocation(target->x(), target->y());
        onMovemevt();
        target = NULL;
    }
}

Zombies::Zombies(QWidget *parent) : QLabel(parent) {

}

void Zombies::decreaseHP(int power)
{
    HP-=power;
}

void Zombies::offMovement()
{
    moveTimer->stop();
    attackTimer->start(TBA);
    WalkingAnimation->stop();
    this->setMovie(EatingAnimation);
    EatingAnimation->start();
}

void Zombies::onMovemevt()
{
    attackTimer->stop();
    moveTimer->start(moveMentDelay);

}
