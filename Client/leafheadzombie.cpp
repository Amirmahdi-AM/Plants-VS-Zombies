#include "leafheadzombie.h"

LeafHeadZombie::LeafHeadZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    picture.load(":/Images/LeafHead_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
