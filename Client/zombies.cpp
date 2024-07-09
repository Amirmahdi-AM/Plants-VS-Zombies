#include "zombies.h"

int Zombies::getHp()
{
    return HP;
}

void Zombies::onAttack()
{
    target->decreaseHP(attackPower);
}

Zombies::Zombies(QWidget *parent) : QLabel(parent) {

}

void Zombies::decreaseHP(int power)
{
    HP-=power;
}

void Zombies::offMovement()
{
    moveX = 0;
    attackTimer->start(TBA);
}

void Zombies::onMovemevt()
{
    moveX = 5.3;
    if(attackTimer->isActive()) {
        attackTimer->stop();
    }
}
