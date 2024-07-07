#include "plummine.h"

PlumMine::PlumMine(int x, int y, QWidget *parent) : Plants(parent) {
    picture.load(":/Images/Plum_Mine_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
