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
