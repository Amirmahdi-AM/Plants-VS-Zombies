#include "zombies.h"

int Zombies::getHp()
{
    return HP;
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
}

void Zombies::onMovemevt()
{
    moveX = 5.3;
}
