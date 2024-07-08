#include "purplehairzombie.h"

PurpleHairZombie::PurpleHairZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    picture.load(":/Images/PurpleHead_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
