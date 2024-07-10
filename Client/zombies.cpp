#include "zombies.h"
#include <QMessageBox>

int Zombies::getHp()
{
    return HP;
}

void Zombies::onAttack()
{
    target->decreaseHP(attackPower);
    if (target->getHP() <= 0) {
        emit cleanLocation(target->x(), target->y());
        target->setGeometry(-200, -200, 100, 100);
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
}

void Zombies::onMovemevt()
{
    attackTimer->stop();
    moveTimer->start(moveMentDelay);

}
