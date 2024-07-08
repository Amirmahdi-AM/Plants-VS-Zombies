#include "tallzombie.h"



TallZombie::TallZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    picture.load(":/Images/Tall_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 200);
    show();
}
