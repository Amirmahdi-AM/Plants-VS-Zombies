#include "astronautzombie.h"


AstronautZombie::AstronautZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    picture.load(":/Images/Austronut_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
