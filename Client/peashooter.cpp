#include "peashooter.h"

PeaShooter::PeaShooter(int x, int y, QWidget *parent) :Plants(parent) {
    picture.load(":/Images/Peashooter_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
