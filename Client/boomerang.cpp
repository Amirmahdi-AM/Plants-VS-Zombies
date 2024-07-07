#include "boomerang.h"

Boomerang::Boomerang(int x, int y, QWidget *parent) : Plants(parent) {
    picture.load(":/Images/Boomerang_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
