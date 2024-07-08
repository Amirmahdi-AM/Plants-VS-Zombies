#include "regularzombie.h"

RegularZombie::RegularZombie(int x, int y, QWidget *parent) : Zombies(parent)
{
    picture.load(":/Images/Regular_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
