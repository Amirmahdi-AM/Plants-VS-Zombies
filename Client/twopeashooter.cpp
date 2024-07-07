#include "twopeashooter.h"

TwoPeaShooter::TwoPeaShooter(int x, int y, QWidget *parent) : Plants(parent) {
    picture.load(":/Images/two_peashooter_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
