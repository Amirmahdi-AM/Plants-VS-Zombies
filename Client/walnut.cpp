#include "walnut.h"

Walnut::Walnut(int x, int y, QWidget *parent) : Plants(parent) {
    picture.load(":/Images/Walnut_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    HP = 400;
}
